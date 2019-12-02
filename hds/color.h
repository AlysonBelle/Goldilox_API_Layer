#ifndef COLOR_H
#define COLOR_H

struct Color
{
	double r, g, b;

	Color();
	Color(double l);
	Color(double r, double g, double b);

	virtual ~Color();

	void clamp(double min = 0.0f, double max = 1.0f);
	void applyGammaCorrection(double exposure, double gamma);

	Color& operator =(const Color& c);
	Color& operator +=(const Color& c);
	Color& operator *=(const Color& c);
	Color& operator *=(double f);
};

inline Color operator +(const Color& c1, const Color& c2)
{
	return Color(c1.r + c2.r,
		c1.g + c2.g,
		c1.b + c2.b);
}

inline Color operator *(const Color& c1, const Color& c2)
{
	return Color(c1.r * c2.r,
		c1.g * c2.g,
		c1.b * c2.b);
}

inline Color operator *(const Color& c, double f)
{
	return Color(c.r * f,
		c.g * f,
		c.b * f);
}

inline Color operator *(double f, const Color& c)
{
	return Color(f * c.r,
		f * c.g,
		f * c.b);
}

#endif // COLOR_H
