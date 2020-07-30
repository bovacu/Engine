#pragma once

#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <glm.hpp>
#include <string>

namespace engine {

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
        Tag(const std::string& _tag) : tag(_tag) {  }

        explicit operator std::string& () { return this->tag; }
    };

}

#endif //ENGINE_COMPONENTS_H
