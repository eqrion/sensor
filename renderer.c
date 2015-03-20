#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int_m img_alloc(uint_m width, uint_m height, image_buffer *buf)
{
	uint_m data_length = width * height * sizeof(image_pixel);
	image_pixel *pixels = (image_pixel *)malloc(data_length);

	if (pixels != NULL)
	{
		buf->width = width;
		buf->height = height;
		buf->pixels = pixels;
		return 1;
	}
	return 0;
}
void img_dealloc(image_buffer *buf)
{
	if (buf != NULL && buf->pixels != NULL)
	{
		free(buf->pixels);
	}
}

int_m ds_alloc(uint_m width, uint_m height, ds_buffer *buf)
{
	uint_m data_length = width * height * sizeof(ds_pixel);
	ds_pixel *pixels = (ds_pixel *)malloc(data_length);

	if (pixels != NULL)
	{
		buf->width = width;
		buf->height = height;
		buf->pixels = pixels;
		return 1;
	}
	return 0;
}
void ds_dealloc(ds_buffer *buf)
{
	if (buf != NULL && buf->pixels != NULL)
	{
		free(buf->pixels);
	}
}

int_m vs_validate_desc(vertex_element *elems, uint_m elems_count)
{
	uint_m position_elem_count = 0;
	uint_m previous_elem_end = 0;

	for (uint_m i = 0; i < elems_count; ++i)
	{
		if (elems[i].type == vertex_elem_position_vec3)
			position_elem_count += 1;

		if (elems[i].offset < previous_elem_end)
			return 0;
		previous_elem_end = elems[i].offset + vs_element_size(elems[i].type);
	}

	return !(position_elem_count == 0 || position_elem_count > 1);
}
vertex_element *vs_find_element(vertex_element *elems, uint_m elems_count, uint_m type)
{
	for (uint_m i = 0; i < elems_count; ++i)
	{
		if (elems[i].type == type)
			return elems + i;
	}
	return NULL;
}
uint_m vs_element_size(uint_m type)
{
	switch (type)
	{
	case vertex_elem_position_vec3:
		return 3 * sizeof(real);
	case vertex_elem_color_vec4:
		return 4 * sizeof(real);
	case vertex_elem_normal_vec3:
		return 3 * sizeof(real);
	}
	return 0;
}
uint_m vs_vertex_size(vertex_element *elems, uint_m elems_count)
{
	return elems[elems_count - 1].offset + vs_element_size(elems[elems_count - 1].type);
}

int_m vs_alloc(vertex_element *elems, uint_m elems_count, uint_m verts_count, vertex_buffer *buf)
{
	if (!vs_validate_desc(elems, elems_count))
		return 0;

	uint_m data_length = verts_count * vs_vertex_size(elems, elems_count);
	void *verts = malloc(data_length);

	if (verts != NULL)
	{
		buf->verts = verts;
		buf->verts_count = verts_count;
		buf->vert_desc = elems;
		buf->vert_desc_count = elems_count;
		return 1;
	}
	return 0;	
}
int_m vs_buffer_element(vertex_buffer *buf, int_m element_index, void *elements, uint_m elements_count)
{
	if (buf == NULL)
		return 0;

	if (elements_count > buf->verts_count)
		return 0;

	uint_m vert_size = vs_vertex_size(buf->vert_desc, buf->vert_desc_count);
	uint_m elem_offset = buf->vert_desc[element_index].offset;
	uint_m elem_size = vs_element_size(buf->vert_desc[element_index].type);

	uint8_t *read = (uint8_t *)elements;
	uint8_t *write = (uint8_t *)(buf->verts) + elem_offset;

	while (elements_count > 0)
	{
		memcpy(write, read, elem_size);

		read += elem_size;
		write += vert_size;

		--elements_count;
	}

	return 1;	
}
void vs_dealloc(vertex_buffer *buf)
{
	if (buf != NULL && buf->verts != NULL)
	{
		free(buf->verts);
	}	
}

void rs_clear(image_pixel color, image_buffer *buf)
{
	uint_m pixel_count = buf->width * buf->height;

	for (uint_m i = 0; i < pixel_count; ++i)
	{
		buf->pixels[i].r = color.r;
		buf->pixels[i].g = color.g;
		buf->pixels[i].b = color.b;
	}
}

void rs_order_verts(vector3_f *screen_vertices, uint8_t **vertices)
{
	/* screen_vertices[0] is the highest vert */
	/* screen_vertices[1] is the farthest right vert */
	/* screen_vertices[2] is the farthest left vert */

	/* put the highgest vertex into screen_vertices[0] */
	if (screen_vertices[1].y > screen_vertices[0].y)
	{
		if (screen_vertices[2].y > screen_vertices[1].y)
		{
			vector3_f temp = screen_vertices[2];
			screen_vertices[2] = screen_vertices[0];
			screen_vertices[0] = temp;

			uint8_t *temp_2 = vertices[2];
			vertices[2] = vertices[0];
			vertices[0] = temp_2;
		}
		else
		{
			vector3_f temp = screen_vertices[1];
			screen_vertices[1] = screen_vertices[0];
			screen_vertices[0] = temp;

			uint8_t *temp_2 = vertices[1];
			vertices[1] = vertices[0];
			vertices[0] = temp_2;
		}
	}
	else if (screen_vertices[2].y > screen_vertices[0].y)
	{
		vector3_f temp = screen_vertices[2];
		screen_vertices[2] = screen_vertices[0];
		screen_vertices[0] = temp;

		uint8_t *temp_2 = vertices[2];
		vertices[2] = vertices[0];
		vertices[0] = temp_2;
	}

	/* sort the other two vertices by X coordinate */
	if (screen_vertices[2].x < screen_vertices[1].x)
	{
		vector3_f temp = screen_vertices[2];
		screen_vertices[2] = screen_vertices[1];
		screen_vertices[1] = temp;

		uint8_t *temp_2 = vertices[2];
		vertices[2] = vertices[1];
		vertices[1] = temp_2;
	}
}
void rs_blit_line(
	image_buffer *buf,
	fragment_shader frag_shader,
	vertex_buffer *vert_buffer,
	vector3_f *screen_vertices,
	uint8_t **vertices,
	uint_m line,
	int_m first,
	int_m last
	)
{
	if (first < 0)
		first = 0;
	else if (first > buf->width - 1)
		first = buf->width - 1;

	if (last < 0)
		last = 0;
	else if (last > buf->height - 1)
		last = buf->height - 1;

	image_pixel *blit = &buf->pixels[line * buf->width + first];

	uint8_t fragment_vertex[
		vs_vertex_size(
			vert_buffer->vert_desc,
			vert_buffer->vert_desc_count
			)];

	while (first <= last)
	{
		vector3_f tri_point = {
			first,
			line,
			0
		};
		vector3_f barycentric = convert_to_barycentric(
			tri_point,
			screen_vertices[0],
			screen_vertices[1],
			screen_vertices[2]
			);
		tri_point.z = barycentric.x * screen_vertices[0].z + barycentric.y * screen_vertices[1].z + barycentric.z * screen_vertices[2].z;

		for (uint_m i = 0; i < vert_buffer->vert_desc_count; ++i)
		{
			switch (vert_buffer->vert_desc[i].type)
			{
			case vertex_elem_position_vec3:
			{
				*(vector3_f *)(fragment_vertex + vert_buffer->vert_desc[i].offset) =
					tri_point;
			}
			break;
			case vertex_elem_color_vec4:
			{
				vector4_f color_1 = *(vector4_f *)(vertices[0] + vert_buffer->vert_desc[i].offset);
				vector4_f color_2 = *(vector4_f *)(vertices[1] + vert_buffer->vert_desc[i].offset);
				vector4_f color_3 = *(vector4_f *)(vertices[2] + vert_buffer->vert_desc[i].offset);

				*(vector4_f *)(fragment_vertex + vert_buffer->vert_desc[i].offset) = 
					vec4_add(vec4_add(
						vec4_scale(color_1, barycentric.x),
						vec4_scale(color_2, barycentric.y)
						),
					vec4_scale(color_3, barycentric.z)
					);
			}
			break;
			case vertex_elem_normal_vec3:
			{
				vector3_f normal_1 = *(vector3_f *)(vertices[0] + vert_buffer->vert_desc[i].offset);
				vector3_f normal_2 = *(vector3_f *)(vertices[1] + vert_buffer->vert_desc[i].offset);
				vector3_f normal_3 = *(vector3_f *)(vertices[2] + vert_buffer->vert_desc[i].offset);

				*(vector3_f *)(fragment_vertex + vert_buffer->vert_desc[i].offset) = 
					vec3_add(vec3_add(
						vec3_scale(normal_1, barycentric.x),
						vec3_scale(normal_2, barycentric.y)
						),
					vec3_scale(normal_3, barycentric.z)
					);
			}
			break;
			}
		}

		(*blit) = frag_shader.function(fragment_vertex);

		++blit;
		++first;
	}
}
void rs_draw_trianglelist(vertex_buffer *vert_buffer, uint_m offset, uint_m count, fragment_shader frag_shader, image_buffer *buf)
{
	if (!vert_buffer->verts || !vs_validate_desc(vert_buffer->vert_desc, vert_buffer->vert_desc_count))
	{
		fprintf(stderr, "%s\n", "invalid vertex_buffer object");
		return;
	}
	if (count % 3 != 0)
	{
		fprintf(stderr, "%s\n", "invalid amount of triangles for rasterizing a trianglelist");
		return;
	}
	if (offset + count > vert_buffer->verts_count)
	{
		fprintf(stderr, "%s\n", "invalid range of triangles to render");
	}

	vector2_i buf_bounds = { buf->width - 1, buf->height - 1 };

	uint_m vertex_size = vs_vertex_size(vert_buffer->vert_desc, vert_buffer->vert_desc_count);
	uint_m pos_offset = vs_find_element(vert_buffer->vert_desc, vert_buffer->vert_desc_count, vertex_elem_position_vec3)->offset;

	vector3_f screen_vertices[3];
	uint8_t *vertices[3];
	uint8_t *next_vertex = (uint8_t *)vert_buffer->verts;

	for (uint_m i = 0; i < count;)
	{
		vertices[0] = next_vertex;
		memcpy(screen_vertices + 0, next_vertex + pos_offset, sizeof(vector3_f));
		next_vertex += vertex_size;

		vertices[1] = next_vertex;
		memcpy(screen_vertices + 1, next_vertex + pos_offset, sizeof(vector3_f));
		next_vertex += vertex_size;

		vertices[2] = next_vertex;
		memcpy(screen_vertices + 2, next_vertex + pos_offset, sizeof(vector3_f));
		next_vertex += vertex_size;

		screen_vertices[0].x = (screen_vertices[0].x * buf_bounds.x + buf_bounds.x) * 0.5F;
		screen_vertices[0].y = (screen_vertices[0].y * buf_bounds.y + buf_bounds.y) * 0.5F;

		screen_vertices[1].x = (screen_vertices[1].x * buf_bounds.x + buf_bounds.x) * 0.5F;
		screen_vertices[1].y = (screen_vertices[1].y * buf_bounds.y + buf_bounds.y) * 0.5F;

		screen_vertices[2].x = (screen_vertices[2].x * buf_bounds.x + buf_bounds.x) * 0.5F;
		screen_vertices[2].y = (screen_vertices[2].y * buf_bounds.y + buf_bounds.y) * 0.5F;

		i += 3;

		rs_order_verts(screen_vertices, vertices);

		int_m top_scanline = (int_m)screen_vertices[0].y;
		int_m middle_scanline = (int_m)fmaxf(screen_vertices[1].y, screen_vertices[2].y);
		int_m bottom_scanline = (int_m)fminf(screen_vertices[1].y, screen_vertices[2].y);
		int_m cur_scanline = top_scanline;

		/*
		 * render the top-half of the triangle
		 */
		if (cur_scanline == middle_scanline)
		{
			int_m left_mark = screen_vertices[0].x;
			int_m right_mark = screen_vertices[1].y > screen_vertices[2].y ? screen_vertices[1].x : screen_vertices[2].x;

			if (left_mark > right_mark)
			{
				int_m temp = left_mark;
				left_mark = right_mark;
				right_mark = temp;
			}

			rs_blit_line(
				buf,
				frag_shader,
				vert_buffer,
				screen_vertices,
				vertices,
				cur_scanline,
				left_mark,
				right_mark
				);

			cur_scanline -= 1;
		}
		else
		{
			real left_inv_slope = (screen_vertices[1].x - screen_vertices[0].x) / (screen_vertices[1].y - screen_vertices[0].y);
			real right_inv_slope = (screen_vertices[2].x - screen_vertices[0].x) / (screen_vertices[2].y - screen_vertices[0].y);

			while (cur_scanline > middle_scanline)
			{
				rs_blit_line(
					buf,
					frag_shader,
					vert_buffer,
					screen_vertices,
					vertices,
					cur_scanline,
					(int_m)(screen_vertices[0].x + left_inv_slope * (cur_scanline - top_scanline)),
					(int_m)(screen_vertices[0].x + right_inv_slope * (cur_scanline - top_scanline))
					);

				cur_scanline -= 1;
			}
		}

		/*
		 * render the bottom-half of the triangle
		 */
		if (screen_vertices[1].y == screen_vertices[2].y)
		{
			if (top_scanline != middle_scanline)
			{
				int_m left_mark = screen_vertices[1].x;
				int_m right_mark = screen_vertices[2].x;

				if (left_mark > right_mark)
				{
					int_m temp = left_mark;
					left_mark = right_mark;
					right_mark = temp;
				}

				rs_blit_line(
					buf,
					frag_shader,
					vert_buffer,
					screen_vertices,
					vertices,
					cur_scanline,
					left_mark,
					right_mark
					);

				cur_scanline -= 1;
			}
		}
		else if (screen_vertices[1].y > screen_vertices[2].y)
		{
			real right_inv_slope = (screen_vertices[2].x - screen_vertices[0].x) / (screen_vertices[2].y - screen_vertices[0].y);
			real left_inv_slope = (screen_vertices[2].x - screen_vertices[1].x) / (screen_vertices[2].y - screen_vertices[1].y);

			while (cur_scanline >= bottom_scanline)
			{
				rs_blit_line(
					buf,
					frag_shader,
					vert_buffer,
					screen_vertices,
					vertices,
					cur_scanline,
					(int_m)(screen_vertices[1].x + left_inv_slope * (cur_scanline - middle_scanline)),
					(int_m)(screen_vertices[0].x + right_inv_slope * (cur_scanline - top_scanline))
					);

				cur_scanline -= 1;
			}
		}
		else
		{
			real right_inv_slope = (screen_vertices[1].x - screen_vertices[2].x) / (screen_vertices[1].y - screen_vertices[2].y);
			real left_inv_slope = (screen_vertices[1].x - screen_vertices[0].x) / (screen_vertices[1].y - screen_vertices[0].y);

			while (cur_scanline >= bottom_scanline)
			{
				rs_blit_line(
					buf,
					frag_shader,
					vert_buffer,
					screen_vertices,
					vertices,
					cur_scanline,
					(int_m)(screen_vertices[0].x + left_inv_slope * (cur_scanline - top_scanline)),
					(int_m)(screen_vertices[2].x + right_inv_slope * (cur_scanline - middle_scanline))
					);

				cur_scanline -= 1;
			}
		}
	}
}
