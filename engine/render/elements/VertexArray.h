#pragma once

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <engine/render/elements/Buffer.h>

namespace engine {

    class VertexArray;
    typedef std::shared_ptr<VertexArray> VertexArrayPtr;

    class VertexArray {
        public:
            virtual ~VertexArray() = default;

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual void addVertexBuffer(const VertexBufferPtr& vertexBuffer) = 0;
            virtual void setIndexBuffer(const IndexBufferPtr& indexBuffer) = 0;

            [[nodiscard]] virtual const std::vector<VertexBufferPtr>& getVertexBuffers() const = 0;
            [[nodiscard]] virtual const IndexBufferPtr& getIndexBuffer() const = 0;

            static std::shared_ptr<VertexArray> create();
    };

}

#endif // VERTEX_ARRAY_H
