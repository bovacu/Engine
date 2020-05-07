#pragma once

#ifndef MOUSE_KEYS_H
#define MOUSE_KEYS_H

namespace engine {

    /// -----------------------------------------------------------------
    /// |			CODES FOR MOUSE BUTTONS, THEY ARE					|
    /// |			THE SAME AS GLFW'S ONES, BUT NOW ARE				|
    /// |			PROPERTY OF THE ENGINE AND DON'T RELAY				|
    /// |			ON GLFW												|
    /// -----------------------------------------------------------------

    typedef enum class MouseCode : uint16_t {
        // From glfw3.h
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,

        ButtonLast = Button7,
        ButtonLeft = Button0,
        ButtonRight = Button1,
        ButtonMiddle = Button2
    } Mouse;

    inline std::ostream &operator<<(std::ostream &os, MouseCode mouseCode) {
        os << static_cast<int32_t>(mouseCode);
        return os;
    }
}
#define MOUSE_BUTTON_0      ::engine::Mouse::Button0
#define MOUSE_BUTTON_1      ::engine::Mouse::Button1
#define MOUSE_BUTTON_2      ::engine::Mouse::Button2
#define MOUSE_BUTTON_3      ::engine::Mouse::Button3
#define MOUSE_BUTTON_4      ::engine::Mouse::Button4
#define MOUSE_BUTTON_5      ::engine::Mouse::Button5
#define MOUSE_BUTTON_6      ::engine::Mouse::Button6
#define MOUSE_BUTTON_7      ::engine::Mouse::Button7
#define MOUSE_BUTTON_LAST   ::engine::Mouse::ButtonLast
#define MOUSE_BUTTON_LEFT   ::engine::Mouse::ButtonLeft
#define MOUSE_BUTTON_RIGHT  ::engine::Mouse::ButtonRight
#define MOUSE_BUTTON_MIDDLE ::engine::Mouse::ButtonMiddle

#endif // MOUSE_KEYS_H
