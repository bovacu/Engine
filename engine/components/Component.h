#pragma once
#include "pch.h"

#ifndef COMPONENT_H
#define COMPONENT_H

namespace engine {
    class GameObject;
    enum ComponentType { COLLIDER, BOX_COLLIDER, CIRCLE_COLLIDER, POLYGON_COLLIDER, PHYSICS_BODY, SPRITE,NONE };

    class Component;
    namespace Ptr {
        typedef std::shared_ptr<Component> ComponentPtr;
    }

    class Component {
        
        public:
            GameObject* gameObject;
            ComponentType componentType;

        public:
            Component(GameObject* _gameObject, const ComponentType& _componentType = ComponentType::NONE)  : gameObject(_gameObject), componentType(_componentType) {  }

        public:
            virtual void update(float _dt) {};
            virtual void fixUpdate(float _fixDt) {};
            virtual void postUpdate(float _dt) {};
    };
}

#endif // COMPONENT_H