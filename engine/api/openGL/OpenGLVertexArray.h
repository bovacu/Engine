#pragma once

#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include <engine/render/elements/VertexArray.h>

namespace engine {

    class OpenGLVertexArray : public VertexArray {
        private:
            uint32_t                        rendererID = 0;
            uint32_t                        vertexBufferIndex = 0;
            std::vector<VertexBufferPtr>    vertexBuffers;
            IndexBufferPtr                  indexBuffer;

        public:
            OpenGLVertexArray();
            ~OpenGLVertexArray()                                                        override;

            void bind() const                                                           override;
            void unbind() const                                                         override;

            void addVertexBuffer(const VertexBufferPtr& _vertexBuffer)                  override;
            void setIndexBuffer(const IndexBufferPtr& _indexBuffer)                     override;

            [[nodiscard]] const std::vector<VertexBufferPtr>& getVertexBuffers() const  override { return this->vertexBuffers; }
            [[nodiscard]] const IndexBufferPtr& getIndexBuffer() const                  override { return this->indexBuffer; }
    };

}

#endif // OPENGL_VERTEX_ARRAY_H
