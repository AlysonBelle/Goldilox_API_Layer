#include "shape.h"

ShapeSet::ShapeSet() {
}

ShapeSet::~ShapeSet() {
}

void ShapeSet::addShape(Shape* shape) {
	shapes.push_back(shape);
}

bool ShapeSet::intersect(Intersection& intersection) {
	bool doesIntersect = false;

	for (Shape* shape : this->shapes) {
		if (shape->intersect(intersection))
			doesIntersect = true;
	}
	return doesIntersect;
}

bool ShapeSet::doesIntersect(const Ray& ray) {
	for (Shape* shape : this->shapes) {
		if (shape->doesIntersect(ray))
			return true;
	}
	return false;
}

bool ShapeSet::doesIntersect(const Vector& point) {
	for (Shape* shape : this->shapes) {
		if (shape->doesIntersect(point))
			return true;
	}
	return false;
}
