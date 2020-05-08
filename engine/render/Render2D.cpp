#include "pch.h"
#include "Render2D.h"

#include "engine/render/elements/VertexArray.h"
#include "engine/render/elements/Shader.h"
#include "engine/render/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
        float tilingFactor;
    };

    struct Render2DData {
        static const uint32_t maxQuads = 20000;
        static const uint32_t maxVertices = maxQuads * 4;
        static const uint32_t maxIndices = maxQuads * 6;
        static const uint32_t maxTextureSlots = 32; // TODO: RenderCaps

        std::shared_ptr<VertexArray> quadVertexArray;
        std::shared_ptr<VertexBuffer> quadVertexBuffer;
        std::shared_ptr<Shader> textureShader;
        std::shared_ptr<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        std::array<std::shared_ptr<Texture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1; // 0 = white texture

        glm::vec4 quadVertexPositions[4];

        Render2D::Statistics stats;
    };

    static Render2DData data;

    void Render2D::init() {
        data.quadVertexArray = VertexArray::create();

        data.quadVertexBuffer = VertexBuffer::create(data.maxVertices * sizeof(QuadVertex));
        data.quadVertexBuffer->setLayout({

            { ShaderDataType::Float3, "a_Position"      },
            { ShaderDataType::Float4, "a_Color"         },
            { ShaderDataType::Float2, "a_TexCoord"      },
            { ShaderDataType::Float, "a_TexIndex"       },
            { ShaderDataType::Float, "a_TilingFactor"   }

        });

        data.quadVertexArray->addVertexBuffer(data.quadVertexBuffer);

        data.quadVertexBufferBase = new QuadVertex[data.maxVertices];

        uint32_t* _quadIndices = new uint32_t[data.maxIndices];

        uint32_t _offset = 0;
        for (uint32_t i = 0; i < data.maxIndices; i += 6) {
            _quadIndices[i + 0] = _offset + 0;
            _quadIndices[i + 1] = _offset + 1;
            _quadIndices[i + 2] = _offset + 2;

            _quadIndices[i + 3] = _offset + 2;
            _quadIndices[i + 4] = _offset + 3;
            _quadIndices[i + 5] = _offset + 0;

            _offset += 4;
        }

        std::shared_ptr<IndexBuffer> _quadIB = IndexBuffer::create(_quadIndices, data.maxIndices);
        data.quadVertexArray->setIndexBuffer(_quadIB);
        delete[] _quadIndices;

        data.whiteTexture = Texture2D::create(1, 1);
        uint32_t _whiteTextureData = 0xffffffff;
        data.whiteTexture->setData(&_whiteTextureData, sizeof(uint32_t));

        int32_t _samplers[data.maxTextureSlots];
        for (uint32_t i = 0; i < data.maxTextureSlots; i++)
            _samplers[i] = i;

        data.textureShader = Shader::create(DEFAULT_SHADER_PATH);
        data.textureShader->bind();
        data.textureShader->setIntArray("u_Textures", _samplers, data.maxTextureSlots);

        // Set all texture slots to 0
        data.textureSlots[0] = data.whiteTexture;

        data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Render2D::shutdown() {

    }

    void Render2D::beginRender(const OrthographicCamera& camera) {
        data.textureShader->bind();
        data.textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

        data.quadIndexCount = 0;
        data.quadVertexBufferPtr = data.quadVertexBufferBase;

        data.textureSlotIndex = 1;
    }

    void Render2D::endRender() {
        uint32_t dataSize = (uint32_t)( (uint8_t*)data.quadVertexBufferPtr - (uint8_t*)data.quadVertexBufferBase );
        data.quadVertexBuffer->setData(data.quadVertexBufferBase, dataSize);
        Render2D::flush();
    }

    void Render2D::flush() {
        // Bind textures
        for (uint32_t i = 0; i < data.textureSlotIndex; i++)
            data.textureSlots[i]->bind(i);

        RenderCommand::drawIndexed(data.quadVertexArray, data.quadIndexCount);
        data.stats.drawCalls++;
    }

    void Render2D::flushAndReset() {
        Render2D::endRender();

        data.quadIndexCount = 0;
        data.quadVertexBufferPtr = data.quadVertexBufferBase;

        data.textureSlotIndex = 1;
    }

    void Render2D::drawRect(const Vec2f& _position, const Size& _size, const Color& _color) {
        drawRect({_position.x, _position.y, 0.0f}, {_size.width, _size.height}, {_color.r, _color.g, _color.b, _color.a});
    }

    void Render2D::drawRect(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color) {

        constexpr size_t    _quadVertexCount = 4;
        const float         _textureIndex = 0.0f; // White Texture
        constexpr glm::vec2 _textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float         _tilingFactor = 1.0f;

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
                              * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
            data.quadVertexBufferPtr->color = _color;
            data.quadVertexBufferPtr->texCoord = _textureCoords[_i];
            data.quadVertexBufferPtr->texIndex = _textureIndex;
            data.quadVertexBufferPtr->tilingFactor = _tilingFactor;
            data.quadVertexBufferPtr++;
        }

        data.quadIndexCount += 6;

        data.stats.quadCount++;
    }

    void Render2D::drawRect(const Vec2f& _position, const Size& _size, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        Render2D::drawRect({_position.x, _position.y, 0.0f}, {_size.width, _size.height}, _texture, _tilingFactor, _tintColor);
    }

    void Render2D::drawRect(const glm::vec3& _position, const glm::vec2& _size, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        constexpr size_t _quadVertexCount = 4;
        constexpr glm::vec4 _color = { 1.0f, 1.0f, 1.0f, 1.0f };
        constexpr glm::vec2 _textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        float _textureIndex = 0.0f;
        for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
            if (*data.textureSlots[i].get() == *_texture.get()) {
                _textureIndex = (float)i;
                break;
            }
        }

        if (_textureIndex == 0.0f) {
            if (data.textureSlotIndex >= Render2DData::maxTextureSlots)
                Render2D::flushAndReset();

            _textureIndex = (float)data.textureSlotIndex;
            data.textureSlots[data.textureSlotIndex] = _texture;
            data.textureSlotIndex++;
        }

        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
                              * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
            data.quadVertexBufferPtr->color = _color;
            data.quadVertexBufferPtr->texCoord = _textureCoords[_i];
            data.quadVertexBufferPtr->texIndex = _textureIndex;
            data.quadVertexBufferPtr->tilingFactor = _tilingFactor;
            data.quadVertexBufferPtr++;
        }

        data.quadIndexCount += 6;
        data.stats.quadCount++;
    }

    void Render2D::drawRotatedRect(const Vec2f& _position, const Size& _size, float _rotation, const Color& _color) {
        Render2D::drawRotatedRect({_position.x, _position.y, 0.0f}, {_size.width, _size.height}, _rotation, {_color.r, _color.g, _color.b, _color.a});
    }

    void Render2D::drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float rotation, const glm::vec4& _color) {
        constexpr size_t        _quadVertexCount = 4;
        const float             _textureIndex = 0.0f; // White Texture
        constexpr glm::vec2     _textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float             _tilingFactor = 1.0f;

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
                              * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
            data.quadVertexBufferPtr->color = _color;
            data.quadVertexBufferPtr->texCoord = _textureCoords[_i];
            data.quadVertexBufferPtr->texIndex = _textureIndex;
            data.quadVertexBufferPtr->tilingFactor = _tilingFactor;
            data.quadVertexBufferPtr++;
        }

        data.quadIndexCount += 6;

        data.stats.quadCount++;
    }

    void Render2D::drawRotatedRect(const Vec2f& _position, const Size& _size, float _rotation, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        Render2D::drawRotatedRect({_position.x, _position.y, 0.0f}, {_size.width, _size.height}, _rotation, _texture, _tilingFactor,
                                  _tintColor);
    }

    void Render2D::drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float rotation, const std::shared_ptr<Texture2D>& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        constexpr size_t        _quadVertexCount = 4;
        constexpr glm::vec4     _color = { 1.0f, 1.0f, 1.0f, 1.0f };
        constexpr glm::vec2     _textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        float _textureIndex = 0.0f;
        for (uint32_t _i = 1; _i < data.textureSlotIndex; _i++) {
            if (*data.textureSlots[_i].get() == *_texture.get()) {
                _textureIndex = (float)_i;
                break;
            }
        }

        if (_textureIndex == 0.0f) {
            if (data.textureSlotIndex >= Render2DData::maxTextureSlots)
                Render2D::flushAndReset();

            _textureIndex = (float)data.textureSlotIndex;
            data.textureSlots[data.textureSlotIndex] = _texture;
            data.textureSlotIndex++;
        }

        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f })
                              * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
            data.quadVertexBufferPtr->color = _color;
            data.quadVertexBufferPtr->texCoord = _textureCoords[_i];
            data.quadVertexBufferPtr->texIndex = _textureIndex;
            data.quadVertexBufferPtr->tilingFactor = _tilingFactor;
            data.quadVertexBufferPtr++;
        }

        data.quadIndexCount += 6;

        data.stats.quadCount++;
    }

    void Render2D::resetStats() {
        memset(&data.stats, 0, sizeof(Statistics));
    }

    Render2D::Statistics Render2D::getStats() {
        return data.stats;
    }

    void Render2D::draw(const Ptr::GameObjectPtr& _gameObject, float _tilingFactor, const glm::vec4& _tintColor) {
//        auto _sprite = _gameObject->getComponentOfType<Sprite>();
//        ENGINE_CORE_ASSERT(_sprite, "CAN'T DRAW A GAME OBJECT WITHOUT A SPRITE AS COMPONENT");
//
//        if(_sprite->getRotation() > 0)
//            Render2D::drawRotated(_sprite, _sprite->getRotation(), _tilingFactor, _tintColor);
//
//        constexpr size_t        _quadVertexCount = 4;
//        const float             _textureIndex = 0.0f; // White Texture
//        const Vec2f*            _textureCoords = _sprite->getTexture()->getTextureCoords();
//        constexpr glm::vec4 _color = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//        if (data.quadIndexCount >= Render2DData::maxIndices)
//            Render2D::flushAndReset();
//
//        glm::vec3 _position = {_sprite->getPosition().x, _sprite->getPosition().y, 0.0f};
//        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
//                               * glm::scale(glm::mat4(1.0f), { _sprite->getScale().width, _sprite->getScale().height, 1.0f });
//
//        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
//            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
//            data.quadVertexBufferPtr->color = _color;
//            data.quadVertexBufferPtr->texCoord = { _textureCoords[_i].x, _textureCoords[_i].y };
//            data.quadVertexBufferPtr->texIndex = _textureIndex;
//            data.quadVertexBufferPtr->tilingFactor = _tilingFactor;
//            data.quadVertexBufferPtr++;
//        }
//
//        data.quadIndexCount += 6;
//        data.stats.quadCount++;
    }

    void Render2D::drawRotated(const Ptr::SpritePtr& _sprite, float _rotation, float _tilingFactor, const glm::vec4& _tintColor) {
        LOG_CRITICAL_CORE("DRAW GAME OBJECT ROTATED NOT IMPLEMENTED YET");
    }

}