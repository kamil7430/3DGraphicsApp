#ifndef INC_3DGRAPHICSAPP_SPHERE_H
#define INC_3DGRAPHICSAPP_SPHERE_H

#include "../object_base.h"

class Sphere final : public ObjectBase {
public:
    Sphere(const int stackCount, const int sectorCount);
};

#endif //INC_3DGRAPHICSAPP_SPHERE_H