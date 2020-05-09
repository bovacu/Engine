#pragma once

#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H
#include <engine/util/EngineUtil.h>

/*  =================================================================================================
    =                                      TRANSFORM2D                                              =
    =================================================================================================
    = (*) This object defines the position, rotation and scale in the game world and every          =
    =     object in the game should have one so everything works fine.                              =
    =                                                                                               =
    = (*) Each GameObject will have one as default.                                                 =
    =                                                                                               =
    = (*) PhysicsBody component position and every physic calculation will be based on this         =
    =     object.                                                                                   =
    =                                                                                               =
    ================================================================================================= 
*/

namespace engine {

    class Transform2D {
        public:
            Vec2f position;
            float  rotation;
            Vec2f scale;

        public:
            Transform2D();
            Transform2D(const Vec2f& _position, float _rotation = 0, const Vec2f& _scale = Vec2f(1, 1));

            void setPosition(const Vec2f& _position);
            void setPosition(float _x, float _y);

            void translate(const Vec2f& _position);
            void translate(float _x, float _y);
    };

}

#endif //TRANSFORM_H