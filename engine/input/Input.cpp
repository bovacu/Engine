#include "pch.h"
#include "Input.h"

#ifdef ENGINE_PLATFORM_WINDOWS
    #include <engine/platform/Windows/WindowsInput.h>
#endif

namespace engine {

    InputPtr Input::inputInstance = Input::create();
    std::unordered_map<KeyCode, bool> Input::pressedKeys = { {KEY_SPACE, false} };

    InputPtr Input::create() {

        #ifdef ENGINE_PLATFORM_WINDOWS
                return std::make_unique<WindowsInput>();
        #else
                ENGINE_CORE_ASSERT(false, "Unknown platform!");
                return nullptr;
        #endif
    }

}