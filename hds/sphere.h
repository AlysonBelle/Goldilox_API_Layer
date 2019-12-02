#ifndef SPHERE_H
# define SPHERE_H

# include "shape.h"

class Sphere : public Shape {
private:
	double		getIntencity() { return radius; };

protected:
	Sphere();
	Point centre;
	double radius;
	Color color;

public:
	Sphere(const Point& centre, double radius,
		const Color& color = Color(1.0f, 1.0f, 1.0f));

	virtual ~Sphere();

	virtual bool intersect(Intersection& intersection);
	virtual bool doesIntersect(const Ray& ray);
	virtual bool doesIntersect(const Vector& point);
	virtual bool isInfluenced(Vector min, Vector max);
	virtual E_ShapeType getShapeType() { return E_SSphere; };
	virtual Vector		getOrigin() { return centre; };
	virtual Vector		getNormal( Vector point ) { return Vector( point - centre ); };
};

#endif