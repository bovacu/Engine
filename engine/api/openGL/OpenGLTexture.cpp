#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace engine {

    OpenGLTexture::OpenGLTexture(uint32_t _width, uint32_t _height, bool _useAlpha)
            : width(_width), height(_height) {
        this->bpp = _useAlpha ? 4 : 3;
        this->internalFormat = _useAlpha ? GL_RGBA8 : GL_RGB8;
        this->dataFormat = _useAlpha ? GL_RGBA : GL_RGB;

        glCreateTextures(GL_TEXTURE_2D, 1, &this->rendererID);
        glTextureStorage2D(this->rendererID, 1, this->internalFormat, this->width, this->height);

        glTextureParameteri(this->rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        this->bufferData = new GLubyte[_width * _height * (_useAlpha ? 4 : 3)];
    }

    OpenGLTexture::OpenGLTexture(const std::string& _path)
            : path(_path) {

        int _width, _height, _channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* _data = nullptr;

        {
//            ENGINE_PROFILE_SCOPE("stbi_load - OpenGLTexture::OpenGLTexture(const std:string&)");
            _data = stbi_load(_path.c_str(), &_width, &_height, &_channels, 0);
        }

        ENGINE_CORE_ASSERT(_data, "Failed to load image!");
        this->width = _width;
        this->height = _height;
        this->bpp = _channels;

        GLenum _internalFormat = 0, _dataFormat = 0;
        if (_channels == 4) {
            _internalFormat = GL_RGBA8;
            _dataFormat = GL_RGBA;
        } else if (_channels == 3) {
            _internalFormat = GL_RGB8;
            _dataFormat = GL_RGB;
        }

        this->internalFormat = _internalFormat;
        this->dataFormat = _dataFormat;

        ENGINE_CORE_ASSERT(_internalFormat & _dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &this->rendererID);
        glTextureStorage2D(this->rendererID, 1, this->internalFormat, this->width, this->height);

        glTextureParameteri(this->rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height, this->dataFormat, GL_UNSIGNED_BYTE, _data);

        stbi_image_free(_data);
    }

    OpenGLTexture::~OpenGLTexture() {
        glDeleteTextures(1, &this->rendererID);
//        delete this->bufferData;
    }

    void OpenGLTexture::setData(void* _data, uint32_t _size) {
        uint32_t _bpp = this->dataFormat == GL_RGBA ? 4 : 3;
        ENGINE_CORE_ASSERT(_size == this->width * this->height * _bpp, "Data must be entire texture!");
        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height, this->dataFormat, GL_UNSIGNED_BYTE, _data);
    }

    void OpenGLTexture::bind(uint32_t _slot) const {
        glBindTextureUnit(_slot, this->rendererID);
    }

    bool OpenGLTexture::usesAlpha() const {
        return this->bpp == 4;
    }

    void OpenGLTexture::setPixel(int _x, int _y, unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
        int _pixel = this->bpp * _x + (this->bpp * (int)this->width * _y);
        this->bufferData[_pixel] = _r;
        this->bufferData[_pixel + 1] = _g;
        this->bufferData[_pixel + 2] = _b;
        if(this->bpp == 4) this->bufferData[_pixel + 3] = _a;
    }

    void OpenGLTexture::setPixel(int _x, int _y, const Color& _color) {
        this->setPixel(_x, _y, _color.r, _color.g, _color.b, _color.a);
    }

    Color OpenGLTexture::getPixel(int _x, int _y) const {
        Color _color;
        int _pixel = this->bpp * _x + (this->bpp * (int)this->width * _y);
        _color.r = this->bufferData[_pixel];
        _color.g = this->bufferData[_pixel + 1];
        _color.b = this->bufferData[_pixel + 2];
        if(usesAlpha()) _color.a = this->bufferData[_pixel + 3];
        return _color;
    }

    void OpenGLTexture::updateTexture() {
        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height, this->dataFormat, GL_UNSIGNED_BYTE, this->bufferData);
    }

    bool OpenGLTexture::LoadTextureFromFile(const char* filename, unsigned int* out_texture, int* out_width, int* out_height) {
        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return false;

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        return true;
    }

    //--------------------------------

    OpenGLImGuiTexture::OpenGLImGuiTexture(const char* _filePath) {
        unsigned char* image_data = stbi_load(_filePath, &this->width, &this->height, NULL, 4);
        if (image_data == NULL)
            ENGINE_CORE_ASSERT(false, "Couldn't load ImGui Texture");

        // Create a OpenGL texture identifier
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);
    }

    OpenGLImGuiTexture::~OpenGLImGuiTexture() {
        glDeleteTextures(1, &this->texture);
    }
}