#include "pch.h"
#include "Window.h"

#ifdef ENGINE_PLATFORM_WINDOWS
    #include "engine/platform/Windows/WindowsWindow.h"
#endif

namespace engine {

    std::unique_ptr<Window> Window::createWindow(const WindowProperties& _props) {
        #ifdef ENGINE_PLATFORM_WINDOWS
                return std::make_unique<WindowsWindow>(_props);
        #else
                ENGINE_CORE_ASSERT(false, "Unknown platform!");
                return nullptr;
        #endif
    }

}
