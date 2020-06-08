#pragma once

#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <engine/render/elements/Texture.h>
#include <glad/glad.h>

namespace engine {

    class OpenGLTexture : public Texture2D {
        private:
            std::string     path;
            uint32_t        width, height;
            uint32_t        rendererID;
            GLenum          internalFormat, dataFormat;
            GLubyte*        bufferData;
            int             bpp;

        public:
            OpenGLTexture(uint32_t _width, uint32_t _height, bool _useAlpha = false);
            explicit OpenGLTexture(const std::string& _path);
            ~OpenGLTexture()                                                                                            override;

            [[nodiscard]] uint32_t getWidth() const                                                                     override { return this->width;  }
            [[nodiscard]] uint32_t getHeight() const                                                                    override { return this->height; }

            void setData(void* data, uint32_t size)                                                                     override;

            void bind(uint32_t slot) const                                                                              override;

            [[nodiscard]] bool usesAlpha() const                                                                        override;

            void setPixel(int _x, int _y, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)       override;
            void setPixel(int _x, int _y, const Color& _color)                                                          override;
            [[nodiscard]] Color getPixel(int _x, int _y) const                                                          override;

            [[nodiscard]] unsigned char* getBuffer() const                                                              override { return this->bufferData; }
            void setBuffer(unsigned char* _buffer)                                                                      override { this->bufferData = _buffer; }

            [[nodiscard]] int getBPP() const                                                                            override { return this->bpp; }

            void updateTexture()                                                                                        override;
            uint32_t getRendererID()                                                                                    override { return this->rendererID; }

            bool operator==(const Texture& other) const                                                                 override { return this->rendererID == ((OpenGLTexture&)other).rendererID; }

            bool LoadTextureFromFile(const char* filename, unsigned int* out_texture, int* out_width, int* out_height)  override;
    };

    class OpenGLImGuiTexture : public ImGuiTexture2D {
        public:
            int width, height;
            unsigned int texture;

            int getWidth()              override { return this->width; }
            int getHeight()             override { return this->height; }
            unsigned int getTexture()   override { return this->texture; }

            OpenGLImGuiTexture(const char* _filePath);
            ~OpenGLImGuiTexture()               override;
    };
}

#endif // OPENGL_TEXTURE_H
