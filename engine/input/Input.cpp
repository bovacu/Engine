#include "pch.h"
#include "Input.h"

#ifdef ENGINE_PLATFORM_WINDOWS
    #include <engine/platform/Windows/WindowsInput.h>
#endif

#ifdef ENGINE_PLATFORM_LINUX
    #include <engine/platform/Linux/LinuxInput.h>
#endif

namespace engine {

    InputPtr Input::inputInstance = Input::create();
    std::unordered_map<KeyCode, bool> Input::pressedKeys = {
            {KeyCode::LeftControl,  false},
            {KeyCode::LeftSuper,    false},
            {KeyCode::LeftAlt, false},
            {KeyCode::Space,        false},
            {KeyCode::RightAlt,     false},
            {KeyCode::RightControl, false},
            {KeyCode::LeftShift,    false},
            {KeyCode::Z,            false},
            {KeyCode::X,            false},
            {KeyCode::C,            false},
            {KeyCode::V,            false},
            {KeyCode::B,            false},
            {KeyCode::N,            false},
            {KeyCode::M,            false},
            {KeyCode::Comma,        false},
            {KeyCode::Period,       false},
            {KeyCode::Slash,        false},
            {KeyCode::RightShift,   false},
            {KeyCode::CapsLock,     false},
            {KeyCode::A,            false},
            {KeyCode::S,            false},
            {KeyCode::D,            false},
            {KeyCode::F,            false},
            {KeyCode::G,            false},
            {KeyCode::H,            false},
            {KeyCode::J,            false},
            {KeyCode::K,            false},
            {KeyCode::L,            false},
            {KeyCode::Semicolon,    false},
            {KeyCode::Apostrophe,   false},
            {KeyCode::Enter,        false},
            {KeyCode::Tab,          false},
            {KeyCode::Q,            false},
            {KeyCode::W,            false},
            {KeyCode::E,            false},
            {KeyCode::R,            false},
            {KeyCode::T,            false},
            {KeyCode::Y,            false},
            {KeyCode::U,            false},
            {KeyCode::I,            false},
            {KeyCode::O,            false},
            {KeyCode::P,            false},
            {KeyCode::LeftBracket,  false},
            {KeyCode::RightBracket, false},
            {KeyCode::RightSuper,   false},
            {KeyCode::Backslash,    false},
            {KeyCode::GraveAccent,  false},
            {KeyCode::D0,           false},
            {KeyCode::D1,           false},
            {KeyCode::D2,           false},
            {KeyCode::D3,           false},
            {KeyCode::D4,           false},
            {KeyCode::D5,           false},
            {KeyCode::D6,           false},
            {KeyCode::D7,           false},
            {KeyCode::D8,           false},
            {KeyCode::D9,           false},
            {KeyCode::Minus,        false},
            {KeyCode::Equal,        false},
            {KeyCode::Backspace,    false},
            {KeyCode::Escape,       false},
            {KeyCode::F1,           false},
            {KeyCode::F2,           false},
            {KeyCode::F3,           false},
            {KeyCode::F4,           false},
            {KeyCode::F5,           false},
            {KeyCode::F6,           false},
            {KeyCode::F7,           false},
            {KeyCode::F8,           false},
            {KeyCode::F9,           false},
            {KeyCode::F10,          false},
            {KeyCode::F11,          false},
            {KeyCode::F12,          false},
            {KeyCode::PrintScreen,  false},
            {KeyCode::ScrollLock,   false},
            {KeyCode::Insert,       false},
            {KeyCode::Delete,       false},
            {KeyCode::Home,         false},
            {KeyCode::End,          false},
            {KeyCode::PageUp,       false},
            {KeyCode::PageDown,     false},
            {KeyCode::Right,        false},
            {KeyCode::Left,         false},
            {KeyCode::Up,           false},
            {KeyCode::Down,         false},

    };

    InputPtr Input::create() {

        #if defined(ENGINE_PLATFORM_WINDOWS)
                return std::make_unique<WindowsInput>();
        #elif defined(ENGINE_PLATFORM_LINUX)
                return std::make_unique<LinuxInput>();
        #else
                ENGINE_CORE_ASSERT(false, "Unknown platform!");
                return nullptr;
        #endif
    }

}