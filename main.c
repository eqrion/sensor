#include "math.h"
#include "renderer.h"
#include "bmp.h"

#include <stdio.h>
#include <string.h>

image_pixel basic_frag_shader1(void *frag_vertex, void *state)
{
	vector4_f color = *(vector4_f *)((uint8_t *)frag_vertex + sizeof(vector3_f));

	image_pixel result = {
		.r = (uint8_t)(color.x * 255),
		.g = (uint8_t)(color.y * 255),
		.b = (uint8_t)(color.z * 255),
	};
	return result;

	/*image_pixel result = {
		.r = 100,
		.g = 100,
		.b = 100,
	};
	return result;*/
}
image_pixel basic_frag_shader2(void *frag_vertex, void *state)
{
	vector2_f uv = *(vector2_f *)((uint8_t *)frag_vertex + sizeof(vector3_f));

	return img_sample((image_buffer *)state, uv.x, uv.y);
}

int render_cube()
{
	/* allocate buffers for rendering */

	image_buffer color_buffer;
	ds_buffer ds_buffer;

	if (!img_alloc(1000, 1000, &color_buffer))
	{
		fprintf(stderr, "%s\n", "cannot allocate memory for an image.");
		return 1;
	}
	if (!ds_alloc(1000, 1000, &ds_buffer))
	{
		fprintf(stderr, "%s\n", "cannot allocate memory for a depth buffer.");
		img_dealloc(&color_buffer);
		return 1;
	}
	
	/* allocate a vertex buffer for storing the cube */

	vertex_element vert_desc[] = {
		{ .type = vertex_elem_position_vec3, .offset = 0},
		{ .type = vertex_elem_color_vec4, .offset = sizeof(vector3_f) }
	};
	vertex_buffer vert_buffer = { 0 };

	if (!vs_alloc(vert_desc, 2, 36, &vert_buffer))
	{
		fprintf(stderr, "%s\n", "cannot allocate a vertex_buffer");
		img_dealloc(&color_buffer);
		ds_dealloc(&ds_buffer);
		return 1;
	}

	vector3_f original_cube_positions[36] =
	{
		{1.0F,  -1.0F,  1.0F}, {-1.0F,  -1.0F,  1.0F}, {-1.0F,  -1.0F,  -1.0F},
		{-1.0F,  1.0F,  -1.0F}, {-1.0F,  1.0F,  1.0F}, {1.0F,  1.0F,  1.0F},
		{1.0F,  -1.0F,  -1.0F}, {1.0F,  1.0F,  -1.0F}, {1.0F,  1.0F,  1.0F},
		{1.0F,  -1.0F,  1.0F}, {1.0F,  1.0F,  1.0F}, {-1.0F,  1.0F,  1.0F},
		{-1.0F,  1.0F,  1.0F}, {-1.0F,  1.0F,  -1.0F}, {-1.0F,  -1.0F,  -1.0F},
		{1.0F,  -1.0F,  -1.0F}, {-1.0F,  -1.0F,  -1.0F}, {-1.0F,  1.0F,  -1.0F},
		{1.0F,  -1.0F,  -1.0F}, {1.0F,  -1.0F,  1.0F}, {-1.0F,  -1.0F,  -1.0F},
		{1.0F,  1.0F,  -1.0F}, {-1.0F,  1.0F,  -1.0F}, {1.0F,  1.0F,  1.0F},
		{1.0F,  -1.0F,  1.0F}, {1.0F,  -1.0F,  -1.0F}, {1.0F,  1.0F,  1.0F},
		{-1.0F,  -1.0F,  1.0F}, {1.0F,  -1.0F,  1.0F}, {-1.0F,  1.0F,  1.0F},
		{-1.0F,  -1.0F,  1.0F}, {-1.0F,  1.0F,  1.0F}, {-1.0F,  -1.0F,  -1.0F},
		{1.0F,  1.0F,  -1.0F}, {1.0F,  -1.0F,  -1.0F}, {-1.0F,  1.0F,  -1.0F}
	};
	vector4_f original_cube_colors[36] =
	{
		{1.0F,  1.0F,  1.0F, 0.0F}, {1.0F,  1.0F,  1.0F, 0.0F}, {1.0F,  1.0F,  1.0F, 0.0F},
		{1.0F,  1.0F,  0.0F, 0.0F}, {1.0F,  1.0F,  0.0F, 0.0F}, {1.0F,  1.0F,  0.0F, 0.0F},
		{1.0F,  0.0F,  1.0F, 0.0F}, {1.0F,  0.0F,  1.0F, 0.0F}, {1.0F,  0.0F,  1.0F, 0.0F},
		{0.0F,  1.0F,  1.0F, 0.0F}, {0.0F,  1.0F,  1.0F, 0.0F}, {0.0F,  1.0F,  1.0F, 0.0F},
		{1.0F,  0.0F,  0.0F, 0.0F}, {1.0F,  0.0F,  0.0F, 0.0F}, {1.0F,  0.0F,  0.0F, 0.0F},
		{0.0F,  0.0F,  1.0F, 0.0F}, {0.0F,  0.0F,  1.0F, 0.0F}, {0.0F,  0.0F,  1.0F, 0.0F},
		{0.0F,  1.0F,  0.0F, 0.0F}, {0.0F,  1.0F,  0.0F, 0.0F}, {0.0F,  1.0F,  0.0F, 0.0F},
		{0.0F,  0.0F,  0.0F, 0.0F}, {0.0F,  0.0F,  0.0F, 0.0F}, {0.0F,  0.0F,  0.0F, 0.0F},
		{0.0F,  0.0F,  0.5F, 0.0F}, {0.0F,  0.0F,  0.5F, 0.0F}, {0.0F,  0.0F,  0.5F, 0.0F},
		{0.0F,  0.5F,  0.0F, 0.0F}, {0.0F,  0.5F,  0.0F, 0.0F}, {0.0F,  0.5F,  0.0F, 0.0F},
		{0.5F,  0.0F,  0.0F, 0.0F}, {0.5F,  0.0F,  0.0F, 0.0F}, {0.5F,  0.0F,  0.0F, 0.0F},
		{0.5F,  0.0F,  0.5F, 0.0F}, {0.5F,  0.0F,  0.5F, 0.0F}, {0.5F,  0.0F,  0.5F, 0.0F},
	};
	vector3_f cube_positions[36];

	for (int i = 0; i < 32; ++i)
	{
		/* transform and output the vertices into the buffer */

		memcpy(cube_positions, original_cube_positions, sizeof(vector3_f) * 36);

		vector3_f new_center = { 0, 0, -5 };
		mat4x4 translation = mat4x4_translation(new_center);
		mat4x4 rotation1 = mat4x4_rot_y(M_PI_OVER_8 / 2 * i);
		mat4x4 rotation2 = mat4x4_identity();
		mat4x4 world = mat4x4_multiply(&rotation2, &rotation1);
		world = mat4x4_multiply(&translation, &world);

		mat4x4 projection = mat4x4_projection_fov_rh(-.01F, .01F, .01F, -.01F, 0.01F, 10.0F);

		mat4x4 wvp = mat4x4_multiply(&projection, &world);

		mat4x4_transform_stream(&wvp, cube_positions, 36);

		vs_buffer_element(&vert_buffer, 0, cube_positions, 36);
		vs_buffer_element(&vert_buffer, 1, original_cube_colors, 36);

		/* render the cube into the buffers */

		fragment_shader fs = { .function = basic_frag_shader1, .state = NULL };

		image_pixel clear = {0,255,0};
		ds_pixel depth_zero = {0};
		img_clear(clear, &color_buffer);
		ds_clear(depth_zero, &ds_buffer);
		rs_draw_trianglelist(&vert_buffer, 0, 36, fs, &color_buffer, &ds_buffer);

		/* output the buffers to a bmp */

		char filename[20];
		sprintf(filename, "cube-%d.bmp", i);
	    bmp_write(filename, &color_buffer);
	}

	img_dealloc(&color_buffer);
	ds_dealloc(&ds_buffer);
	vs_dealloc(&vert_buffer);

	return 0;
}

int render_triangle(vector3_f *tri_positions, const char *filename)
{
	/* allocate buffers for rendering */

	image_buffer color_buffer;
	ds_buffer ds_buffer;

	if (!img_alloc(500, 500, &color_buffer))
	{
		fprintf(stderr, "%s\n", "cannot allocate memory for an image.");
		return 1;
	}
	if (!ds_alloc(500, 500, &ds_buffer))
	{
		fprintf(stderr, "%s\n", "cannot allocate memory for a depth buffer.");
		img_dealloc(&color_buffer);
		return 1;
	}
	
	/* read a texture for rendering */

	image_buffer texture;

	if (!bmp_read("texture.bmp", &texture))
	{
		fprintf(stderr, "%s\n", "cannot read texture from texture.bmp");
		img_dealloc(&color_buffer);
		ds_dealloc(&ds_buffer);
		return 1;
	}

	/* allocate a vertex buffer for storing the triangle */

	vertex_element vert_desc[] = {
		{ .type = vertex_elem_position_vec3, .offset = 0},
		{ .type = vertex_elem_texcoord_vec2, .offset = sizeof(vector3_f) }
	};
	vertex_buffer vert_buffer = { 0 };

	if (!vs_alloc(vert_desc, 2, 3, &vert_buffer))
	{
		fprintf(stderr, "%s\n", "cannot allocate a vertex_buffer");
		img_dealloc(&color_buffer);
		ds_dealloc(&ds_buffer);
		img_dealloc(&texture);
		return 1;
	}

	vector2_f tri_texcoords[3] = { { 0.0F, 0.0F }, { 1.0F, 0.0F }, { 0.0F, 1.0F } };

	vs_buffer_element(&vert_buffer, 0, tri_positions, 3);
	vs_buffer_element(&vert_buffer, 1, tri_texcoords, 3);	

	/* render the triangles */

	fragment_shader fs = { .function = basic_frag_shader2, .state = &texture };
	image_pixel clear = {0,255,0};
	ds_pixel depth_zero = {0};

	img_clear(clear, &color_buffer);
	ds_clear(depth_zero, &ds_buffer);
	rs_draw_trianglelist(&vert_buffer, 0, 3, fs, &color_buffer, &ds_buffer);

    bmp_write(filename, &color_buffer);

	img_dealloc(&color_buffer);
	ds_dealloc(&ds_buffer);
	img_dealloc(&texture);
	vs_dealloc(&vert_buffer);

	return 0;
}

int main(int argc, char** argcv)
{
	return render_cube();

	/*vector3_f coordinates[3] = {
		{ 0.203238428, -0.172840074, 1.00546229},
		{ -0.127220005, -0.161920354, 1.00524366},
		{ 0.205457479, -0.261497766, 1.00723708}
    };
	return render_triangle(coordinates, "triangle.bmp");*/
}