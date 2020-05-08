#pragma once

#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

/*  =================================================================================================
    =                                       BOX COLLIDER                                            =
    =================================================================================================
    = (*) To construct a BoxCollider with vertices, define them like this:                          =
    =                                                                                               =
    =       v0 *---------* v3                                                                       =
    =          |         |                                                                          =
    =          |    *    |                                                                          =
    =          |         |                                                                          =                                                                          =
    =       v1 *---------* v2                                                                       =
    =                                                                                               =
    = (*) To construct with center and size, set the center to desired point and                    =
    =     width and hight is relative to the center.                                                =
    =                                                                                               =
    = (*) BoxCollider can be either:                                                                =
    =           - AABB     -> Axis Aligned Bounding Box, no rotation over coordinate system axis    =
    =           - Not AABB -> The rectangle is rotated by some alpha value                          =
    =                                                                                               =
    = (*) Collision detection modes:                                                                =
    =           - If it's AABB vs AABB                                                              =
    =               - AABB collision detection is applied                                           =
    =           - If it's AABB vs Polygon                                                           =
    =               - SAT collision detection is applied                                            =
    =           - If it's AABB vs Circle                                                            =
    =               - AABB modified for circles                                                     =
    =           - If box collider is not AABB                                                       =
    =               - Always SAT no matter the other collider                                       =
    =                                                                                               =                               
    =================================================================================================
*/

namespace engine {

    class BoxCollider : public Collider {
        public:
            BoxCollider(GameObject* _gameObject, const Vec2f& _center, const Size& _size, bool _isGhost = false);
            static BoxCollider* create(GameObject* _gameObject, const Vec2f& _center, const Size& _size, bool _isGhost = false);

        protected:
            virtual Mtv isColliding(Collider& _collider, float _push = 0.0f) override;

        private:
            float top()     { return Collider::center.y - Collider::size.height / 2; }
            float bottom()  { return Collider::center.y + Collider::size.height / 2; }
            float left()    { return Collider::center.x - Collider::size.width  / 2; }
            float right()   { return Collider::center.x + Collider::size.width  / 2; }
            
        private:            
            Mtv collidingWithAABB(Collider& _collider, float _push);
            Mtv collidingWithCircle(Collider& _collider, float _push);
    };

    namespace Ptr {
        typedef std::shared_ptr<engine::BoxCollider> BoxColliderPtr;
    }
}

#endif //BOX_COLLIDER_H