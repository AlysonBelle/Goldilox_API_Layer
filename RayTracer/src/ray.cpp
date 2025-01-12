#include "ray.h"

Ray::Ray()
	: origin(0.0f, 0.0f, 0.0f),
	direction(),
	tMax(RAY_T_MAX)
{
}

Ray::Ray(const Ray& r)
	: origin(r.origin),
	direction(r.direction),
	tMax(r.tMax)
{
}

Ray::Ray(const Point& origin, const Vector& direction, double tMax)
	: origin(origin),
	direction(direction),
	tMax(tMax)
{
}

Ray::~Ray()
{
}

Ray& Ray::operator =(const Ray& r)
{
	origin = r.origin;
	direction = r.direction;
	tMax = r.tMax;
	return *this;
}

Point Ray::calculate(double t) const
{
	return origin + direction * t;
}


Intersection::Intersection()
	: ray(),
	t(RAY_T_MAX),
	pShape(NULL)
{
}

Intersection::Intersection(const Intersection& i)
	: ray(i.ray),
	t(i.t),
	pShape(i.pShape),
	color(i.color),
	lightDistance(i.lightDistance)
{
}

Intersection::Intersection(const Ray& ray)
	: ray(ray),
	t(ray.tMax),
	pShape(NULL),
	color(0),
	lightDistance(0)
{
}

Intersection::~Intersection()
{
}

Intersection& Intersection::operator =(const Intersection& i)
{
	ray = i.ray;
	t = i.t;
	pShape = i.pShape;
	return *this;
}

bool Intersection::intersected() const
{
	return (pShape != NULL);
}

Point Intersection::position() const
{
	return ray.calculate(t);
}
