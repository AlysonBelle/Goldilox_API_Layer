#include "sphere.h"

Sphere::Sphere() {
	
}

Sphere::Sphere(const Point& centre, double radius,
	const Color& color)
	: centre(centre),
	radius(radius),
	color(color) {
}

Sphere::~Sphere() {
}

bool Sphere::intersect(Intersection& intersection) {
	// Transform ray so we can consider origin-centred sphere
	Ray localRay = intersection.ray;
	localRay.origin -= centre;

	// Calculate quadratic coefficients
	double a = localRay.direction.length2();
	double b = 2 * dot(localRay.direction, localRay.origin);
	double c = localRay.origin.length2() - sqr(radius);

	// Check whether we intersect
	double discriminant = sqr(b) - 4 * a * c;

	if (discriminant < 0.0f)
	{
		return false;
	}

	// Find two points of intersection, t1 close and t2 far
	double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
	double t2 = (-b + std::sqrt(discriminant)) / (2 * a);

	// First check if close intersection is valid
	if (t1 > RAY_T_MIN && t1 < intersection.t)
	{
		intersection.t = t1;
	}
	else if (t2 > RAY_T_MIN && t2 < intersection.t)
	{
		intersection.t = t2;
	}
	else
	{
		// Neither is valid
		return false;
	}

	// Finish populating intersection
	intersection.pShape = this;
	intersection.color = color;

	return true;
}

bool Sphere::doesIntersect(const Ray& ray) {
	// Transform ray so we can consider origin-centred sphere
	Ray localRay = ray;
	localRay.origin -= centre;

	// Calculate quadratic coefficients
	double a = localRay.direction.length2();
	double b = 2 * dot(localRay.direction, localRay.origin);
	double c = localRay.origin.length2() - sqr(radius);

	// Check whether we intersect
	double discriminant = sqr(b) - 4 * a * c;

	if (discriminant < 0.0f)
	{
		return false;
	}

	// Find two points of intersection, t1 close and t2 far
	double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
	if (t1 > RAY_T_MIN && t1 < ray.tMax)
		return true;

	double t2 = (-b + std::sqrt(discriminant)) / (2 * a);
	if (t2 > RAY_T_MIN && t2 < ray.tMax)
		return true;

	return false;
}

bool Sphere::doesIntersect(const Vector& point) {
	// Transform ray so we can consider origin-centred sphere
	Ray localRay;
	localRay.origin = point - centre;

	// Calculate quadratic coefficients
	double a = localRay.direction.length2();
	double b = 2 * dot(localRay.direction, localRay.origin);
	double c = localRay.origin.length2() - sqr(radius);

	// Check whether we intersect
	double discriminant = sqr(b) - 4 * a * c;

	if (discriminant < 0.0f)
	{
		return false;
	}

	// Find two points of intersection, t1 close and t2 far
	double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
	if (t1 > RAY_T_MIN && t1 < RAY_T_MAX)
		return true;

	double t2 = (-b + std::sqrt(discriminant)) / (2 * a);
	if (t2 > RAY_T_MIN && t2 < RAY_T_MAX)
		return true;

	return false;
}

bool  Sphere::isInfluenced(Vector min, Vector max) {
	int x, y, z = 0;

	x = std::max(min.x, std::min(centre.x, max.x));
	y = std::max(min.y, std::min(centre.y, max.y));
	z = std::max(min.z, std::min(centre.z, max.z));


	double distance = std::sqrt((x - centre.x) * (x - centre.x) +
    	(y - centre.y) * (y - centre.y) +
    	(z - centre.z) * (z - centre.z));

	 return distance < radius;
}
