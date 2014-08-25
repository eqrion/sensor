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

	return 0;
}