#include "pch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace engine {


    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& _specs) : specs(_specs) {
        this->invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() {
        glDeleteFramebuffers(1, &this->rendererID);
        glDeleteTextures(1, &this->colorAttachment);
        glDeleteTextures(1, &this->depthAttachment);
    }

    void OpenGLFrameBuffer::invalidate() {
        if (this->rendererID) {
            glDeleteFramebuffers(1, &this->rendererID);
            glDeleteTextures(1, &this->colorAttachment);
            glDeleteTextures(1, &this->depthAttachment);
        }

        glCreateFramebuffers(1, &this->rendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, this->rendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &this->colorAttachment);
        glBindTexture(GL_TEXTURE_2D, this->colorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->specs.width, this->specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &this->depthAttachment);
        glBindTexture(GL_TEXTURE_2D, this->depthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, this->specs.width, this->specs.height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->depthAttachment, 0);

        ENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->rendererID);
        glViewport(0, 0, this->specs.width, this->specs.height);
    }

    void OpenGLFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::resize(uint32_t _width, uint32_t _height) {
        this->specs.width = _width;
        this->specs.height = _height;
        this->invalidate();
    }
}