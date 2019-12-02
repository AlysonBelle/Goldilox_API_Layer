#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <cmath>

#include "vectormath.h"
#include "ray.h"

enum E_ShapeType {E_SPlane, E_SSphere, E_SPLight, E_SDLight};

class Shape
{
public:
	virtual ~Shape() { }

	virtual Vector	getOrigin() = 0;
	virtual Vector	getNormal( Vector point ) = 0;
	virtual bool	isInfluenced(Vector min, Vector max) = 0;
	virtual E_ShapeType getShapeType() = 0;

	virtual bool	intersect(Intersection& intersection) = 0;
	virtual bool	doesIntersect(const Ray& ray) = 0;
	virtual bool	doesIntersect(const Vector& point) = 0;
	virtual double	getIntencity() = 0;
};

class ShapeSet
{
protected:

public:
	ShapeSet();
	~ShapeSet();

	void addShape(Shape* shape);

	bool intersect(Intersection& intersection);
	bool doesIntersect(const Ray& ray);
	bool doesIntersect(const Vector& point);

	std::vector<Shape *>	shapes;
};

#include "sphere.h"
#include "plane.h"

#endif // SHAPE_H
