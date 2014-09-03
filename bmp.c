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

void bmp_write_framebuffer(const char* file_name, image_buffer *image)
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
	bmp.horizontalres = 100;
	bmp.verticalres = 100;
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
		memcpy(scanline, image->pixels + image->width * i, scanline_length);
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