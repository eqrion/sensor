#ifndef sensor_bmp_h
#define sensor_bmp_h

#include "renderer.h"

/*
 * outputs the image buffer to a bitmap file. very simple code, only supports
 * square image_buffers that are at least 100x100 pixels wide.
 */
void bmp_write(const char* file_out, image_buffer *image);

/*
 * reads a bitmap file into an image buffer. very simple code, probably doesn't
 * read every type of bitmap file possible.
 */
int_m bmp_read(const char *file_name, image_buffer *result);

#endif