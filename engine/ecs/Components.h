#pragma once

#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <glm.hpp>
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

    /// --------------------------------- RENDERING

    struct CameraComponent {
        SceneCamera sceneCamera;
        bool primary = true;
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent& _camera) = default;
    };

    struct SpriteRenderer {
        Color color;

        SpriteRenderer() = default;
        SpriteRenderer(const SpriteRenderer&) = default;
        explicit SpriteRenderer(const Color& _color) : color(_color) {  }
    };

    /// --------------------------------- SCRIPTING

    struct NativeScript {
        ScriptableObject* scriptableObject;

        std::function<void()> instantiate;
        std::function<void()> destroy;

        std::function<void(ScriptableObject*)> onCreate;
        std::function<void(ScriptableObject*)> onDestroy;
        std::function<void(ScriptableObject*, float _dt)> onUpdate;

        template<typename T>
        void bind() {
            this->instantiate   = [&]() { this->scriptableObject = new T(); };
            this->destroy       = [&]() { delete (T*)this->scriptableObject; this->scriptableObject = nullptr; };

            this->onCreate      = [](ScriptableObject* _so) { ((T*)_so)->onCreate(); };
            this->onDestroy     = [](ScriptableObject* _so) { ((T*)_so)->onDestroy(); };
            this->onUpdate      = [](ScriptableObject* _so, float _dt) { ((T*)_so)->onUpdate(_dt); };
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
