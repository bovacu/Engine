#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace engine {

    OpenGLTexture::OpenGLTexture(uint32_t _width, uint32_t _height)
            : width(_width), height(_height) {
        this->internalFormat = GL_RGBA8;
        this->dataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &this->rendererID);
        glTextureStorage2D(this->rendererID, 1, this->internalFormat, this->width, this->height);

        glTextureParameteri(this->rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
        glTextureStorage2D(this->rendererID, 1, _internalFormat, this->width, this->height);

        glTextureParameteri(this->rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, _data);

        stbi_image_free(_data);
    }

    OpenGLTexture::~OpenGLTexture() {
        glDeleteTextures(1, &this->rendererID);
    }

    void OpenGLTexture::setData(void* _data, uint32_t _size) {
        uint32_t _bpp = this->dataFormat == GL_RGBA ? 4 : 3;
        ENGINE_CORE_ASSERT(_size == this->width * this->height * _bpp, "Data must be entire texture!");
        glTextureSubImage2D(this->rendererID, 0, 0, 0, this->width, this->height, this->dataFormat, GL_UNSIGNED_BYTE, _data);
    }

    void OpenGLTexture::bind(uint32_t _slot) const {
        glBindTextureUnit(_slot, this->rendererID);
    }

}