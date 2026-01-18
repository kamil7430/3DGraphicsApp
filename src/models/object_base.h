#ifndef INC_3DGRAPHICSAPP_OBJECT_BASE_H
#define INC_3DGRAPHICSAPP_OBJECT_BASE_H

#include "object_opengl_model.h"

class ObjectBase {
protected:
    ObjectOpenGlModel *openGlModel = nullptr;

public:
    virtual void draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, const std::vector<LightSource> &lightSources,
        bool isDay, float k_s);
    virtual ~ObjectBase();
};

#endif //INC_3DGRAPHICSAPP_OBJECT_BASE_H