#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int img_alloc(uint32_t width, uint32_t height, image_buffer *buf)
{
	uint32_t data_length = width * height * sizeof(image_pixel);
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

int ds_alloc(uint32_t width, uint32_t height, ds_buffer *buf)
{
	uint32_t data_length = width * height * sizeof(ds_pixel);
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

uint32_t vs_element_size(uint32_t type)
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
uint32_t vs_vertex_size(vertex_element *elems, uint32_t elems_count)
{
	vertex_element *max = elems;
	for (uint32_t i = 1; i < elems_count; ++i)
	{
		if (elems[i].offset > max->offset)
			max = elems;
	}
	return max->offset + vs_element_size(max->type);
}

int vs_alloc(vertex_element *elems, uint32_t elems_count, uint32_t verts_count, vertex_buffer *buf)
{
	uint32_t data_length = verts_count * vs_vertex_size(elems, elems_count);
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
void vs_dealloc(vertex_buffer *buf)
{
	if (buf != NULL && buf->verts != NULL)
	{
		free(buf->verts);
	}	
}

void rs_clear(image_pixel color, image_buffer *buf)
{
	uint32_t pixel_count = buf->width * buf->height;

	for (uint32_t i = 0; i < pixel_count; ++i)
	{
		buf->pixels[i].r = color.r;
		buf->pixels[i].g = color.g;
		buf->pixels[i].b = color.b;
	}
}
void rs_draw_trianglelist(vector3_f *verts, uint32_t verts_count, image_buffer *buf)
{
	if (verts_count % 3 != 0)
	{
		printf("invalid amount of triangles for rasterizing a trianglelist");
		return;
	}

	vector2_f screen_verts[3];
	vector2_i buf_size = { buf->width - 1, buf->height - 1 };

	for (uint32_t i = 0; i < verts_count;)
	{
		screen_verts[0].x = (verts[i].x * buf_size.x + buf_size.x) * 0.5F;
		screen_verts[0].y = (verts[i].y * buf_size.y + buf_size.y) * 0.5F;
		++i;

		screen_verts[1].x = (verts[i].x * buf_size.x + buf_size.x) * 0.5F;
		screen_verts[1].y = (verts[i].y * buf_size.y + buf_size.y) * 0.5F;
		++i;

		screen_verts[2].x = (verts[i].x * buf_size.x + buf_size.x) * 0.5F;
		screen_verts[2].y = (verts[i].y * buf_size.y + buf_size.y) * 0.5F;
		++i;

		/* screen_verts[0] is the highest vert */
		if (screen_verts[1].y > screen_verts[0].y)
		{
			if (screen_verts[2].y > screen_verts[1].y)
			{
				vector2_f temp = screen_verts[2];
				screen_verts[2] = screen_verts[0];
				screen_verts[0] = temp;
			}
			else
			{
				vector2_f temp = screen_verts[1];
				screen_verts[1] = screen_verts[0];
				screen_verts[0] = temp;
			}
		}
		else if (screen_verts[2].y > screen_verts[0].y)
		{
			vector2_f temp = screen_verts[2];
			screen_verts[2] = screen_verts[0];
			screen_verts[0] = temp;
		}

		/* screen_verts[1] is the farthest right vert */
		/* screen_verts[2] is the farthest left vert */

		if (screen_verts[2].x > screen_verts[1].x)
		{
			vector2_f temp = screen_verts[2];
			screen_verts[2] = screen_verts[1];
			screen_verts[1] = temp;
		}

		/*
		*    A
		*    | \
		*    |  \
		*    C---B
		*/
		real right_inv_slope = (screen_verts[1].x - screen_verts[0].x) / (screen_verts[1].y - screen_verts[0].y);
		real left_inv_slope = (screen_verts[2].x - screen_verts[0].x) / (screen_verts[2].y - screen_verts[0].y);

		int32_t top_scanline = (int32_t)screen_verts[0].y;
		int32_t middle_scanline = (int32_t)fmaxf(screen_verts[1].y, screen_verts[2].y);
		int32_t bottom_scanline = (int32_t)fminf(screen_verts[1].y, screen_verts[2].y);
		int32_t cur_scanline = top_scanline;

		while (cur_scanline >= middle_scanline)
		{
			int32_t left_mark = (int32_t)(screen_verts[0].x + left_inv_slope * (cur_scanline - top_scanline));
			int32_t right_mark = (int32_t)(screen_verts[0].x + right_inv_slope * (cur_scanline - top_scanline));
			int32_t cur_mark = left_mark;

			image_pixel *blit = &buf->pixels[cur_scanline * buf->width + cur_mark];
			while (cur_mark <= right_mark)
			{
				blit->r = 255;
				blit->g = 255;
				blit->b = 255;
				++blit;
				++cur_mark;
			}

			cur_scanline -= 1;
		}

		if (middle_scanline == screen_verts[1].y)
		{
			right_inv_slope = (screen_verts[2].x - screen_verts[1].x) / (screen_verts[2].y - screen_verts[1].y);

			while (cur_scanline >= bottom_scanline)
			{
				int32_t left_mark = (int32_t)(screen_verts[0].x + left_inv_slope * (cur_scanline - top_scanline));
				int32_t right_mark = (int32_t)(screen_verts[1].x + right_inv_slope * (cur_scanline - middle_scanline));
				int32_t cur_mark = left_mark;

				image_pixel *blit = &buf->pixels[cur_scanline * buf->width + cur_mark];
				while (cur_mark <= right_mark)
				{
					blit->r = 255;
					blit->g = 255;
					blit->b = 255;
					++blit;
					++cur_mark;
				}

				cur_scanline -= 1;
			}
		}
		else
		{
			left_inv_slope = (screen_verts[1].x - screen_verts[2].x) / (screen_verts[1].y - screen_verts[2].y);

			while (cur_scanline >= bottom_scanline)
			{
				int32_t left_mark = (int32_t)(screen_verts[2].x + left_inv_slope * (cur_scanline - middle_scanline));
				int32_t right_mark = (int32_t)(screen_verts[0].x + right_inv_slope * (cur_scanline - top_scanline));
				int32_t cur_mark = left_mark;

				image_pixel *blit = &buf->pixels[cur_scanline * buf->width + cur_mark];
				while (cur_mark <= right_mark)
				{
					blit->r = 255;
					blit->g = 255;
					blit->b = 255;
					++blit;
					++cur_mark;
				}

				cur_scanline -= 1;
			}
		}
	}
}
