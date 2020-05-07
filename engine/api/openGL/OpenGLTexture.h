#pragma once

#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "engine/render/elements/Texture.h"
#include <glad/glad.h>

namespace engine {

    class OpenGLTexture : public Texture2D {
        private:
            std::string path;
            uint32_t width, height;
            uint32_t rendererID;
            GLenum internalFormat, dataFormat;

        public:
            OpenGLTexture(uint32_t _width, uint32_t _height);
            OpenGLTexture(const std::string& _path);
            virtual ~OpenGLTexture();

            virtual uint32_t getWidth() const override { return this->width;  }
            virtual uint32_t getHeight() const override { return this->height; }

            virtual void setData(void* data, uint32_t size) override;

            virtual void bind(uint32_t slot = 0) const override;

            virtual bool operator==(const Texture& other) const override {
                return this->rendererID == ((OpenGLTexture&)other).rendererID;
            }
    };

}

#endif // OPENGL_TEXTURE_H
