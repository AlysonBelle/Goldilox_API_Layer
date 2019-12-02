#include "octivation.h"
#include <algorithm>

void	OctTree::octavateWorld() {
	std::cout << "Bounding World\n";
	getWorldBoundingBox();
	std::cout << "Adding shapes to world\n";
	initShapesAndLights(this, a.scene->shapes);
	std::cout << "Octavating\n";
	octavate(this);
	std::cout << "Octation Completed\n";
}

void	OctTree::initShapesAndLights(OctTree *o, std::vector<Shape *> things) {
	for (Shape *thing : things) {
		if (thing->isInfluenced(o->min, o->max)) {
			if ((thing->getShapeType() == E_SPLight) || (thing->getShapeType() == E_SDLight))
				o->lights.push_back(thing);
			o->shapes.push_back(thing);
			continue ;
		}
	}
}

void	OctTree::shapeShifter(OctTree *o, std::vector<Shape *> shapes) {
	for (Shape *shape : shapes) {
		if (shape->isInfluenced(o->min, o->max)) {
			o->shapes.push_back(shape);
			continue ;
		}
	}
}

void	OctTree::lightShifter(OctTree *o, std::vector<Shape *> lights) {
		Vector	origin;

	for (Shape *light : lights) {
		if (light->isInfluenced(o->min, o->max)) {
			o->lights.push_back(light);
			continue ;
		}
	}
}

OctTree	*OctTree::createOcta(OctTree *octa, int x, int y, int z) {
	Vector min = octa->min;
	Vector max = octa->max;

	if (x == 0) {
		min.x = octa->min.x;
		max.x = (octa->min.x + octa->max.x) / 2;
	} else {
		min.x = (octa->min.x + octa->max.x) / 2;
		max.x = octa->max.x;
	}

	if (y == 0) {
		min.y = octa->min.y;
		max.y = (octa->min.y + octa->max.y) / 2;
	} else {
		min.y = (octa->min.y + octa->max.y) / 2;
		max.y = octa->max.y;
	}

	if (z == 0) {
		min.z = octa->min.z;
		max.z = (octa->min.z + octa->max.z) / 2;
	} else {
		min.z = (octa->min.z + octa->max.z) / 2;
		max.z = octa->max.z;
	}
	
	OctTree	*o = new OctTree(&a);
	o->id = octa->id + 1;
	o->min = min;
	o->max = max;
	shapeShifter(o, octa->shapes);
	lightShifter(o, octa->lights);
	if (o->shapes.size() + o->lights.size() > OCTA_MAX_SIZE ) {
		octavate(o);
	}
	return o;
}

void	OctTree::octavate(OctTree *octa) {
	if (octa->id > 5) {
		return ;
	}
	for (int z = 0 ; z < 2 ; z++) {
		for (int y = 0 ; y < 2 ; y++) {
			for (int x = 0 ; x < 2 ; x++) {
				octa->octas.push_back(createOcta(octa, x, y, z));
			}
		}
	}
}

void	OctTree::getWorldBoundingBox() {
	Vector	min;
	Vector	max;
	Vector	origin;

	max = {OCTA_STARTING_MAX_BOX_LOCATION};
	min = {OCTA_STARTING_MIN_BOX_LOCATION};

	for (Shape *shape : a.scene->shapes) {
		origin = shape->getOrigin();
		if (origin.x < min.x) {
			min.x = origin.x;
		}
		if (origin.x > max.x) {
			max.x = origin.x;
		}
		if (origin.y < min.y) {
			min.y = origin.y;
		}
		if (origin.y > max.y) {
			max.y = origin.y;
		}
		if (origin.z < min.z) {
			min.z = origin.z;
		}
		if (origin.z > max.z) {
			max.z = origin.z;
		}
	}
	this->min = min;
	this->max = max;
}

bool 	OctTree::doesIntersectShapes(const Ray & r) {
	bool	doesIntersect = false;

	for (Shape* shape : this->shapes) {
		if ((shape->getShapeType() != E_SPLight) && (shape->getShapeType() != E_SDLight))
			if (shape->doesIntersect(r))
				doesIntersect = true;
	}
	return doesIntersect;
}

bool 	OctTree::doesIntersectLights(const Ray & r) {
	bool	doesIntersect = false;

	for (Shape* light : this->lights) {
		if (light->doesIntersect(r))
			doesIntersect = true;
	}
	return doesIntersect;
}

bool	OctTree::intersectShapes(Intersection& intersection) {
	bool doesIntersect = false;

	if (this->doesIntersect(intersection.ray)) {
		for (Shape* shape : this->shapes) {
			if (shape->intersect(intersection))
				doesIntersect = true;
		}
	}
	return doesIntersect;
}

double	OctTree::pointLightSourceLevel(Shape* light, Intersection& intersection) {
	Vector	V;
	Vector	N;
	double	facingRatio;
	double	distance;
	double	lightAmount;
	Ray		R;

	R = intersection.ray;
	R.origin = intersection.ray.calculate(intersection.t);
	distance = light->getOrigin().distance(R.origin);
	V = intersection.pShape->getOrigin() - R.origin;
	N = light->getNormal(R.calculate(intersection.t));
	N.normalize();
	facingRatio = MIN(0, dot(N, V));
	return ((light->getIntencity() - distance) / light->getIntencity()) * facingRatio;
}

double	OctTree::directLightSourceLevel(Shape* light, Intersection& intersection) {
	Vector	V;
	Vector	N;
	double	facingRatio;
	double	distance;
	double	lightAmount;
	Ray		R;

	V = light->getOrigin() - R.origin;
	R = intersection.ray;
	R.origin = V;
	R.direction = light->getOrigin();

	N = intersection.pShape->getNormal(R.calculate(intersection.t));
	N.normalize();
	facingRatio = (light->getIntencity() / 2) * dot(N, light->getOrigin());
	if (doesIntersectShapes(R)) {
		return 0;
	}
	return facingRatio;
}

bool	OctTree::intersectLights(Intersection& intersection) {
	bool doesIntersect = false;
	double lightAmount;

	for (Shape* light : this->lights) {
		if (light->getShapeType() == E_SPLight)
			lightAmount = pointLightSourceLevel(light, intersection);
		else
			lightAmount = directLightSourceLevel(light, intersection);
		if (lightAmount > 0)
			intersection.lightDistance = MAX(1, intersection.lightDistance + lightAmount);
		doesIntersect = true;
	}
	return doesIntersect;
}

bool	OctTree::doesIntersect(const Ray &r) {
    float tmin = (min.x - r.origin.x) / r.direction.x; 
    float tmax = (max.x - r.origin.x) / r.direction.x; 
	float tmp;

    if (tmin > tmax){
		tmp = tmin;
		tmin = tmax;
		tmax = tmp;
	} 
 
    float tymin = (min.y - r.origin.y) / r.direction.y; 
    float tymax = (max.y - r.origin.y) / r.direction.y; 
 
    if (tymin > tymax) {
		tmp = tymin;
		tymin = tymax;
		tymax = tmp;
	}
 
    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
 
    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 
 
    float tzmin = (min.z - r.origin.z) / r.direction.z; 
    float tzmax = (max.z - r.origin.z) / r.direction.z; 
 
    if (tzmin > tzmax) {
		tmp = tzmin;
		tzmin = tzmax;
		tzmax = tmp;
	}
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
 
    if (tzmin > tmin) 
        tmin = tzmin; 
 
    if (tzmax < tmax) 
        tmax = tzmax; 
 
    return true; 
}

