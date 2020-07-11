#include "pch.h"
#include "Render2D.h"

#include <engine/render/elements/VertexArray.h>
#include <engine/render/elements/Shader.h>
#include <engine/render/RenderCommand.h>

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    RenderingType Render2D::renderingType = RenderingType::QUADS;

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
        float tilingFactor;
    };
    struct PixelVertex {
        glm::vec3 position;
        glm::vec4 color;
    };

    struct Render2DData {
        static int              numberOfVertices;
        static uint32_t         maxQuads;
        static uint32_t         maxVertices;
        static uint32_t         maxIndices;
        static const uint32_t   maxTextureSlots = 32; // TODO: RenderCaps

        VertexArrayPtr  quadVertexArray;
        VertexBufferPtr quadVertexBuffer;
        ShaderPtr       textureShader;
        Texture2DPtr    whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        std::array<Texture2DPtr, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1; // 0 = white texture

        glm::vec4 quadVertexPositions[4];

        Render2D::Statistics stats;
    };

    int Render2DData::numberOfVertices = 4;
    uint32_t Render2DData::maxQuads = 100000;
    uint32_t Render2DData::maxVertices = Render2DData::maxQuads * Render2DData::numberOfVertices;
    uint32_t Render2DData::maxIndices = Render2DData::maxQuads * (Render2DData::numberOfVertices + 2);

    static Render2DData data;

    void Render2D::init() {
        data.quadVertexArray = VertexArray::create();

        data.quadVertexBuffer = VertexBuffer::create(engine::Render2DData::maxVertices * sizeof(QuadVertex));
        data.quadVertexBuffer->setLayout({

            { ShaderDataType::Float3, "position_lay"      },
            { ShaderDataType::Float4, "color_lay"         },
            { ShaderDataType::Float2, "texCoord_lay"      },
            { ShaderDataType::Float,  "texIndex_lay"      },
            { ShaderDataType::Float,  "tilingFactor_lay"  }

        });

        data.quadVertexArray->addVertexBuffer(data.quadVertexBuffer);
        data.quadVertexBufferBase = new QuadVertex[engine::Render2DData::maxVertices];

        auto* _quadIndices = new uint32_t[engine::Render2DData::maxIndices];

        uint32_t _offset = 0;
        for (uint32_t i = 0; i < engine::Render2DData::maxIndices; i += 6) {
            _quadIndices[i + 0] = _offset + 0;
            _quadIndices[i + 1] = _offset + 1;
            _quadIndices[i + 2] = _offset + 2;

            _quadIndices[i + 3] = _offset + 2;
            _quadIndices[i + 4] = _offset + 3;
            _quadIndices[i + 5] = _offset + 0;

            _offset += 4;
        }

        IndexBufferPtr _quadIB = IndexBuffer::create(_quadIndices, engine::Render2DData::maxIndices);
        data.quadVertexArray->setIndexBuffer(_quadIB);
        delete[] _quadIndices;

        data.whiteTexture = Texture2D::create(1, 1, true);
        uint32_t _whiteTextureData = 0xffffffff;
        data.whiteTexture->setData(&_whiteTextureData, sizeof(uint32_t));

        int32_t _samplers[engine::Render2DData::maxTextureSlots];
        for (uint32_t i = 0; i < engine::Render2DData::maxTextureSlots; i++)
            _samplers[i] = i;

        data.textureShader = Shader::create(DEFAULT_SHADER_PATH);
        data.textureShader->bind();
        data.textureShader->setIntArray("textures_uniform", _samplers, engine::Render2DData::maxTextureSlots);

        // Set all texture slots to 0
        data.textureSlots[0] = data.whiteTexture;

        data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }
    void Render2D::shutdown() {
        delete [] data.quadVertexBufferBase;
    }

    void Render2D::beginDraw(const OrthographicCamera& camera, const RenderingType& _type) {
        data.textureShader->bind();
        data.textureShader->setMat4("viewProjection_uniform", camera.getViewProjectionMatrix());

        data.quadIndexCount = 0;
        data.quadVertexBufferPtr = data.quadVertexBufferBase;

        data.textureSlotIndex = 1;
    }
    void Render2D::endDraw() {
        auto dataSize = (uint32_t)( (uint8_t*)data.quadVertexBufferPtr - (uint8_t*)data.quadVertexBufferBase );
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
        Render2D::endDraw();

        data.quadIndexCount = 0;
        data.quadVertexBufferPtr = data.quadVertexBufferBase;
        data.textureSlotIndex = 1;
    }

    void Render2D::drawRect(const Vec2f& _position, const Size& _size, const Color& _color) {
        float _adapter = OrthographicCamera::usingAspectRatio ? ASPECT_RATIO_PIXEL : 1;
        drawRect({_position.x * _adapter, _position.y * _adapter, 0.0f}, {_size.width * _adapter, _size.height * _adapter}, {_color.r, _color.g, _color.b, _color.a});
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

    void Render2D::drawTexture(const Vec2f& _position, const Size& _size, const Texture2DPtr& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        float _adapter = OrthographicCamera::usingAspectRatio ? ASPECT_RATIO_PIXEL : 1;
        Render2D::drawTexture({_position.x * _adapter, _position.y * _adapter, 0.0f},
                              {_size.width * _adapter, _size.height * _adapter}, _texture,
                              _tilingFactor, _tintColor);
    }
    void Render2D::drawTexture(const glm::vec3& _position, const glm::vec2& _size, const Texture2DPtr& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        constexpr size_t _quadVertexCount = 4;
        constexpr glm::vec4 _color = { 1.0f, 1.0f, 1.0f, 1.0f };
        constexpr glm::vec2 _textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        float _textureIndex = 0.0f;
        for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
            if (*data.textureSlots[i] == *_texture.get()) {
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
        float _adapter = OrthographicCamera::usingAspectRatio ? ASPECT_RATIO_PIXEL : 1;
        Render2D::drawRotatedRect({_position.x * _adapter, _position.y * _adapter, 0.0f},
                {_size.width * _adapter, _size.height * _adapter}, _rotation, {_color.r, _color.g, _color.b, _color.a});
    }
    void Render2D::drawRotatedRect(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color) {
        constexpr size_t        _quadVertexCount = 4;
        const float             _textureIndex = 0.0f; // White Texture
        constexpr glm::vec2     _textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float             _tilingFactor = 1.0f;

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), { 0.0f, 0.0f, 1.0f })
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

    void Render2D::drawRotatedTexture(const Vec2f& _position, const Size& _size, float rotation, const Texture2DPtr& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        float _adapter = OrthographicCamera::usingAspectRatio ? ASPECT_RATIO_PIXEL : 1;
        Render2D::drawRotatedTexture({_position.x * _adapter, _position.y * _adapter, 0.0f},
                                     {_size.width * _adapter, _size.height * _adapter}, rotation, _texture,
                                     _tilingFactor, _tintColor);
    }
    void Render2D::drawRotatedTexture(const glm::vec3& _position, const glm::vec2& _size, float rotation, const Texture2DPtr& _texture, float _tilingFactor, const glm::vec4& _tintColor) {
        constexpr size_t        _quadVertexCount = 4;
        constexpr glm::vec4     _color = { 1.0f, 1.0f, 1.0f, 1.0f };
        constexpr glm::vec2     _textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        float _textureIndex = 0.0f;
        for (uint32_t _i = 1; _i < data.textureSlotIndex; _i++) {
            if (*data.textureSlots[_i] == *_texture) {
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

    void Render2D::draw(const GameObjectPtr& _gameObject, float _tilingFactor, const glm::vec4& _tintColor) {
        auto _sprite = _gameObject->getComponentOfType<Sprite>();
        ENGINE_CORE_ASSERT(_sprite, "CAN'T DRAW A GAME OBJECT WITHOUT A SPRITE AS COMPONENT");

        if(_sprite->getRotation() != 0) {
            Render2D::drawRotated(_sprite, _sprite->getRotation(), _tilingFactor, _tintColor);
            return;
        }

        constexpr size_t        _quadVertexCount = 4;
        const Vec2f*            _textureCoords = _sprite->getTexture()->getTextureCoords();
        constexpr glm::vec4 _color = { 1.0f, 1.0f, 1.0f, 1.0f };
        const Texture2DPtr _texture = _sprite->getTexture()->getTexture();

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        float _textureIndex = 0.0f;
        for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
            if (*data.textureSlots[i] == *_texture.get()) {
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

        float _adapter = OrthographicCamera::usingAspectRatio ? ASPECT_RATIO_PIXEL : 1;
        glm::vec3 _position = {_sprite->getPosition().x * _adapter, _sprite->getPosition().y * _adapter, 0.0f};
        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
                               * glm::scale(glm::mat4(1.0f), { _sprite->getScale().width * _sprite->getSize().width * _adapter,
                               _sprite->getScale().height * _sprite->getSize().height * _adapter, 1.0f });

        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
            data.quadVertexBufferPtr->color = _color;
            data.quadVertexBufferPtr->texCoord = { _textureCoords[_i].x, _textureCoords[_i].y };
            data.quadVertexBufferPtr->texIndex = _textureIndex;
            data.quadVertexBufferPtr->tilingFactor = _tilingFactor;
            data.quadVertexBufferPtr++;
        }

        data.quadIndexCount += 6;
        data.stats.quadCount++;
    }
    void Render2D::drawRotated(const SpritePtr& _sprite, float _rotation, float _tilingFactor, const glm::vec4& _tintColor) {
        constexpr size_t        _quadVertexCount = 4;
        const Vec2f*            _textureCoords = _sprite->getTexture()->getTextureCoords();
        constexpr glm::vec4 _color = { 1.0f, 1.0f, 1.0f, 1.0f };
        const Texture2DPtr _texture = _sprite->getTexture()->getTexture();

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        float _textureIndex = 0.0f;
        for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
            if (*data.textureSlots[i] == *_texture.get()) {
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

        float _adapter = OrthographicCamera::usingAspectRatio ? ASPECT_RATIO_PIXEL : 1;
        glm::vec3 _position = {_sprite->getPosition().x * _adapter, _sprite->getPosition().y * _adapter, 0.0f};
        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position)
                               * glm::rotate(glm::mat4(1.0f), glm::radians(_sprite->getRotation()), {0.0f, 0.0f, 1.0f })
                               * glm::scale(glm::mat4(1.0f), { _sprite->getScale().width * _sprite->getSize().width * _adapter,
                                                               _sprite->getScale().height * _sprite->getSize().height * _adapter, 1.0f });

        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
            data.quadVertexBufferPtr->color = _color;
            data.quadVertexBufferPtr->texCoord = { _textureCoords[_i].x, _textureCoords[_i].y };
            data.quadVertexBufferPtr->texIndex = _textureIndex;
            data.quadVertexBufferPtr->tilingFactor = _tilingFactor;
            data.quadVertexBufferPtr++;
        }

        data.quadIndexCount += 6;
        data.stats.quadCount++;
    }

    void Render2D::drawTexture(const Vec2f& _position, const Size& _size, const TextureRegionPtr& _textureRegion, float _rotation,const Color& _tintColor) {
        drawTexture(glm::vec3{_position.x, _position.y, 1.0f}, glm::vec2{_size.width, _size.height}, _textureRegion, _rotation, glm::vec4{_tintColor.r, _tintColor.g, _tintColor.b, _tintColor.a});
    }
    void Render2D::drawTexture(const glm::vec3& _position, const glm::vec2& _size, const TextureRegionPtr& _textureRegion, float _rotation, const glm::vec4& _tintColor) {
        constexpr size_t _quadVertexCount = 4;
        constexpr glm::vec4 _color = { 1.0f, 1.0f, 1.0f, 1.0f };
        auto* _coords = _textureRegion->getTextureCoords();
        const glm::vec2 _textureCoords[] = { { _coords[0].x, _coords[0].y }, { _coords[1].x, _coords[1].y }, { _coords[2].x, _coords[2].y }, { _coords[3].x, _coords[3].y } };
        const Texture2DPtr _texture = _textureRegion->getTexture();

        if (data.quadIndexCount >= Render2DData::maxIndices)
            Render2D::flushAndReset();

        float _textureIndex = 0.0f;
        for (uint32_t i = 1; i < data.textureSlotIndex; i++) {
            if (*data.textureSlots[i] == *_texture.get()) {
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

        glm::mat4 _transform = glm::translate(glm::mat4(1.0f), _position);
        if(_rotation > 0.0f)
            _transform *= glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), {0.0f, 0.0f, 1.0f });
        _transform *= glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        for (size_t _i = 0; _i < _quadVertexCount; _i++) {
            data.quadVertexBufferPtr->position = _transform * data.quadVertexPositions[_i];
            data.quadVertexBufferPtr->color = _color;
            data.quadVertexBufferPtr->texCoord = _textureCoords[_i];
            data.quadVertexBufferPtr->texIndex = _textureIndex;
            data.quadVertexBufferPtr->tilingFactor = 1.0f;
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


}