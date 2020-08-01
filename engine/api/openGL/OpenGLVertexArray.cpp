#include "pch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace engine {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType _type) {
        switch (_type) {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
            case ShaderDataType::None:
                break;
        }

        ENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!")
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &this->rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &this->rendererID);
    }

    void OpenGLVertexArray::bind() const {
        glBindVertexArray(this->rendererID);
    }

    void OpenGLVertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const VertexBufferPtr& _vertexBuffer) {
        ENGINE_CORE_ASSERT(_vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!")

        glBindVertexArray(this->rendererID);
        _vertexBuffer->bind();

        const auto& _layout = _vertexBuffer->getLayout();
        for (const auto& _element : _layout) {
            switch (_element.type) {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool : {
                    glEnableVertexAttribArray(this->vertexBufferIndex);
                    glVertexAttribPointer(this->vertexBufferIndex,
                                          _element.getComponentCount(),
                                          ShaderDataTypeToOpenGLBaseType(_element.type),
                                          _element.normalized ? GL_TRUE : GL_FALSE,
                                          _layout.getStride(),
                                          (const void*)_element.offset);
                    this->vertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4 : {
                    uint8_t _count = _element.getComponentCount();
                    for (uint8_t _i = 0; _i < _count; _i++) {
                        glEnableVertexAttribArray(this->vertexBufferIndex);
                        glVertexAttribPointer(this->vertexBufferIndex,
                                              _count,
                                              ShaderDataTypeToOpenGLBaseType(_element.type),
                                              _element.normalized ? GL_TRUE : GL_FALSE,
                                              _layout.getStride(),
                                              (const void*)(sizeof(float) * _count * _i));
                        glVertexAttribDivisor(this->vertexBufferIndex, 1);
                        this->vertexBufferIndex++;
                    }
                    break;
                }
                default:
                    ENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }

        this->vertexBuffers.push_back(_vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const IndexBufferPtr& _indexBuffer) {
        glBindVertexArray(this->rendererID);
        _indexBuffer->bind();

        this->indexBuffer = _indexBuffer;
    }


}