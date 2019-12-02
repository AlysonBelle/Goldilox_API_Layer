#include "camera.h"

#include <cmath>

PerspectiveCamera::PerspectiveCamera(
	Point origin,
	Vector target,
	Vector upguide,
	double fov,
	double aspectRatio)
	: origin(origin)
{	
	// forward =  norm ( to - from)
	forward = (target - origin).normalized();
	right = cross(forward, upguide).normalized();
	up = cross(right, forward);

	h = tan(fov / 2);
	w = h * aspectRatio;
}

Ray PerspectiveCamera::makeRay(Vector2 point) const
{
	Vector direction =
		forward + point.u * w * right + point.v * h * up;

	return Ray(origin, direction.normalized());
}

void	PerspectiveCamera::moveUp(int amount, int axis) {
	Vector	tmp;
	
	tmp = up;
	up = (up * cos(5 * (5 * axis))) - (forward * sin(5 * (5 * axis)));
	forward = (forward * cos(5 * (5 * axis))) + (tmp * sin(5 * (5 * axis)));
}

void	PerspectiveCamera::moveRight(int amount, int axis) {
	Vector	tmp;

	tmp = forward;
	forward = (forward * cos(5 * (5 * axis))) + (right * sin(5 * (5 * axis)));
	right	= (right * cos(5 * (5 * axis))) - (tmp * sin(5 * (5 * axis)));
}

Vector		findPoint(Vector start, Vector vect, double distance) {
	return Vector(start + (vect * distance));
}

void	PerspectiveCamera::moveForward(int amount, int axis) {
	origin = findPoint(origin, forward, 15 * axis);
}

