#pragma once

#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

/*  =============================================================================================
    =                                     PHYSICS BODY                                          =
    =============================================================================================
    =                                                                                           =
    = (*) This class is in charge of doing all the physics of each gameObject of the scene.     =
    =                                                                                           =
    = (*) This class calculates, movements, gravity, restitution, friction, rotations...        =
    =                                                                                           =
    = (*) Each PhysicsBody can either be, Dynamic, Kinematic or Static:                         =
    =       - Dynamic:   - It's affected by the physics.                                        =
    =                    - Checks collisions with every type of body (if collider(s) present).  =
    =                    - Can be translated and moved by other Dynamics and Kinematics.        =
    =                                                                                           =
    =       - Kinematic: - It's not affected by the physics.                                    =
    =                    - Checks collisions ONLY with Dynamic bodies (if collider(s) present). =
    =                    - Can be translated, but not moved by other bodies.                    =
    =                                                                                           =
    =       - Static:    - It's not affected by the physics.                                    =
    =                    - Checks collisions ONLY with Dynamic bodies (if collider(s) present). =
    =                    - Cannot be neither translated neither moved by other bodies.          =
    =                                                                                           =
    = (*) Methods add___Collider makes easier for the final user to add a collider to a         =
    =     PhysicsBody, making them not having to create the create the shared_pointer.          =  
    =                                                                                           =
    = (*) Each PhysicsBody can have from 0..n colliders, and modify each one as whised.         =                                                                                                                                                                                                                                                                                 
    =                                                                                           =
    =============================================================================================                                                                                           =
    =                               dt = time between each frame                                =
    =                               approach(...) is defined below                              =
    =============================================================================================
    =                                                                                           =
    = (*) Linear movement: MinSpeed = FullSpeed                                                 =
    =       - Used variables: position and velocity                                             =
    =       - We set the desired speed to -> velocity                                           =
    =                                                                                           =
    =       - For dependent frame movement                                                      =
    =           - Operation: [*] position.x += velocity.x                                       =
    =                        [*] position.y += velocity.y                                       =
    =       - For independent frame movement                                                    =
    =           - Operation: [*] position.x += velocity.x * dt                                  =
    =                        [*] position.y += velocity.y * dt                                  =
    =                                                                                           =
    = (*) Velocity movement: Speed augments gradually                                           =
    =       - Used variables: acceleration, velocity and position                               =
    =       - We set desired speed to -> acceleration                                           =
    =                                                                                           =
    =       - For dependent frame movement                                                      =
    =           - Operation: [*] velocity.x += acceleration.x                                   =
    =                        [*] velocity.y += acceleration.y                                   =
    =                                                                                           =
    =                        [*] position.x += velocity.x                                       =
    =                        [*] position.y += velocity.y                                       =
    =       - For independent frame movement                                                    =
    =           - Operation: [*] velocity.x += acceleration.x * dt                              =
    =                        [*] velocity.y += acceleration.y * dt                              =
    =                                                                                           =
    =                        [*] position.x += velocity.x * dt                                  =
    =                        [*] position.y += velocity.y * dt                                  =
    =                                                                                           =
    = (*) Velocity movement with approach: Approachs the next point to go                       =
    =       - Used variables: velocity, goalVelocity and position                               =
    =       - We set desired speed to -> acceleration                                           =
    =                                                                                           =
    =       - For independent frame movement                                                    =
    =           - Operation: [*] velocity.x += approach(goalVelocity.x, velocity.x, dt)         =
    =                        [*] velocity.y += approach(goalVelocity.y, velocity.y, dt)         =
    =                                                                                           =
    =                        [*] position.x += velocity.x * dt                                  =
    =                        [*] position.y += velocity.y * dt                                  =
    =                                                                                           =
    =============================================================================================
    =                                                                                           =
    = (*) Notes: - It makes no sense having a Static PhysicsBody without a collider.            =
    =            - Don't give a PhysicsBody mass of -1, give the macro INFINTE_MASS and know    =
    =              -1 means infinite mass for the engine.                                       =                                                                               
    =============================================================================================
*/

#include <engine/components/colliders/Collider.h>
#include <engine/components/colliders/BoxCollider.h>
#include <engine/components/colliders/CircleCollider.h>
#include <engine/components/colliders/PolygonCollider.h>
#include <engine/components/Component.h>

#define INFINTE_MASS -1
#define NO_MASS      0

namespace engine {

    class PhysicsBody;
    typedef std::shared_ptr<PhysicsBody> PhysicsBodyPtr;

    enum BodyType { DYNAMIC, KNINEMATIC, STATIC };
    enum MovementType { LINEAR, APPROACH, ACCELERATED };

    class PhysicsBody : public Component {

        public:
            Vec2f position;

            Vec2f velocity, gaolVelocity;
            Vec2f acceleration;
            float angularSpeed;
            float mass;

            float restitution;
            float friction;

            Vec2f gravity;

            BodyType bodyType;
            MovementType movementType;

            std::vector<ColliderPtr> colliders;

        public:
            explicit PhysicsBody(GameObject* _gameObject, const Vec2f& _position, const BodyType& _bodyType = BodyType::STATIC, const Vec2f& _gravity = Vec2f(),
             const MovementType& _movementType = MovementType::APPROACH);

            static PhysicsBodyPtr create(GameObject* _gameObject, const Vec2f& _position, const BodyType& _bodyType = BodyType::STATIC, const Vec2f& _gravity = Vec2f(),
                                       const MovementType& _movementType = MovementType::APPROACH);

        public:
            void update(float _dt)    override;
            void fixUpdate(float _fixDt) override;
            void postUpdate(float _dt)   override;

        private:
            float approach(float _goalVelocity, float _currentVelocity, float _dt);
            void move(float _dt);
            void applyGravity(float _dt);
    };
}

#endif //PHYSICS_BODY_H