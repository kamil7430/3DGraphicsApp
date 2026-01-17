#include "object_base.h"

void ObjectBase::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
    openGlModel->draw(model, view, projection);
}

ObjectBase::~ObjectBase() {
    delete openGlModel;
}
