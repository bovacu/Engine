#pragma once

#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include <engine/render/elements/Buffer.h>

namespace engine {

    class OpenGLVertexBuffer : public VertexBuffer {
        private:
            uint32_t            rendererID = 0;
            BufferLayout        layout;

        public:
            explicit OpenGLVertexBuffer(uint32_t _size);
            OpenGLVertexBuffer(float* _vertices, uint32_t _size);
            ~OpenGLVertexBuffer()                                   override;

            void bind() const                                       override;
            void unbind() const                                     override;

            void setData(const void* _data, uint32_t _size)         override;

            [[nodiscard]] const BufferLayout& getLayout() const     override { return this->layout; }
            void setLayout(const BufferLayout& _layout)             override { this->layout = _layout; }
    };

    class OpenGLIndexBuffer : public IndexBuffer {
        private:
            uint32_t            rendererID = 0;
            uint32_t            count      = 0;

        public:
            OpenGLIndexBuffer(uint32_t* _indices, uint32_t _count);
            ~OpenGLIndexBuffer()                                    override;

            void bind() const                                       override;
            void unbind() const                                     override;

            [[nodiscard]] uint32_t getCount() const                 override { return this->count; }
    };

}

#endif // OPENGL_BUFFER_H
