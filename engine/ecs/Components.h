#pragma once

#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <string>
#include <engine/ecs/ScriptableObject.h>
#include <box2d/box2d.h>
#include <engine/ecs/SceneCamera.h>

namespace engine {

    /// -------------------------------- BASICS AND COMMON FOR EVERYONE
    struct Transform {
        glm::mat4 transform { 1.0f };

        Transform() = default;
        Transform(const Transform& _transform) = default;
        Transform(const glm::mat4& _transform) : transform(_transform) {  }

        [[nodiscard]] Vec2f getPosition2D() const { return {transform[3][0], transform[3][1]}; }
        [[nodiscard]] float getX() const { return this->getPosition2D().x; }
        [[nodiscard]] float getY() const { return this->getPosition2D().y; }

        Vec2f getScale2D() const { return {transform[0][0], transform[1][1]}; }
        [[nodiscard]] float getScaleX() const { return this->getScale2D().x; }
        [[nodiscard]] float getScaleY() const { return this->getScale2D().y; }

        void setPosition2D(const Vec2f& _position) { this->transform[3][0] = _position.x; this->transform[3][1] = _position.y;}
        void setX(float _x) { this->transform[3][0] = _x; }
        void setY(float _y) { this->transform[3][1] = _y; }

        void setScale2D(const Vec2f& _scale) { this->transform[0][0] = _scale.x; this->transform[1][1] = _scale.y;}
        void setScaleX(float _x) { this->transform[0][0] = _x; }
        void setScaleY(float _y) { this->transform[1][1] = _y; }

        explicit operator glm::mat4& () { return this->transform; }
        explicit operator const glm::mat4& () const { return this->transform; }
    };

    struct Tag {
        std::string tag;

        Tag() = default;
        Tag(const Tag& _tag) = default;
        explicit Tag(const std::string& _tag) : tag(_tag) {  }

        explicit operator std::string& () { return this->tag; }
    };

    struct Active {
        bool active;

        Active() = default;
        Active(const Active& _active) = default;
        Active(bool _active) : active(_active) {  }
    };

    /// --------------------------------- RENDERING

    struct CameraComponent {
        SceneCamera sceneCamera;
        bool primary = true;
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent& _camera) = default;
    };

    struct SpriteRenderer {
        Color color = Color::Purple;

        SpriteRenderer() = default;
        SpriteRenderer(const SpriteRenderer&) = default;
        explicit SpriteRenderer(const Color& _color) : color(_color) {  }
    };

    /// --------------------------------- SCRIPTING

    struct NativeScript {
        ScriptableObject* scriptableObject = nullptr;
        std::string className;

        /// Function pointers are needed here, with std::function and using the lambdas made any second
        /// script not to run as scriptableObject is firstly well captured by the this in the lambda, but when a
        /// new script needs to be created, the this captured passes from one scriptableObject to another, leaving
        /// the first one as null and not able to work properly.
        void(* instantiateFunction)(ScriptableObject*&) = nullptr;
        void(* destroyInstanceFunction)(ScriptableObject*&) = nullptr;

        void(* onCreateFunction)(ScriptableObject*) = nullptr;
        void(* onDestroyFunction)(ScriptableObject*) = nullptr;
        void(* onUpdateFunction)(ScriptableObject*, Delta) = nullptr;

        template<typename T>
        void bind() {
            /// Just to get the name of the script
            std::stringstream _stream(typeid(T).name());
            std::vector<std::string>   _result;
            std::string _line;
            while(std::getline(_stream,_line, ':')) {
                _result.push_back(_line);
            }
            ///======== END

            instantiateFunction = [](ScriptableObject*& _instance) { _instance = new T(); };
            destroyInstanceFunction = [](ScriptableObject*& _instance) { delete static_cast<T*>(_instance); _instance = nullptr; };

            onCreateFunction = [](ScriptableObject* _instance) { static_cast<T*>(_instance)->onCreate(); };
            onDestroyFunction = [](ScriptableObject* _instance) { static_cast<T*>(_instance)->onDestroy(); };
            onUpdateFunction = [](ScriptableObject* _instance, Delta ts) { static_cast<T*>(_instance)->onUpdate(ts); };

        }

        ~NativeScript() {
            delete this->scriptableObject;
        }
    };

    struct CSharpScript {

    };

    /// --------------------------- PHYSICS

    struct BoxCollider {

    };

    struct CircleCollider {

    };

    struct PolygonCollider {

    };

}

#endif //ENGINE_COMPONENTS_H
