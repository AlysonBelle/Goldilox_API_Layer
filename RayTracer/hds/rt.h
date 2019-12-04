#ifndef RT_H
#define RT_H

enum eKey {eKNone, eKUp, eKDown, eKRight, eKLeft, eKForward, eKSpace, eKBackward, eKExit};

#define MAX(x, y) x < y ? x : y
#define MIN(x, y) x > y ? x : y

# include <algorithm>
# include <iostream>
# include "SDL.hpp"

# include "vectormath.h"
# include "shape.h"
# include "camera.h"

typedef struct			s_Main {
	SDL					*sdl;
	SDL					*minimap;
	ShapeSet			*scene;
	PerspectiveCamera	*camera;
}						t_Main;

# include "octivation.h"
# include "image.h"
# include "camera.h"
# include "shape.h"
# include "SDL.hpp"
# include "light.h"

class OctTree;

static	int		g_width		= 1000;
static	int		g_height	= 1000;

void	drawRect(Vector min, Vector max, Color c, int i);
void	drawHitBounds(OctTree *o);
void	rayTrace();
bool	calcInterShapes(OctTree *o, Intersection &intersection);
bool	calcInterLights(OctTree *o, Intersection &intersection);

#endif