#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

namespace engine {

    class Texture;
    typedef  std::shared_ptr<Texture> TexturePtr;

    class Texture {
        public:
            virtual ~Texture()                                                                                                      = default;

            [[nodiscard]] virtual uint32_t getWidth() const                                                                         = 0;
            [[nodiscard]] virtual uint32_t getHeight() const                                                                        = 0;

            virtual void setData(void* _data, uint32_t _size)                                                                       = 0;

            virtual void bind(uint32_t _slot = 0) const                                                                             = 0;
            [[nodiscard]] virtual bool usesAlpha() const                                                                            = 0;

            virtual void setPixel(int _x, int _y, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)     = 0;
            virtual void setPixel(int _x, int _y, const Color& _color)                                                              = 0;
            [[nodiscard]] virtual Color getPixel(int _x, int _y) const                                                              = 0;

            [[nodiscard]] virtual unsigned char* getBuffer() const                                                                  = 0;
            virtual void setBuffer(unsigned char* _buffer)                                                                          = 0;

            [[nodiscard]] virtual int getBPP() const                                                                                = 0;

            virtual void updateTexture()                                                                                            = 0;

            virtual bool operator==(const Texture& _other) const                                                                    = 0;
    };

    class Texture2D;
    typedef std::shared_ptr<Texture2D> Texture2DPtr;

    class Texture2D : public Texture {
        public:
            static std::shared_ptr<Texture2D> create(uint32_t _width, uint32_t _height, bool _useAlpha = false);
            static std::shared_ptr<Texture2D> create(const std::string& _path);
    };

}




#endif // TEXTURE_H
