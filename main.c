#include "math.h"
#include "renderer.h"
#include "bmp.h"

int main(int argc, char** argcv)
{
	image_buffer buf;
	if (!img_alloc(200, 200, &buf))
	{
		return 1;
	}

	image_pixel color = { 0, 0, 0 };
	rs_clear(color, &buf);

	vector3_f tris[3];

	tris[0].x = 1;
	tris[0].y = -1;
	tris[0].z = 0;

	tris[1].x = 1;
	tris[1].y = 1;
	tris[1].z = 0;

	tris[2].x = -1;
	tris[2].y = -1;
	tris[2].z = 0;

	rs_draw_trianglelist(tris, 3, &buf);

	bmp_write_framebuffer("out.bmp", &buf);

	img_dealloc(&buf);

	vector3_f a = {5, 5, 5};
	mat4x4 t1 = mat4x4_translation(a);
	vector3_f b = {15, 25, 35};
	mat4x4 t2 = mat4x4_translation(b);
	vector3_f c = {2, 2, 2};
	mat4x4 t3 = mat4x4_scaling(c);

	mat4x4 t4 = mat4x4_multiply(&t3, &t2);
	mat4x4 t5 = mat4x4_multiply(&t4, &t1);

	mat4x4_print(&t5);

	return 0;
}