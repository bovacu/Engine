#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "pch.h"

#include "engine/components/transform/Transform2D.h"
#include "engine/components/Component.h"
#include "engine/components/colliders/BoxCollider.h"
#include "engine/components/colliders/CircleCollider.h"
#include "engine/components/colliders/PolygonCollider.h"
#include "engine/components/physics/PhysicsBody.h"

#define TAG_SIZE 30

namespace engine {

    class GameObject {
        private:
            char tag[TAG_SIZE];
        
        public:
            Transform2D transform;
            std::vector<Ptr::ComponentPtr> components;

        public:
            GameObject();
            GameObject(const Transform2D& _transform);
            GameObject(const Vec2f& _position, float _rotation = 0, const Vec2f _scale = {0, 0});

            char* getTag() { return this->tag; }
            void setTag(char* _tag) { strcpy(this->tag, _tag); }

            Ptr::ComponentPtr addComponent(Ptr::ComponentPtr _component);
            Ptr::ComponentPtr removeComponent(const Ptr::ComponentPtr& _component);

            Ptr::BoxColliderPtr     addBoxCollider(const Size& _size, bool _isGhost = false);
            Ptr::CircleColliderPtr  addCircleCollider(float _radius, bool _isGhost = false);
            Ptr::PolygonColliderPtr addPolygonCollider(const std::vector<Vec2f> _vertices, bool _isGhost = false);
            Ptr::PhysicsBodyPtr     addPhysicsBody();
            
            template<class T>
            std::shared_ptr<T> getComponentOfType();

            template<class T>
            std::vector<std::shared_ptr<T>> getComponentsOfType();
    };

    template<class T>
    std::shared_ptr<T> GameObject::getComponentOfType() {
        std::shared_ptr<T> _componentCasted = nullptr;
        for(int _c = 0; _c < this->components.size(); _c++) {
            if((_componentCasted = std::dynamic_pointer_cast<T>(this->components[_c])) != nullptr) {
                return _componentCasted;
            }
        }

        return _componentCasted;
    }

    template<class T>
    std::vector<std::shared_ptr<T>> GameObject::getComponentsOfType() {
        std::vector<std::shared_ptr<T>> _components;

        std::shared_ptr<T> _componentCasted = nullptr;
        for(int _c = 0; _c < this->components.size(); _c++)
            if((_componentCasted = std::dynamic_pointer_cast<T>(this->components[_c])) != nullptr)
                _components.push_back(_componentCasted);

        return _components;
    }

    namespace Ptr {
        typedef std::shared_ptr<GameObject> GameObjectPtr;
    }
}

#endif //GAME_OBJECT_H
