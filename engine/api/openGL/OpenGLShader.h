#pragma once

#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <engine/render/elements/Shader.h>
#include <glad/glad.h>

namespace engine {

    class OpenGLShader : public Shader {

        private:
            uint32_t        renderID = 0;
            std::string     name;

        public:
            explicit OpenGLShader(const std::string& _filepath);
            OpenGLShader(const std::string& _name, const std::string& _vertexSrc, const std::string& _fragmentSrc);
             ~OpenGLShader()                                                                override;

            void bind() const                                                               override;
            void unbind() const                                                             override;

            void setInt(const std::string& _name, int _value)                               override;
            void setIntArray(const std::string& _name, int* _values, uint32_t _count)       override;
            void setFloat(const std::string& _name, float _value)                           override;
            void setFloat2(const std::string& _name, const glm::vec2& _value)               override;
            void setFloat3(const std::string& _name, const glm::vec3& _value)               override;
            void setFloat4(const std::string& _name, const glm::vec4& _value)               override;
            void setMat4(const std::string& _name, const glm::mat4& _value)                 override;

            [[nodiscard]] const std::string& getName() const                                override { return this->name; }

            void uploadUniformInt(const std::string& _name, int _value) const;
            void uploadUniformIntArray(const std::string& _name, int* _values, uint32_t _count) const;

            void uploadUniformFloat(const std::string& _name, float _value) const;
            void uploadUniformFloat2(const std::string& _name, const glm::vec2& _value) const;
            void uploadUniformFloat3(const std::string& _name, const glm::vec3& _value) const;
            void uploadUniformFloat4(const std::string& _name, const glm::vec4& _value) const;

            void uploadUniformMat3(const std::string& _name, const glm::mat3& _matrix) const;
            void uploadUniformMat4(const std::string& _name, const glm::mat4& _matrix) const;

    private:
            std::string readFile(const std::string& _filepath);
            std::unordered_map<GLenum, std::string> preProcess(const std::string& _source);
            void compile(const std::unordered_map<GLenum, std::string>& _shaderSources);
    };

}

#endif // OPENGL_SHADER_H
