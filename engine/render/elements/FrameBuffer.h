#pragma once

#ifndef ENGINE_FRAMEBUFFER_H
#define ENGINE_FRAMEBUFFER_H


namespace engine {

    struct FrameBufferSpecification {
        uint32_t    width{}, height{};
        uint32_t    samples = 1;
        bool        swapChainTarget = false;
    };

    class FrameBuffer;
    typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

    class FrameBuffer {
        public:
            virtual ~FrameBuffer()                                                          = default;

            virtual void bind()                                                             = 0;
            virtual void unbind()                                                           = 0;

            virtual void resize(uint32_t _width, uint32_t _height)                          = 0;
            [[nodiscard]] virtual uint32_t getColorAttachmentRendererID() const             = 0;
            [[nodiscard]] virtual const FrameBufferSpecification& getSpecification() const  = 0;

            static FrameBufferPtr create(const FrameBufferSpecification& _specs);
    };

}

#endif //ENGINE_FRAMEBUFFER_H
