#ifndef LIGHT_H
# define LIGHT_H

# include "sphere.h"

class Light : public Sphere {
protected:
	E_ShapeType	type;
	float		intensity;

public:
	Light(const Point& centre, double intensity, E_ShapeType type);
	~Light();
	E_ShapeType getShapeType() { return type; };
	double		getIntencity() { return intensity; };
};

#endif