#ifndef IMAGE_H
#define IMAGE_H

#include "rt.h"
#include <SDL2/SDL.h>
#include <string>
#include "color.h"

class Image
{
protected:
	int width, height;
	Color* data;

public:
	Image(int width, int height);

	virtual ~Image();
	
	int getWidth() const;
	int getHeight() const;

	Color* getPixel(int x, int y);

	void saveImage(std::string filename,
		double exposure = 1.0f, double gamma = 2.2f) const;

	unsigned char *getImage(std::string filename,
		double exposure = 1.0f, double gamma = 2.2f) const;
};

#endif
