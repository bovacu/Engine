#include "pch.h"
#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace engine {

    void OpenGLMessageCallback(
            unsigned        _source,
            unsigned        _type,
            unsigned        _id,
            unsigned        _severity,
            int             _length,
            const char*     _message,
            const void*     _userParam) {

        switch (_severity) {
            case GL_DEBUG_SEVERITY_HIGH:         LOG_CRITICAL_CORE(_message); return;
            case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERROR_CORE(_message); return;
            case GL_DEBUG_SEVERITY_LOW:          LOG_WARN_CORE(_message); return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_TRACE_CORE(_message); return;
        }

        ENGINE_CORE_ASSERT(false, "Unknown severity level!");
    }

    void OpenGLRenderAPI::init() {

        #ifdef ENGINE_DEBUG
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(OpenGLMessageCallback, nullptr);

                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        #endif

//        glEnable(GL_DEPTH_TEST);                            // Using this, caused the engine not to render alpha
        glEnable(GL_BLEND);                                 // Enables transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Most typical blending function
    }

    void OpenGLRenderAPI::setViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) {
        glViewport(_x, _y, _width, _height);
    }

    void OpenGLRenderAPI::setClearColor(const glm::vec4& _color) {
        glClearColor(_color.r, _color.g, _color.b, _color.a);
    }

    void OpenGLRenderAPI::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderAPI::drawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount) {
        uint32_t _count = _indexCount ? _indexCount : _vertexArray->getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}