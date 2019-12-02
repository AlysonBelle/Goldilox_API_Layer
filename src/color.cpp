#include "color.h"

#include <cmath>
#include <algorithm>

Color::Color()
	: r(0.0f), g(0.0f), b(0.0f)
{
}

Color::Color(double l)
	: r(l), g(l), b(l)
{
}

Color::Color(double r, double g, double b)
	: r(r), g(g), b(b)
{
}

Color::~Color()
{
}

void Color::clamp(double min, double max)
{
	r = std::max(min, std::min(max, r));
	g = std::max(min, std::min(max, g));
	b = std::max(min, std::min(max, b));
}

void Color::applyGammaCorrection(double exposure, double gamma)
{
	r = std::pow(r * exposure, gamma);
	g = std::pow(g * exposure, gamma);
	b = std::pow(b * exposure, gamma);
}

Color& Color::operator =(const Color& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	return *this;
}

Color& Color::operator +=(const Color& c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

Color& Color::operator *=(const Color& c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

Color& Color::operator *=(double f)
{
	r *= f;
	g *= f;
	b *= f;
	return *this;
}
