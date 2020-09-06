#include "pch.h"
#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace engine {

    /// Callback for OpenGL in case there is an error while rendering.
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
            default : {  };
        }

        ENGINE_CORE_ASSERT(false, "Unknown severity level!");
    }

    void OpenGLRenderAPI::init() {

        /// Enabling some debugging info to get the most of an error.
        #ifdef ENGINE_DEBUG
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(OpenGLMessageCallback, nullptr);

                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        #endif

        /// Enables transparency
        glEnable(GL_BLEND);

        /// Most typical blending function
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /// Using this, caused the engine not to render alpha
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRenderAPI::setViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) {
        /// Sets viewport position and size.
        glViewport(_x, _y, _width, _height);
    }

    void OpenGLRenderAPI::setClearColor(const glm::vec4& _color) {
        /// Sets the color to draw the background.
        glClearColor(_color.r / 255.f, _color.g / 255.f, _color.b / 255.f, _color.a / 255.f);
    }

    void OpenGLRenderAPI::clear() {
        /// Clears the rendering buffer from all pixels.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderAPI::drawIndexed(const VertexArrayPtr& _vertexArray, uint32_t _indexCount) {
        /// This is in case the index count is passed wrongly.
        uint32_t _count = _indexCount ? _indexCount : _vertexArray->getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLRenderAPI::setClearColor(const Color& _color) {
        this->setClearColor(glm::vec4{(float)_color.r, (float)_color.g, (float)_color.b, (float)_color.a});
    }

}