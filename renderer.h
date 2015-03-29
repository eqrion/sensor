#ifndef sensor_renderer_h
#define sensor_renderer_h

#include "math.h"
#include <stdint.h>

/*
 *	Image Buffer:
 *	
 * 	Image memory is laid out with 'height' scanlines of length 'width', with the
 *	pixel (0, 0) in the bottom left corner, and the pixel (width - 1, height - 1)
 *	in the upper right corner.
 */

typedef struct
{
	uint8_t r, g, b;
} image_pixel;
typedef struct
{
	uint_m width;
	uint_m height;
	image_pixel *pixels;
} image_buffer;

int_m img_alloc(uint_m width, uint_m height, image_buffer *buf);
void img_clear(image_pixel color, image_buffer *buffer);
void img_dealloc(image_buffer *buf);

image_pixel img_sample(image_buffer *buf, real x, real y);

/*
 *	Depth Stencil Buffer
 *
 * 	DS Buffer memory is laid out with 'height' scanlines of length 'width', with the
 *	pixel (0, 0) in the bottom left corner, and the pixel (width - 1, height - 1)
 *	in the upper right corner.
 */

typedef struct
{
	real depth;
} ds_pixel;
typedef struct
{
	uint_m width;
	uint_m height;
	ds_pixel *pixels;
} ds_buffer;

int_m ds_alloc(uint_m width, uint_m height, ds_buffer *buf);
void ds_clear(ds_pixel ds, ds_buffer *ds_buffer);
void ds_dealloc(ds_buffer *buf);

/*
*	Vertices
*/

enum vertex_element_type
{
	vertex_elem_position_vec3,
	vertex_elem_color_vec4,
	vertex_elem_normal_vec3,
	vertex_elem_texcoord_vec2,
};
typedef struct
{
	uint_m type;
	uint_m offset;
} vertex_element;
typedef struct
{
	void 			*verts;
	uint_m 			verts_count;
	vertex_element 	*vert_desc;
	uint_m 			vert_desc_count;
} vertex_buffer;

uint_m vs_element_size(uint_m type);
uint_m vs_vertex_size(vertex_element *elems, uint_m elems_count);

int_m vs_alloc(vertex_element *elems, uint_m elems_count, uint_m verts_count, vertex_buffer *buf);
int_m vs_buffer_element(vertex_buffer *buf, int_m element_index, void *elements, uint_m elements_count);
void vs_dealloc(vertex_buffer *buf);

/*
*	Shaders
*/

typedef void (* vertex_shader_function)(void* vertice);
typedef image_pixel (* fragment_shader_function)(void *vertice, void *state);

typedef struct
{
	vertex_shader_function function;
	vertex_element *out_desc;
	uint_m 			out_desc_count;
} vertex_shader;

typedef struct
{
	fragment_shader_function function;
	void *state;
} fragment_shader;

void rs_draw_trianglelist(vertex_buffer *vert_buffer, uint_m offset, uint_m count, fragment_shader shader, image_buffer *out_image, ds_buffer *out_ds);

#endif