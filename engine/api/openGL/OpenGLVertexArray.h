#pragma once

#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "engine/render/elements/VertexArray.h"

namespace engine {

    class OpenGLVertexArray : public VertexArray {
        private:
            uint32_t rendererID;
            uint32_t vertexBufferIndex = 0;
            std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
            std::shared_ptr<IndexBuffer> indexBuffer;

        public:
            OpenGLVertexArray();
            virtual ~OpenGLVertexArray();

            virtual void bind() const override;
            virtual void unbind() const override;

            virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& _vertexBuffer) override;
            virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& _indexBuffer) override;

            virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return this->vertexBuffers; }
            virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return this->indexBuffer; }
    };

}

#endif // OPENGL_VERTEX_ARRAY_H
