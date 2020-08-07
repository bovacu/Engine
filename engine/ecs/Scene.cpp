#include "pch.h"
#include "Scene.h"
#include <engine//ecs/Components.h>
#include <engine/ecs/GameObject.h>

namespace engine {

    Scene::Scene() {

    }

    Scene::~Scene() {

    }

    GameObject Scene::createGameObject(const std::string& _name) {
        GameObject _gameObject = { this->gameObjectsRegistry.create(), this };
        _gameObject.addComponent<Transform>();
        auto& _tag = _gameObject.addComponent<Tag>(_name);
        _tag.tag = _name.empty() ? "GameObject" : _name;
        return _gameObject;
    }

    void Scene::onUpdate(Timestep _dt) {

    }

    void Scene::onFixUpdate(Timestep _fixedDt) {

    }

    void Scene::onRender(Timestep _dt) {

    }

    void Scene::onImGuiRender(Timestep _dt) {

    }

}