#include "object_base.h"

void ObjectBase::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, const std::vector<LightSource> &lightSources,
    bool isDay, float k_s) {
    openGlModel->draw(model, view, projection, lightSources, isDay, k_s);
}

ObjectBase::~ObjectBase() {
    delete openGlModel;
}
