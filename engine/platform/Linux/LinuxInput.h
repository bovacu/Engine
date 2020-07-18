#pragma once

#ifndef LINUX_INPUT_H
#define LINUX_INPUT_H

#include <engine/input/Input.h>

namespace engine {

    class LinuxInput : public Input {
        protected:
            bool isKeyPressed_v(KeyCode _key)               override;
            bool isKeyJustPressed_v(KeyCode _keyCode)       override;
            bool isKeyReleased_v(KeyCode _key)              override;

            bool isMousePressed_v(MouseCode _mouseButton)   override;
            bool isMouseReleased_v(MouseCode _mouseButton)  override;

            Vec2f getMousePosition_v()                      override;
            float getMouseX_v()                             override;
            float getMouseY_v()                             override;
    };

}

#endif //LINUX_INPUT_H
