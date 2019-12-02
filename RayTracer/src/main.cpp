#include <ctime>
#include <iostream>
#include <fstream>
#include "rt.h"
#include "image.h"
#include "camera.h"
#include "shape.h"
#include "SDL.hpp"
#include "light.h"

t_Main	a;
OctTree	*boundingBox;

int X1, Y1, X2, Y2 = 0;

using namespace std;

void    rayTrace(int piece, int max_pieces) {
    std::cout   << "TRACING\n";

    X1 = 0;
    X2 = g_width;
    Y1 = 0;
    Y2 = g_height;
    if (piece == 1) {
        X1 = 0;
		X2 = g_width / 2;
    } else if (piece == 2) {
        X1 = g_width / 2;
        X2 = g_width;
    }

	Camera		*camera = a.camera;
	ShapeSet	*scene	= a.scene;
	double		r, g, b;
	
	drawHitBounds(boundingBox);
	for (int x = X1; x < X2; x++) {
		for (int y = Y1; y < Y2; y++) {
			Vector2 screenCoord((2.0f*x) / g_width - 1.0f,
								(-2.0f*y) / g_height + 1.0f);
			Ray ray = camera->makeRay(screenCoord);
			Intersection intersection(ray);

			//if (calcInterShapes(boundingBox, intersection)) {
			if (a.scene->intersect(intersection)) {
				Intersection lightIntersection = intersection;

				if (boundingBox->intersectLights(lightIntersection)) {
					intersection.lightDistance = lightIntersection.lightDistance;
				}

				r = intersection.color.r * 255.0f * intersection.lightDistance;
				g = intersection.color.g * 255.0f * intersection.lightDistance;
				b = intersection.color.b * 255.0f * intersection.lightDistance;

				a.sdl->putpixel(x, y, Color(r, g, b));
			} else {
				a.sdl->putpixel(x, y, 0);
			}
		}
	}
	std::cout << "TRACED!\n";
}

bool	calcInterShapes(OctTree *o, Intersection &intersection) {
	bool f = false;
	bool r = false;

	if (o->doesIntersect(intersection.ray)) {
		if (o->octas.size() == 8) {
			for (int i = 0; i < 8; i++) {
				f = calcInterShapes(o->octas[i], intersection);
				r = r ? true : f;
			}
		} else if (o->intersectShapes(intersection)) {
			Intersection lightIntersection = intersection;

			lightIntersection.ray.origin = lightIntersection.ray.calculate(lightIntersection.t);
			if (boundingBox->intersectLights(lightIntersection)) {
				intersection.lightDistance = lightIntersection.lightDistance;
			}
			return true;
		}
	}
	return r;
}

bool	calcInterLights(OctTree *o, Intersection &intersection) {
	bool f = false;
	bool r = false;

	if (o->doesIntersect(intersection.ray)) {
		if (o->octas.size() == 8) {
			for (int i = 0; i < 8; i++) {
				f = calcInterLights(o->octas[i], intersection);
				r = r ? true : f;
			}
		} else if (o->intersectLights(intersection)) {
			return true;			
		}
	}
	return r;
}

void	drawRect(Vector min, Vector max, Color c, int i = 1) {
	if (i) {
		a.minimap->drawLine(0 + min.x, 250 + min.z, 0 + max.x, 250 + min.z, c);
		a.minimap->drawLine(0 + min.x, 250 + min.z, 0 + min.x, 250 + max.z, c);
		a.minimap->drawLine(0 + max.x, 250 + max.z, 0 + max.x, 250 + min.z, c);
		a.minimap->drawLine(0 + max.x, 250 + max.z, 0 + min.x, 250 + max.z, c);
	} else {
		a.minimap->drawLine(350 + min.x, 250 + min.y, 350 + max.x, 250 + min.y, c);
		a.minimap->drawLine(350 + min.x, 250 + min.y, 350 + min.x, 250 + max.y, c);
		a.minimap->drawLine(350 + max.x, 250 + max.y, 350 + max.x, 250 + min.y, c);
		a.minimap->drawLine(350 + max.x, 250 + max.y, 350 + min.x, 250 + max.y, c);
	}

}

void	drawHitBounds(OctTree *o) {
	Color c;

	drawRect(o->min, o->max, {255, 0, 0});
	if (o->octas.size() == 8)
		for (int i = 0; i < 8; i++) {
			drawRect(o->octas[i]->min, o->octas[i]->max, {255, 0, 0}, 1);
			drawHitBounds(o->octas[i]);
		}
}

void 	draw(OctTree *o) {
	if (o->octas.size() > 0)
	for (int i = 0; i < 8; i++) {
		drawRect(o->octas[i]->min, o->octas[i]->max, {255, 0, 0}, 1);
		drawRect(o->octas[i]->min, o->octas[i]->max, {255, 0, 0}, 0);
		draw(o->octas[i]);
	}
}

void	spawnSphere(Vector v, double size, Color c) {
	Sphere *sphere = new Sphere(v, size, c);
	a.scene->addShape(sphere);
}


void	spawnLight(Vector v, double size, E_ShapeType type) {
	Light *light = new Light(v, size, type);
	a.scene->addShape(light);
}

void	spawnPlane(Vector v, Vector n, Color c) {
	Plane *floor = new Plane(v, n, c);
	a.scene->addShape(floor);
}

void	init() {
	boundingBox = new OctTree(&a);

	a.sdl = new SDL(g_width, g_height);
	a.sdl->display();
	SDL_SetWindowPosition(a.sdl->window, 0, 200);

	a.minimap = new SDL(g_width, g_height);
	SDL_SetWindowPosition(a.minimap->window, g_width, 200);
	a.minimap->display();

	a.camera = new PerspectiveCamera(
		Point(-5.0f, 1.0f, 0.0f),
		Vector(0.0f, 1.0f, 0.0f),
		Vector(), 70.0f * PI / 180.0f,
		(double)g_width / (double)g_height);
	a.scene = new ShapeSet;
}

void	createScene() {
	spawnPlane({0.0f, 0.0f, 0.0f}, Vector(), {0.5f, 1.0f, 0.5f});
	// spawnSphere({5.0f, 2.0f, 5.0f}, 1, {1.0f, 1.0f, 1.0f});

	// spawnSphere({5.0f, 0.0f, 0.0f}, 3, {1.0f, 0.0f, 0.0f});
	// spawnSphere({15.0f, 0.0f, 0.0f}, 3, {1.0f, 1.0f, 0.0f});

	// spawnSphere({5.0f, 4.0f, 0.0f}, 1, {1.0f, 1.0f, 1.0f});
    spawnLight({5.0f, 5.0f, 0.0f}, 25, E_SPLight);
    return ;
	
	for (int i = -20; i < 20; i += 2) {
		spawnSphere(
		   {5.0f * abs(i + i), 
			2.0f * abs(i + i), 
			5.0f * i + i},
			1, {
			1.0f * (abs(i) % 3 == 0), 
			1.0f * (abs(i) % 3 == 1), 
			1.0f * (abs(i) % 3 == 2)});
	}

	for ( double y = 0 ; y < 10 ; y++ ) {
		for ( double x = 0 ; x < 10 ; x++ ) {
		spawnSphere(
		{
			60,
			4 * y,
			4 * (x - 5)
			},
			2,
			{
			0,
			0,
			1
			});

		}
	}

	for (int i = 1; i < 50; i++) {
		spawnSphere(
		   {(100.0f * (rand() % i) - 50.0f), 
			1, 
			(100.0f * (rand() % i) - 50.0f)},
			1, {
			1.0f * (abs(i) % 3 == 0), 
			1.0f * (abs(i) % 3 == 1), 
			1.0f * (abs(i) % 3 == 2)});
	}
}

int		main(int argc, char *argv[]) {
	char *name;

	srand(1000);
	init();
	createScene();

	boundingBox->octavateWorld();

	time_t stime = time(NULL);

	if (argc > 1) {
		name = (char*)(void *)"Display 1.bmp";
        rayTrace(atoi(argv[1]), 2);
    } else {
		name = (char*)(void *)"Display 2.bmp";
    }
	std::cout << time(NULL) - stime << " seconds to trace" << std::endl;
	rayTrace(0, 2);
	std::cout << "Starting display driver: " << std::endl;
	a.sdl->display();
	a.minimap->display();
	std::cout << "done" << std::endl;
	bool running = true;
	while (running) {
		stime = time(NULL);
		SDL_Delay(16);
	}
}
