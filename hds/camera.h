#ifndef CAMERA_H
#define CAMERA_H

#include "vectormath.h"
#include "ray.h"

class Camera
{
public:
	virtual ~Camera() { }

	virtual Ray makeRay(Vector2 point) const = 0;
};

class PerspectiveCamera : public Camera
{

public:
	Point origin;
	double h, w;

protected:
	Vector forward;
	Vector up;
	Vector right;


public:
	PerspectiveCamera(Point origin, Vector target,
		Vector upguide, double fov, double aspectRatio);

	void	moveUp(int amount, int axis);
	void	moveRight(int amount, int axis);
	void	moveForward(int amount, int axis);

	virtual Ray makeRay(Vector2 point) const;
};

#endif // CAMERA_H
