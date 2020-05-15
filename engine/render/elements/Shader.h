#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

namespace engine {

    class Shader;
    typedef std::shared_ptr<Shader> ShaderPtr;

    class Shader {
        public:
            virtual ~Shader() = default;

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual void setInt(const std::string& _name, int _value) = 0;
            virtual void setIntArray(const std::string& _name, int* _values, uint32_t _count) = 0;
            virtual void setFloat(const std::string& _name, float _value) = 0;
            virtual void setFloat3(const std::string& _name, const glm::vec3& _value) = 0;
            virtual void setFloat2(const std::string& _name, const glm::vec2& _value) = 0;
            virtual void setFloat4(const std::string& _name, const glm::vec4& _value) = 0;
            virtual void setMat4(const std::string& _name, const glm::mat4& _value) = 0;

            [[nodiscard]] virtual const std::string& getName() const = 0;

            static ShaderPtr create(const std::string& _filepath);
            static ShaderPtr create(const std::string& _name, const std::string& _vertexSrc, const std::string& _fragmentSrc);
    };

    class ShaderLibrary;
    typedef std::shared_ptr<ShaderLibrary> ShaderLibraryPtr;

    class ShaderLibrary {
        private:
            std::unordered_map<std::string, ShaderPtr> shaders;

        public:
            void add(const std::string& _name, const ShaderPtr& _shader);
            void add(const ShaderPtr& _shader);
            ShaderPtr load(const std::string& _filepath);
            ShaderPtr load(const std::string& _name, const std::string& _filepath);

            ShaderPtr get(const std::string& _name);

            [[nodiscard]] bool exists(const std::string& _name) const;
    };
}

#endif // SHADER_H
