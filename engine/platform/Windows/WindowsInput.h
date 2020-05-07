#pragma once

#ifndef WINDOWS_INPUT_H
#define WINDOWS_INPUT_H

#include "engine/input/Input.h"

namespace engine {

    class WindowsInput : public Input {
        protected:
            bool isKeyPressed_v(KeyCode _key)               override;
            bool isKeyDown_v(KeyCode _key)                  override;
            bool isKeyReleased_v(KeyCode _key)              override;

            bool isMousePressed_v(MouseCode _mouseButton)   override;
            bool isMouseDown_v(MouseCode _mouseButton)      override;
            bool isMouseReleased_v(MouseCode _mouseButton)  override;

            Vec2f getMousePosition_v()                      override;
            float getMouseX_v()                             override;
            float getMouseY_v()                             override;
    };

}

#endif //WINDOWS_INPUT_H
