#include "GLutilities.h"
#include "screenshot.h"

int screenshotBMP(char *fileName) {
	FILE *fp = fopen(fileName, "wb");
	if (fp == NULL) {
		gl_log_error("BMP File could not be opened: ", fileName);
		return -1;
	}

	bitmap *pbitmap = (bitmap*)calloc(1, sizeof(bitmap));

	strcpy(pbitmap->fileheader.signature, "BM");

	pbitmap->fileheader.filesize = BMP_FileSize;
	pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
	pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader);
	pbitmap->bitmapinfoheader.width = BMP_Width;
	pbitmap->bitmapinfoheader.height = BMP_Height;
	pbitmap->bitmapinfoheader.planes = BMP_Planes;
	pbitmap->bitmapinfoheader.bitsperpixel = BMP_BitsPerPixel;
	pbitmap->bitmapinfoheader.compression = BMP_Compression;
	pbitmap->bitmapinfoheader.imagesize = BMP_PixelByteSize;
	pbitmap->bitmapinfoheader.ypixelpermeter = BMP_YPixelPerMeter;
	pbitmap->bitmapinfoheader.xpixelpermeter = BMP_XPixelPerMeter;
	pbitmap->bitmapinfoheader.numcolorspallette = 0;

	//write header to file
	fwrite(pbitmap, 1, sizeof(bitmap), fp);

	//write color to pixel buffer
	
	GLubyte* pixelbuffer = (GLubyte*)malloc(BMP_PixelByteSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glReadPixels(0,0,BMP_Width, BMP_Height, GL_RGB, GL_UNSIGNED_BYTE, pixelbuffer);
	

	//this writes entire buffer to color of pixel...
	//memset(pixelbuffer, pixel, BMP_PixelByteSize);

	//write pixelbuffer to file
	fwrite(pixelbuffer, 1, BMP_PixelByteSize, fp);

	fclose(fp);
	free(pbitmap);
	free(pixelbuffer);

	return 1;
}