#include "pch.h"
#include "Shader.h"

#include <engine/render/Renderer.h>
#include <engine/api/openGL/OpenGLShader.h>

namespace engine {

    ShaderPtr Shader::create(const std::string& _filepath) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(_filepath);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    ShaderPtr Shader::create(const std::string& _name, const std::string& _vertexSrc, const std::string& _fragmentSrc) {
        switch (Renderer::getAPI()) {
            case RenderAPI::API::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(_name, _vertexSrc, _fragmentSrc);
        }

        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    void ShaderLibrary::add(const std::string& _name, const ShaderPtr& _shader) {
        ENGINE_CORE_ASSERT(!this->exists(_name), "Shader already exists!");
        this->shaders[_name] = _shader;
    }

    void ShaderLibrary::add(const ShaderPtr& _shader) {
        auto& _name = _shader->getName();
        this->add(_name, _shader);
    }

    ShaderPtr ShaderLibrary::load(const std::string& _filepath) {
        auto _shader = Shader::create(_filepath);
        this->add(_shader);
        return _shader;
    }

    ShaderPtr ShaderLibrary::load(const std::string& _name, const std::string& _filepath) {
        auto _shader = Shader::create(_filepath);
        this->add(_name, _shader);
        return _shader;
    }

    ShaderPtr ShaderLibrary::get(const std::string& _name) {
        ENGINE_CORE_ASSERT(!this->exists(_name), "Shader not found!");
        return this->shaders[_name];
    }

    bool ShaderLibrary::exists(const std::string& _name) const {
        return this->shaders.find(_name) != this->shaders.end();
    }

}