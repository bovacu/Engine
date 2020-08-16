#include "pch.h"
#include "Scene.h"
#include <engine//ecs/Components.h>
#include <engine/render/Renderer.h>

namespace engine {

    ScenePtr Scene::create(const std::string& _name) {
        return std::make_shared<Scene>(_name);
    }

    Scene::Scene(const std::string& _name) : name(_name) {

    }

    Scene::~Scene() {

    }

    GameObject Scene::createGameObject(const std::string& _name) {
        GameObject _gameObject = { this->gameObjectsRegistry.create(), this };
        _gameObject.addComponent<Active>(true);
        _gameObject.addComponent<Transform>();
        auto& _tag = _gameObject.addComponent<Tag>(_name);
        _tag.tag = _name.empty() ? "GameObject" : _name;
        return _gameObject;
    }

    void Scene::onUpdate(Delta _dt) {
        this->gameObjectsRegistry.view<Active, NativeScript>().each([=] (auto _gameObject, auto& _active, auto& _nativeScript) {
            if(_active.active) {
                if(!_nativeScript.scriptableObject) {
                    _nativeScript.instantiate();
                    _nativeScript.scriptableObject->gameObject = GameObject{ _gameObject, this };

                    if(_nativeScript.onCreate)
                        _nativeScript.onCreate(_nativeScript.scriptableObject);
                }

                if(_nativeScript.onUpdate)
                    _nativeScript.onUpdate(_nativeScript.scriptableObject, _dt);
            }
        });

    }

    void Scene::onFixUpdate(Delta _fixedDt) {

    }

    void Scene::onRender(Delta _dt) {
        Camera* _camera = nullptr;
        glm::mat4* _cameraTransform = nullptr;

        auto _view = this->gameObjectsRegistry.view<Active, Transform, CameraComponent>();
        for(auto _gameObject : _view) {
            auto& _active = _view.get<Active>(_gameObject);

            if(_active.active) {
                auto& _transform = _view.get<Transform>(_gameObject);
                auto& _cam = _view.get<CameraComponent>(_gameObject);

                if(_cam.primary) {
                    _camera = &_cam.sceneCamera;
                    _cameraTransform = &_transform.transform;
                    break;
                }
            }
        }

        if(_camera) {
            Renderer::beginDrawCall(_camera->getProjectionMatrix(), *_cameraTransform);
                auto _group = this->gameObjectsRegistry.group<Active, Transform>(entt::get<SpriteRenderer>);
                for(auto _gameObject : _group) {
                    auto& _active = _group.get<Active>(_gameObject);

                    if(_active.active) {
                        auto& _transform = _group.get<Transform>(_gameObject);
                        auto& _sprite = _group.get<SpriteRenderer>(_gameObject);
                        Renderer::drawRectangle(_transform.transform, _sprite.color);
                    }
                }
            Renderer::endDrawCall();
        }
    }

    void Scene::onImGuiRender(Delta _dt) {

    }

    void Scene::onViewportResize(int _width, int _height) {
        this->viewportWidth = _width;
        this->viewportHeight = _height;

        auto _view = this->gameObjectsRegistry.view<CameraComponent>();
        for(auto _gameObject : _view) {
            auto& _cameraComponent = _view.get<CameraComponent>(_gameObject);
            if(!_cameraComponent.fixedAspectRatio)
                _cameraComponent.sceneCamera.setViewportSize(_width, _height);
        }
    }

}