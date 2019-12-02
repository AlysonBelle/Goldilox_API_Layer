#include "light.h"

Light::Light(const Point& centre, double intensity, E_ShapeType type) : intensity(intensity), type(type) {
    this->centre = centre;
    this->radius = 1;
    this->color = {1, 1, 1};
}

Light::~Light() {

}
