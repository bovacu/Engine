#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace engine {

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t _size) {
        glCreateBuffers(1, &this->rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);
        glBufferData(GL_ARRAY_BUFFER, _size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* _vertices, uint32_t _size) {
        glCreateBuffers(1, &this->rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);
        glBufferData(GL_ARRAY_BUFFER, _size, _vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &this->rendererID);
    }

    void OpenGLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);
    }

    void OpenGLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void* _data, uint32_t _size) {
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _size, _data);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* _indices, uint32_t _count)
            : count(_count) {
        glCreateBuffers(1, &this->rendererID);

        // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
        // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);
        glBufferData(GL_ARRAY_BUFFER, _count * sizeof(uint32_t), _indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        glDeleteBuffers(1, &this->rendererID);
    }

    void OpenGLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->rendererID);
    }

    void OpenGLIndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}