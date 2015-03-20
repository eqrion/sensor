#include "math.h"
#include "renderer.h"
#include "bmp.h"

#include <stdio.h>

image_pixel basic_frag_shader(void *frag_vertex)
{
	vector4_f color = *(vector4_f *)((uint8_t *)frag_vertex + sizeof(vector3_f));
	image_pixel result = {
		.r = (uint8_t)(color.x * 255),
		.g = (uint8_t)(color.y * 255),
		.b = (uint8_t)(color.z * 255),
	};
	return result;
}

int main(int argc, char** argcv)
{
	image_buffer buf;

	if (!img_alloc(1000, 1000, &buf))
	{
		fprintf(stderr, "cannot allocate memory for an image.\n");
		return 1;
	}

	vertex_element vert_desc[] = { { .type = vertex_elem_position_vec3, .offset = 0}, { .type = vertex_elem_color_vec4, .offset = sizeof(vector3_f) } };
	vertex_buffer vert_buffer = { 0 };

	fragment_shader fs = { .function = basic_frag_shader };

	if (!vs_alloc(vert_desc, 2, 3, &vert_buffer))
	{
		fprintf(stderr, "%s\n", "cannot allocate a vertex_buffer");
		return 1;
	}

	vector3_f triangle_positions[3] = { {0.0F,0.4330127019F,0.0F}, {-0.5F,-0.4330127019F,0.0F},{0.5F,-0.4330127019F,0.0F} };
	vector4_f triangle_colors[3] = { {1.0F,0.0F,0.0F,0.0F}, {0.0F,1.0F,0.0F,0.0F}, {0.0F,0.0F,1.0F,0.0F} };

	vs_buffer_element(&vert_buffer, 0, triangle_positions, 3);
	vs_buffer_element(&vert_buffer, 1, triangle_colors, 3);

	mat4x4 transform = mat4x4_rot_z(M_PI_OVER_8 / 2);

	for (int i = 0; i < 32; ++i)
	{
		image_pixel clear = {0,255,0};
		
		rs_clear(clear, &buf);
		rs_draw_trianglelist(&vert_buffer, 0, 3, fs, &buf);

		char filename[20];
		sprintf(filename, "out%d.bmp", i);
	    bmp_write_framebuffer(filename, &buf);

	    mat4x4_transform_stream(&transform, triangle_positions, 3);

		vs_buffer_element(&vert_buffer, 0, triangle_positions, 3);
		vs_buffer_element(&vert_buffer, 1, triangle_colors, 3);
	}

	return 0;
}