#ifndef sensor_renderer_h
#define sensor_renderer_h

#include "math.h"

#include <stdint.h>

/*
*	Image Buffer
*/

typedef struct
{
	uint8_t r, g, b;
} image_pixel;
typedef struct
{
	uint32_t width;
	uint32_t height;
	image_pixel *pixels;
} image_buffer;

int img_alloc(uint32_t width, uint32_t height, image_buffer *buf);
void img_dealloc(image_buffer *buf);

/*
*	Depth Stencil Buffer
*/

typedef struct
{
	real depth;
} ds_pixel;
typedef struct
{
	uint32_t width;
	uint32_t height;
	ds_pixel *pixels;
} ds_buffer;

int ds_alloc(uint32_t width, uint32_t height, ds_buffer *buf);
void ds_dealloc(ds_buffer *buf);

/*
*	Vertices
*/

enum vertex_element_type
{
	vertex_elem_position_vec3,
	vertex_elem_color_vec4,
	vertex_elem_normal_vec3
};
typedef struct
{
	uint32_t type;
	uint32_t offset;
} vertex_element;
typedef struct
{
	void *verts;
	uint32_t verts_count;
	vertex_element *vert_desc;
	uint32_t vert_desc_count;
} vertex_buffer;

uint32_t vs_element_size(uint32_t type);
uint32_t vs_vertex_size(vertex_element *elems, uint32_t elems_count);

int vs_alloc(vertex_element *elems, uint32_t elems_count, uint32_t verts_count, vertex_buffer *buf);
void vs_dealloc(vertex_buffer *buf);

/*
*	Shaders
*/

typedef void (* vertex_shader_function)(void* vertice);
typedef image_pixel (* fragment_shader_function)(void* vertice);

typedef struct
{
	vertex_shader_function function;
	vertex_element *out_desc;
	uint32_t 		out_desc_count;
} vertex_shader;

typedef struct
{
	fragment_shader_function function;
} fragment_shader;

void rs_clear(image_pixel color, image_buffer *buffer);
void rs_draw_trianglelist(vector3_f *verts, uint32_t verts_count, image_buffer *buf);

#endif