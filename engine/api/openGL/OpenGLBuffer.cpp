#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace engine {

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t _size) {
        /// First the buffer to store the data is created.
        glCreateBuffers(1, &this->rendererID);

        /// Second, we bind the buffer to be able to work with it.
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);

        /// Third, we tell OpenGL what we need to store and the size, but as we don't know the vertices to draw, we need
        /// to use GL_DYNAMIC_DRAW, and that's why we set nullptr.
        glBufferData(GL_ARRAY_BUFFER, _size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* _vertices, uint32_t _size) {
        /// First the buffer to store the data is created.
        glCreateBuffers(1, &this->rendererID);

        /// Second, we bind the buffer to be able to work with it.
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);

        /// Third, we tell OpenGL what we need to store and the size and as we know the vertices, we pass them and we
        /// set GL_STATIC_DRAW.
        glBufferData(GL_ARRAY_BUFFER, _size, _vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        /// This deletes the stored buffer from memory.
        glDeleteBuffers(1, &this->rendererID);
    }

    void OpenGLVertexBuffer::bind() const {
        /// This sets this current buffer as the one to be using.
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);
    }

    void OpenGLVertexBuffer::unbind() const {
        /// This removes the buffer as the one to be using.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void* _data, uint32_t _size) {
        /// First we bind the buffer.
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);

        /// Then we set the data we want to update. We use BufferSubData as we already previously set the data first.
        glBufferSubData(GL_ARRAY_BUFFER, 0, _size, _data);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* _indices, uint32_t _count)
            : count(_count) {
        /// First the buffer to store the data is created.
        glCreateBuffers(1, &this->rendererID);

        /// GL_ELEMENT_ARRAY_BUFFER is the correct one to use, but it is not valid without an actively bound VAO, so
        /// initially we can't use it until the VAO is built.
        /// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
        /// Second, we bind the buffer to be able to work with it.
        glBindBuffer(GL_ARRAY_BUFFER, this->rendererID);

        /// Third, we tell OpenGL what we need to store and the size and as we know the indices, we pass them and we
        /// set GL_STATIC_DRAW.
        glBufferData(GL_ARRAY_BUFFER, _count * sizeof(uint32_t), _indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        /// This deletes the stored buffer from memory.
        glDeleteBuffers(1, &this->rendererID);
    }

    void OpenGLIndexBuffer::bind() const {
        /// This sets this current buffer as the one to be using. Now we can use and we use GL_ELEMENT_ARRAY_BUFFER.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->rendererID);
    }

    void OpenGLIndexBuffer::unbind() const {
        /// This removes the buffer as the one to be using. Now we can use and we use GL_ELEMENT_ARRAY_BUFFER.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}