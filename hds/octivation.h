#ifndef OCTAV_H
# define  OCTAV_H

# define OCTA_MAX_SIZE 1
# define OCTA_STARTING_MAX_BOX_LOCATION 50
# define OCTA_STARTING_MIN_BOX_LOCATION -50

# include "rt.h"

class OctTree {

public:
	OctTree(t_Main *a) : a(*a), id(1) {};
	~OctTree() {};

	void	octavateWorld();
	bool	doesIntersect(const Ray &r);

	bool 	intersectShapes(Intersection& intersection);
	bool 	intersectLights(Intersection& intersection);

	bool 	doesIntersectShapes(const Ray & r);
	bool 	doesIntersectLights(const Ray & r);

private:
	OctTree	*createOcta(OctTree *octa, int x, int y, int z);
	void	octavate(OctTree *octa);
	void	getWorldBoundingBox();
	void	shapeShifter(OctTree *o, std::vector<Shape *> shapes);
	void	lightShifter(OctTree *o, std::vector<Shape *> lights);

	void	initShapesAndLights(OctTree *o, std::vector<Shape *> things);

	double	pointLightSourceLevel(Shape* light, Intersection& intersection);
	double	directLightSourceLevel(Shape* light, Intersection& intersection);

public:
	t_Main					&a;
	int						id;
	Vector					min;
	Vector					max;
	std::vector<Shape*>		shapes;
	std::vector<Shape*>		lights;
	std::vector<OctTree *>	octas;

};

#endif