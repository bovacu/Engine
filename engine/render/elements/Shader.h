#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

namespace engine {

    class Shader {
        public:
            virtual ~Shader() = default;

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual void setInt(const std::string& _name, int _value) = 0;
            virtual void setIntArray(const std::string& _name, int* _values, uint32_t _count) = 0;
            virtual void setFloat(const std::string& _name, float _value) = 0;
            virtual void setFloat3(const std::string& _name, const glm::vec3& _value) = 0;
            virtual void setFloat4(const std::string& _name, const glm::vec4& _value) = 0;
            virtual void setMat4(const std::string& _name, const glm::mat4& _value) = 0;

            virtual const std::string& getName() const = 0;

            static std::shared_ptr<Shader> create(const std::string& _filepath);
            static std::shared_ptr<Shader> create(const std::string& _name, const std::string& _vertexSrc,
                    const std::string& _fragmentSrc);
    };

    class ShaderLibrary {
        private:
            std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

        public:
            void add(const std::string& _name, const std::shared_ptr<Shader>& _shader);
            void add(const std::shared_ptr<Shader>& _shader);
            std::shared_ptr<Shader> load(const std::string& _filepath);
            std::shared_ptr<Shader> load(const std::string& _name, const std::string& _filepath);

            std::shared_ptr<Shader> get(const std::string& _name);

            bool exists(const std::string& _name) const;
    };
}

#endif // SHADER_H
