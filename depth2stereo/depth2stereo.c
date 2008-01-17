/*******************************************************************************
depth2stereo reads a depth map and creates two random dot stereogram files
Copyright (C) 2008 Gerhard Roethlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************************/

#include <stdlib.h>
#include <memory.h>
#include <libgen.h>
#include <tga.h>

static char * d2sErrorStrings [] =
{
	"OK\n",
	"Error allocating output name string buffer\n",
	"Error finding base name\n",
	"Error opening %s\n",
	"Error writing %s\n",
	"Error reading %s\n",
	"Error reading: Invalid bit depth\n"
};

typedef struct
{
	unsigned short width;
	unsigned short height;
	unsigned char * data;
}
Image;

Image * image_create(unsigned short w, unsigned short h)
{
	Image * t = (Image *)malloc(sizeof(Image));
	t->width = w;
	t->height = h;
	t->data = (unsigned char*)malloc(sizeof(unsigned char)*w*h);
	return t;
}

void image_destroy(Image * img)
{
	free(img->data);
	free(img);
}

inline int image_setPixel(Image * img,
	unsigned short x, unsigned short y, unsigned char pix)
{
	if (!img) return -1;
	
	if (x < img->width)
	{
		if (y < img->height)
		{
			img->data[img->width*y + x] = pix;
			return 0;
		}
	}
	return -2;
}

inline unsigned char image_getPixel(Image * img,
	unsigned short x, unsigned short y)
{
	if (!img) return 0; // Error
	
	if (x < img->width)
	{
		if (y < img->height)
		{
			return img->data[img->width*y + x];
		}
	}
	return 0; // Error
}

int image_copy(Image * a, Image * b)
{
	if (!a) return -1;
	if (!b) return -1;
	
	if (a->width == b->width && a->height == b->height)
	{
		unsigned int size = a->width*a->height;
		memcpy(b->data, a->data, size);
		return 0;
	}
	return -2;
}

void copy_header(TGA *src, TGA *dest)
{
        dest->hdr.id_len        = 0;//src->hdr.id_len;
        dest->hdr.map_t         = 0;//src->hdr.map_t;
        dest->hdr.img_t         = 3;//src->hdr.img_t; // 2 = rgb, 3 = b&w
        dest->hdr.map_first     = 0;//src->hdr.map_first;
        dest->hdr.map_entry     = 0;//src->hdr.map_entry;
        dest->hdr.map_len       = 0;//src->hdr.map_len;
        dest->hdr.x             = src->hdr.x;
        dest->hdr.y             = src->hdr.y;
        dest->hdr.width         = src->hdr.width;
        dest->hdr.height        = src->hdr.height;
        dest->hdr.depth         = 8;//src->hdr.depth;
        dest->hdr.vert          = src->hdr.vert;
        dest->hdr.horz          = src->hdr.horz;
        dest->hdr.alpha         = 0;//src->hdr.alpha;
}

int write_tga(char * name, TGA * template, Image * img)
{
	TGA *	outTGA;
	TGAData	outData;

	outTGA = TGAOpen(name, "w");
	if (!outTGA || outTGA->last != TGA_OK)
	{
		printf(d2sErrorStrings[3], name);
		return -1;
	}
	
	copy_header(template, outTGA);
	
	outData.flags = TGA_IMAGE_DATA;
	outData.img_data = img->data;
	outData.img_id = 0;
	outData.cmap = 0;
	
	if (TGAWriteImage(outTGA, &outData) != TGA_OK)
	{
		printf(d2sErrorStrings[4], "left.tga");
		return -1;
	}
	
	TGAClose(outTGA);
	return 0;
}

void stereogram(Image * in, Image * l, Image * r)
{
	unsigned short i,j;
	for (j = 0; j < in->height; j++)
	{
		for (i = 0; i < in->width; i++)
		{
			int k = i-(image_getPixel(in, i, j) >> 6);
			if (k < 0 || k >= l->width)
			{
				image_setPixel(r, i, j, random()%2*0xff);
			}
			else
			{
				image_setPixel(r, i, j, image_getPixel(l, k, j));
			}

		}
	}

}

int main (int argc, char** argv)
{
	TGA *	inTGA;
	TGAData	inData;
	
	// 1. Load Image
	if (argc != 2)
	{
		printf("Usage: %s <in.tga>\n", argv[0]);
		return -1;
	}
	
	inTGA = TGAOpen(argv[1], "r");
	if (!inTGA || inTGA->last != TGA_OK)
	{
		printf(d2sErrorStrings[3], argv[1]);
		return -1;
	}
	
	inData.flags = TGA_IMAGE_DATA | TGA_IMAGE_ID;
	if (TGAReadImage(inTGA, &inData) != TGA_OK)
	{
		printf(d2sErrorStrings[5], argv[1]);
		return -1;
	}

	if (inTGA->hdr.depth != 8 ||
		inTGA->hdr.alpha != 0 ||
		inTGA->hdr.img_t != 3)
	{
		printf(d2sErrorStrings[6]);
		printf("Depth: %i\n", inTGA->hdr.depth);
		printf("Alpha: %i\n", inTGA->hdr.alpha);
		printf("Type: %i\n", inTGA->hdr.img_t);
		return -1;
	}	
		
	Image * inImage = (Image*)malloc(sizeof(Image));
	inImage->width = inTGA->hdr.width;
	inImage->height = inTGA->hdr.height;
	inImage->data = inData.img_data;
	
	// 2. Create Random Image
	Image * l = image_create(inImage->width, inImage->height);
	
	unsigned short i,j;
	for (j = 0; j < l->height; j++)
	{
		for (i = 0; i < l->height; i++)
		{
			image_setPixel(l, i, j, random()%2*0xff);
		}
	}
	
	// 3. Create second Image
	Image * r = image_create(inImage->width, inImage->height);
	
	// 4. Create stereogram
	stereogram(inImage, l, r);
	
	// 5. Write both images
	char * b = basename(argv[1]);
	if (!b)
	{
		printf(d2sErrorStrings[2]);
		return -1;
	}
	char * leftName;
	char * rightName;
	if (asprintf(&leftName, "%s.left.tga", b) == -1)
	{
		printf(d2sErrorStrings[1]);
		return -1;
	}
	if (asprintf(&rightName, "%s.right.tga", b) == -1)
	{
		printf(d2sErrorStrings[1]);
		return -1;
	}
	write_tga(leftName, inTGA, l);
	write_tga(rightName, inTGA, r);
	free(leftName);
	free(rightName);

	TGAClose(inTGA);	
	
	image_destroy(l);
	image_destroy(r);
	image_destroy(inImage);
	
	return 0;
}