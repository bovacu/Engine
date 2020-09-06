#include "pch.h"
#include "Physics.h"

namespace engine {

    b2World Physics::world(b2Vec2(0, -90.7f));

    void Physics::setGravity(const Vec2f& _gravity) {
        Physics::world.SetGravity(b2Vec2(_gravity.x, _gravity.y));
    }

    Vec2f Physics::getGravity() {
        return {Physics::world.GetGravity().x, Physics::world.GetGravity().y};
    }

    b2World& Physics::getWorld() {
        return Physics::world;
    }

    void Physics::step() {
        Physics::world.Step(1/60.f, 8, 3);
        for ( b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
            LOG_INFO_CORE("y: {0}", b->GetTransform().p.y);
        }
    }
}