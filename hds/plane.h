#ifndef PLANE_H
# define PLANE_H

#include "shape.h"

class Plane : public Shape
{
private:
	double		getIntencity() { return 0; };

protected:
	Point position;
	Vector normal;
	Color color;

public:
	Plane(const Point& position, const Vector& normal,
		const Color& color = Color(1.0f, 1.0f, 1.0f));

	virtual ~Plane();

	virtual bool	intersect(Intersection& intersection);
	virtual bool	doesIntersect(const Ray& ray);
	virtual bool	doesIntersect(const Vector& point);
	virtual bool	isInfluenced(Vector min, Vector max);
	virtual E_ShapeType getShapeType() { return E_SPlane; };
	virtual Vector		getOrigin() { return position; };
	virtual Vector		getNormal( Vector point ) { return normal * -1; };

};

#endif