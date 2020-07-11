#pragma once

#ifndef ENGINE_OPENGLFRAMEBUFFER_H
#define ENGINE_OPENGLFRAMEBUFFER_H


#include <engine/render/elements/FrameBuffer.h>

namespace engine {

    class OpenGLFrameBuffer : public FrameBuffer {
        private:
            uint32_t rendererID = 0;
            uint32_t colorAttachment = 0, depthAttachment = 0;
            FrameBufferSpecification specs;

        public:
            explicit OpenGLFrameBuffer(const FrameBufferSpecification& _specs);
            ~OpenGLFrameBuffer() override;

            void invalidate();

            void bind()                                                             override;
            void unbind()                                                           override;

            void resize(uint32_t _width, uint32_t _height)                          override;

            [[nodiscard]] uint32_t getColorAttachmentRendererID() const             override { return this->colorAttachment; }

            [[nodiscard]] const FrameBufferSpecification& getSpecification() const  override { return this->specs; }
    };

}

#endif //ENGINE_OPENGLFRAMEBUFFER_H
