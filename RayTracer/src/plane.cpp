#include "plane.h"

Plane::Plane(const Point& position, const Vector& normal,
	const Color& color)
	: position(position),
	normal(normal),
	color(color) {
}

Plane::~Plane() {
}

bool Plane::intersect(Intersection& intersection) {
	// First, check if we intersect
	double dDotN = dot(intersection.ray.direction, normal);

	if (dDotN == 0.0f)
	{
		// We just assume the ray is not embedded in the plane
		return false;
	}

	// Find point of intersection
	double t = dot(position - intersection.ray.origin, normal)
		/ dDotN;

	if (t <= RAY_T_MIN || t >= intersection.t)
	{
		// Outside relevant range
		return false;
	}

	intersection.t = t;
	intersection.pShape = this;
	intersection.color = color;

	return true;
}

bool Plane::doesIntersect(const Ray& ray) {
	// First, check if we intersect
	double dDotN = dot(ray.direction, normal);

	if (dDotN == 0.0f)
	{
		// We just assume the ray is not embedded in the plane
		return false;
	}

	// Find point of intersection
	double t = dot(position - ray.origin, normal) / dDotN;

	if (t <= RAY_T_MIN || t >= ray.tMax)
	{
		// Outside relevant range
		return false;
	}

	return true;
}

bool    Plane::doesIntersect(const Vector& point) {
	// Find point of intersection
	double t = dot(position - point, normal);

	if (t <= RAY_T_MIN || t >= RAY_T_MAX) {
		// Outside relevant range
		return false;
	}

	return true;
}

bool    Plane::isInfluenced(Vector min, Vector max) {
	if (min.x <= this->position.x <= max.x) {
        return true;
    }

    if (min.y <= this->position.y <= max.y) {
        return true;
    }

    if (min.z <= this->position.z <= max.z) {
        return true;
    }

    return false;
}
