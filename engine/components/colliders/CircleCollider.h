#pragma once

#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

#include "Collider.h"

/*  =================================================================================================
    =                                     CIRCLE COLLIDER                                           =
    =================================================================================================
    = (*) To construct a CircleCollider just pass the center coordinates and the radius:            =
    =                                                                                               =
    = (*) Collision detection modes:                                                                =
    =           - If it's Circle vs Circle                                                          =
    =               - Collision between circles is applied                                          =
    =           - If it's Circle vs Polygon                                                         =
    =               - SAT collision detection is applied                                            =
    =           - If it's AABB vs Circle                                                            =
    =               - AABB modified for circles                                                     =
    =                                                                                               =
    = (*) Notes:                                                                                    =
    =           - As this class extends from Collider, the radius is not stored in a                =
    =             variable called radius, it's stored in size.width                                 =                                
    =================================================================================================
*/

namespace engine {

    class CircleCollider;
    typedef std::shared_ptr<engine::CircleCollider> CircleColliderPtr;

    class CircleCollider : public Collider {
        public:
            CircleCollider(GameObject* _gameObject, const Vec2f& _center, float _radius, bool _isGhost = false);
            static CircleColliderPtr create(GameObject* _gameObject, const Vec2f& _center, float _radius, bool _isGhost = false);

            virtual Mtv isColliding(Collider& _collider, float _push = 0.0f) override;

        private:
            Mtv collidingWithCircle(Collider& _collider, float _push);
            Mtv collidingWithAABB(Collider& _collider, float _push);
            Mtv collidingWithPolygon(Collider& _collider, float _push);
    };
}

#endif //CIRCLE_COLLIDER_H