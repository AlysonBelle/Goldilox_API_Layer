#include "rt.h"
#include "image.h"
#include <fstream>

Image::Image(int width, int height)
	: width(width), height(height)
{
	data = new Color[width * height];
}

Image::~Image()
{
	delete[] data;
}

int Image::getWidth() const
{
	return width;
}

int Image::getHeight() const
{
	return height;
}

Color* Image::getPixel(int x, int y)
{
	return data + (x + y * width);
}

void Image::saveImage(std::string filename,
	double exposure, double gamma) const
{
	unsigned char* imgData = new unsigned char[4 * width * height];

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Color curColor = data[x + y * width];
			curColor.applyGammaCorrection(exposure, gamma);
			curColor.clamp();

			imgData[4 * (x + y * width) + 3] = 255;
			imgData[4 * (x + y * width) + 2] =
			(unsigned char)(curColor.r * 255.0f);
			imgData[4 * (x + y * width) + 1] =
			(unsigned char)(curColor.g * 255.0f);
			imgData[4 * (x + y * width) + 0] =
			(unsigned char)(curColor.b * 255.0f);
		}
	}

//	QImage img(imgData, width, height, QImage::Format_RGB32);
//	img.save(QString(filename.c_str()));

	delete[] imgData;
}


unsigned char* Image::getImage(std::string filename,
	double exposure, double gamma) const
{
	unsigned char* imgData = new unsigned char[4 * width * height];

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Color curColor = data[x + y * width];
			curColor.applyGammaCorrection(exposure, gamma);
			curColor.clamp();

			imgData[4 * (x + y * width) + 3] = 255;
			imgData[4 * (x + y * width) + 2] = (unsigned char)(curColor.r * 255.0f);
			imgData[4 * (x + y * width) + 1] = (unsigned char)(curColor.g * 255.0f);
			imgData[4 * (x + y * width) + 0] = (unsigned char)(curColor.b * 255.0f);
		}
	}

//	QImage img(imgData, width, height, QImage::Format_RGB32);
//	img.save(QString(filename.c_str()));

	//delete[] imgData;
	return imgData;
}
