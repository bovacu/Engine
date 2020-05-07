#pragma once

#ifndef COLLIDER_H
#define COLLIDER_H
#define _USE_MATH_DEFINES

#include <limits>
#include <functional>
#include <math.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <iostream>
#include "engine/components/transform/Transform2D.h"
#include "engine/components/Component.h"

#define BIG_NUM 9999999
#define TO_RAD(alpha) ((alpha) * M_PI / 180.0)

/*  =================================================================================================
    =                                         COLLIDER                                              =
    =================================================================================================
    = (*) This is the base class of all type of colliders of the engine                             =
    =                                                                                               =
    = (*) The enum is to identify the type of collider for the moment of checking collisions        =
    =                                                                                               =
    = (*) Projection is a struct that represents the projection of a vertex onto a given axis       =
    =     for SAT calculations.                                                                     =
    =                                                                                               =
    = (*) Edge is the representation of a side of a polygon v0*---------*v1 for SAT calculations    =
    =                                                                                               =
    = (*) Mtv contains, for each collision, if the collision exists and in that case, the amount    =
    =     the polygon has to translate outside the other polygon to generate the collision          =
    =                                                                                               =
    = (*) onEnter, onExit and onCollision are pointers to functions that will be defined by the     =
    =     final user and set to the collider via the appropiated setter:                            =
    =           -OnCollisionEnter: code to execute when two polygons collides for the first time.   =
    =           -OnCollision: code to execute while two polygons are colliding.                     =
    =           -OnCollisionExit: code to execute once two polygons that were colliding, aren't     =
    =                             any more.                                                         =
    =                                                                                               =
    = (*) isGhost prevents the push out of the polygons when colliding, but onEnter, onCollision    =
    =     and onExit will be executed if defined                                                    =
    =                                                                                               =
    =================================================================================================
    =                                                                                               =
    = (*) A smart-shared pointer is defined in namespace Ptr for every Collider child so there can  =
    =     be inheritance through the Collider functions that take Collider& as parameter:           =
    =           - BoxColliderPtr, PolygonColiderPtr, CircleColliderPtr                              =
    =                                                                                               =
    ================================================================================================= 
*/

namespace engine {

    struct Edge {
        Vec2f v0, v1;
        Vec2f vector;
        float ilenthsquared;

        Edge(const Vec2f& _v0, const Vec2f& _v1) : v0(_v0), v1(_v1), vector(_v1.x - _v0.x, _v1.y - _v0.y) { 
            float _length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
            ilenthsquared = 1 / (_length * _length);
        }
    };

    struct Mtv {
        Vec2f translation;
        bool collision;
        Mtv(const Vec2f& _translation = {0, 0}, bool _isCollision = false) : translation(_translation), collision(_isCollision) {  }
    };

    class Collider;
    struct Manifold {
        Mtv mtv;
        Collider* otherCollider;

        Manifold(Collider* _otherCollider, const Mtv& _mtv) : otherCollider(_otherCollider), mtv(_mtv) {  }
    };

    class Collider : public Component {
        typedef std::function<void (engine::Collider& )> ColliderFunction;

        public:
            bool isGhost;
            Vec2f center;
            Size size;
            std::vector<Vec2f> vertices;
            bool collisionEntered;
            int numberOfVertices;   

            std::vector<Manifold> manifolds; 

        private:
            ColliderFunction onEnter, onCollision, onExit;

        public:
            Collider(GameObject* _gameObject, const ComponentType& _componentType, bool _isGhost = false);
            void update(Transform2D& _transform);
            void update(const Vec2f& _position, float _rotation, const Vec2f& _scale);
            virtual Mtv collides(Collider& _collider, float _push = 0.0f);

            void update(float _dt)          override;
            void fixUpdate(float _fixDt)    override;
            void postUpdate(float _postDt)  override;

        public:    
            virtual void rotate(float _alpha);
            virtual void rotateRadians(float _alpha);
            virtual void setPosition(const Vec2f& _point);
            virtual void setPosition(float _x, float _y);

        public:
            void setOnCollisionEnterFunction(const ColliderFunction& _onEnter)      { this->onEnter = _onEnter; }
            void setOnCollisionFunction(const ColliderFunction& _onCollision)       { this->onCollision = _onCollision; }
            void setOnCollisionExitFunction(const ColliderFunction& _onExit)        { this->onExit = _onExit; }

        protected:
            Mtv collidingWithSat(Collider& _collider, float _push = 0.0f);
            virtual Mtv isColliding(Collider& _collider, float _push = 0.0f) = 0;
            
            Vec2f getCentroid();
            Vec2f getClosestVertexToCircleCenter(std::vector<Vec2f>& _verteces, const Vec2f& _center); 

        private:
            Vec2f getMinimumOfVectors(std::vector<Vec2f>& _vectors);
    };

    namespace Ptr {
        typedef std::shared_ptr<Collider> ColliderPtr;
    }
}

#endif //COLLIDER_H