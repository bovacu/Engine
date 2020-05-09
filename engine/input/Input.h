#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "pch.h"
#include "engine/input/KeyboardKeys.h"
#include "engine/input/MouseKeys.h"
#include "engine/input/GamePadKeys.h"

namespace engine {

    class Input;
    typedef std::unique_ptr<Input> InputPtr;

    class Input {
        private:
            static InputPtr inputInstance;

        public:
            static bool isKeyPressed(KeyCode _keyCode)              { return inputInstance->isKeyPressed_v(_keyCode); }
            static bool isKeyDown(KeyCode _keyCode)                 { return inputInstance->isKeyDown_v(_keyCode); }
            static bool isKeyReleased(KeyCode _keyCode)             { return inputInstance->isKeyReleased_v(_keyCode); }

            static bool isMousePressed(MouseCode _mouseButton)      { return inputInstance->isMousePressed_v(_mouseButton); }
            static bool isMouseDown(MouseCode _mouseButton)         { return inputInstance->isMouseDown_v(_mouseButton); }
            static bool isMouseReleased(MouseCode _mouseButton)     { return inputInstance->isMouseReleased_v(_mouseButton); }

            static Vec2f getMousePosition()                         { return inputInstance->getMousePosition_v(); }
            static float getMouseX()                                { return inputInstance->getMouseX_v(); }
            static float getMouseY()                                { return inputInstance->getMouseY_v(); }

            virtual ~Input() = default;
            static InputPtr create();

        protected:
            virtual bool isKeyPressed_v(KeyCode _keyCode)           = 0;
            virtual bool isKeyDown_v(KeyCode _keyCode)              = 0;
            virtual bool isKeyReleased_v(KeyCode _keyCode)          = 0;

            virtual bool isMousePressed_v(MouseCode _mouseButton)   = 0;
            virtual bool isMouseDown_v(MouseCode _mouseButton)      = 0;
            virtual bool isMouseReleased_v(MouseCode _mouseButton)  = 0;

            virtual Vec2f getMousePosition_v()                      = 0;
            virtual float getMouseX_v()                             = 0;
            virtual float getMouseY_v()                             = 0;
    };
}

#endif //INPUT_H
