#include "pch.h"
#include "Scene.h"

namespace engine {

    Scene::~Scene() {
        for (Layer* _layer : this->stack) {
            _layer->onEnd();
            delete _layer;
        }
    }
}