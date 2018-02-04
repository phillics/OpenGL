/*Grabs pixel data from OpenGL and puts it into a bmp file...
Note: grabs data from 1920 x 1080 screen

Source:
my edited version from https://stackoverflow.com/questions/11004868/creating-a-bmp-file-bitmap-in-c
*/

#pragma once

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#define BMP_Height 1080
#define BMP_Width 1920
#define BMP_BitsPerPixel 24
#define BMP_Planes 1
#define BMP_Compression 0
#define BMP_PixelByteSize BMP_Height * BMP_Width * BMP_BitsPerPixel/8
#define BMP_FileSize BMP_PixelByteSize + sizeof(bitmap)
#define BMP_XPixelPerMeter 0x130B //2835 , 72 DPI
#define BMP_YPixelPerMeter 0x130B //2835 , 72 DPI
#define pixel 0xFF
#pragma pack(push,1)

typedef struct {
	uint8_t signature[2];
	uint32_t filesize;
	uint32_t reserved;
	uint32_t fileoffset_to_pixelarray;
} fileheader;

typedef struct {
	uint32_t dibheadersize;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitsperpixel;
	uint32_t compression;
	uint32_t imagesize;
	uint32_t ypixelpermeter;
	uint32_t xpixelpermeter;
	uint32_t numcolorspallette;
	uint32_t mostimpcolor;
} bitmapinfoheader;

typedef struct {
	fileheader fileheader;
	bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)

#endif