#pragma once

#ifndef POLYGON_COLLIDER_H
#define POLYGON_COLLIDER_H

#include "Collider.h"

/*  =================================================================================================
    =                                    POLYGON COLLIDER                                           =
    =================================================================================================
    = (*) To construct a PolygonCollider just pass the vertices with RELATIVE position to           = 
    =     the center.                                                                               =
    =                                                                                               =
    =              v0                                                                               =
    =              *                                                                                =
    =            /   \                                                                              =
    =         v1*  *  *v4                                                                           =
    =           |     |                                                                             =
    =         v2*-----*v3                                                                           =
    =                                                                                               =
    = (*) If center is not provided, it will be automatically computed (less efficent) and in       =
    =     this case, coordinates of vertices are in WORLD position.                                 =
    =                                                                                               =
    = (*) Collision detection modes:                                                                =
    =           -Collision detection is always SAT, modified in case of CircleCollider              =
    =                                                                                               =                               
    =================================================================================================
*/

namespace engine {

    class PolygonCollider : public Collider {
        public:
            PolygonCollider(GameObject* _gameObject, const Vec2f& _center, const std::vector<Vec2f>& _vertices, bool _isGhost = false);
            PolygonCollider(GameObject* _gameObject, const std::vector<Vec2f>& _vertices, bool _isGhost = false);
            virtual Mtv isColliding(Collider& _collider, float _push = 0.0f) override;

        private:
            Mtv collidingWithCircle(Collider& _collider, float _push);
    };

    namespace Ptr {
        typedef std::shared_ptr<engine::PolygonCollider> PolygonColliderPtr;
    }
}

#endif //POLYGON_COLLIDER_H