#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push,1)
/* Windows 3.x bitmap file header */
typedef struct {
    char         filetype[2];   /* magic - always 'B' 'M' */
    unsigned int filesize;
    short        reserved1;
    short        reserved2;
    unsigned int dataoffset;    /* offset in bytes to actual bitmap data */
} file_header;

/* Windows 3.x bitmap full header, including file header */
typedef struct {
    file_header  fileheader;
    unsigned int headersize;
    int          width;
    int          height;
    short        planes;
    short        bitsperpixel;  /* we only support the value 24 here */
    unsigned int compression;   /* we do not support compression */
    unsigned int bitmapsize;
    int          horizontalres;
    int          verticalres;
    unsigned int numcolors;
    unsigned int importantcolors;
} bitmap_header;
#pragma pack(pop)

void bmp_write(const char* file_name, image_buffer *image)
{
	bitmap_header bmp;
	memset(&bmp, 0, sizeof(bitmap_header));

	bmp.fileheader.filetype[0] = 'B';
	bmp.fileheader.filetype[1] = 'M';
	bmp.fileheader.filesize = sizeof(bitmap_header) +
		(3 * image->width * image->height);
	bmp.fileheader.dataoffset = sizeof(bitmap_header);
	bmp.headersize = sizeof(bitmap_header) - sizeof(file_header);
	bmp.width = image->width;
	bmp.height = image->height;
	bmp.planes = 1;
	bmp.bitsperpixel = 24;
	bmp.compression = 0;
	bmp.bitmapsize = 0;
	bmp.horizontalres = 1;
	bmp.verticalres = 1;
	bmp.numcolors = 0;
	bmp.importantcolors = 0;

	uint32_t scanline_length = image->width * 3;
	uint8_t* scanline = (uint8_t*)malloc(scanline_length);

	if (scanline == NULL)
	{
		printf("error allocating space to output bmp.");
		return;
	}

	FILE *out_file = fopen(file_name, "wb");

	if (out_file == NULL)
	{
		free(scanline);
		printf("error opening file: %s", file_name);
		return;
	}

	fwrite(&bmp, sizeof(bmp), 1, out_file);

	for (uint32_t i = 0; i < image->height; ++i)
	{
		memcpy(scanline, &image->pixels[image->width * i], scanline_length);
		for (uint32_t j = 0; j < scanline_length;)
		{
			uint8_t temp = scanline[j];
			scanline[j] = scanline[j + 2];
			scanline[j + 2] = temp;
			j += 3;
		}
		fwrite(scanline, sizeof(char), scanline_length, out_file);
	}

	free(scanline);
	fclose(out_file);
}

int_m bmp_read(const char *file_name, image_buffer *result)
{
	FILE *file = fopen(file_name, "rb");

	if (file == NULL)
	{
		fprintf(stderr, "error opening file: %s\n", file_name);
		return 0;
	}

	bitmap_header bmp;
	if (fread(&bmp, 1, sizeof(bmp), file) != sizeof(bmp))
	{
		fprintf(stderr, "%s\n", "error reading header of bitmap.");
		fclose(file);
		return 0;
	}

	if (bmp.fileheader.filetype[0] != 'B' ||
		bmp.fileheader.filetype[1] != 'M' ||
		bmp.headersize != sizeof(bitmap_header) - sizeof(file_header) ||
		bmp.planes != 1 ||
		bmp.bitsperpixel != 24 ||
		bmp.compression != 0)
	{
		fprintf(stderr, "%s\n", "error reading bitmap. invalid header.");
		fclose(file);
		return 0;
	}

	if (!img_alloc(bmp.width, bmp.height, result))
	{
		fprintf(stderr, "%s\n", "error reading bitmap, cannot allocate image.");
		fclose(file);
		return 0;
	}

	uint_m buffer_size = 3 * result->width * result->height;
	if (fread(result->pixels, 1, buffer_size, file) != buffer_size)
	{
		fprintf(stderr, "%s\n", "error reading bitmap, cannot read full buffer.");
		img_dealloc(result);
		fclose(file);
		return 0;
	}

	uint_m scanline_length = result->width * 3;

	for (uint32_t i = 0; i < result->height; ++i)
	{
		uint8_t *scanline = (uint8_t *)&result->pixels[i * result->width];
		for (uint32_t j = 0; j < scanline_length;)
		{
			uint8_t temp = scanline[j];
			scanline[j] = scanline[j + 2];
			scanline[j + 2] = temp;
			j += 3;
		}
	}

	fclose(file);
	return 1;
}