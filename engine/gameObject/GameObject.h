#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <engine/components/transform/Transform2D.h>
#include <engine/components/Component.h>
#include <engine/components/colliders/BoxCollider.h>
#include <engine/components/colliders/CircleCollider.h>
#include <engine/components/colliders/PolygonCollider.h>
#include <engine/components/physics/PhysicsBody.h>
#include <engine/components/sprite/Sprite.h>

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
            explicit GameObject(const Transform2D& _transform);
            explicit GameObject(const Vec2f& _position, float _rotation = 0, const Vec2f& _scale = {1, 1});

            char* getTag() { return this->tag; }
            void setTag(char* _tag) { strcpy_s(this->tag, _tag); }

            Component*              addComponent(Component* _component);
            Component*              removeComponent(Component* _component);

            BoxCollider*            addBoxCollider(const Size& _size, bool _isGhost = false);
            CircleCollider*         addCircleCollider(float _radius, bool _isGhost = false);
            PolygonCollider*        addPolygonCollider(const std::vector<Vec2f>& _vertices, bool _isGhost = false);
            PhysicsBody*            addPhysicsBody();
            Sprite*                 addSprite(const std::shared_ptr<TextureRegion>& = nullptr);
            
            template<class T>
            T* getComponentOfType();

            template<class T>
            std::vector<T*> getComponentsOfType();

        public:
            static GameObject* create(const Transform2D& _transform);
            static GameObject* create(const Vec2f& _position, float _rotation = 0, const Vec2f& _scale = {1, 1});
    };

    template<class T>
    T* GameObject::getComponentOfType() {
        T* _componentCasted = nullptr;
        for(int _c = 0; _c < this->components.size(); _c++) {
            if((_componentCasted = std::dynamic_pointer_cast<T>(this->components[_c])) != nullptr) {
                return _componentCasted;
            }
        }

        return _componentCasted;
    }

    template<class T>
    std::vector<T*> GameObject::getComponentsOfType() {
        std::vector<T*> _components;

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
