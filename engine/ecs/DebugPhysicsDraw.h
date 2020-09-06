#pragma once

#ifndef ENGINE_DEBUGPHYSICSDRAW_H
#define ENGINE_DEBUGPHYSICSDRAW_H

#include <box2d/box2d.h>

namespace engine {

    class DebugPhysicsDraw : public b2Draw {
        public:
            void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

            void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

            void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

            void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

    };

}

#endif //ENGINE_DEBUGPHYSICSDRAW_H
