#include "pch.h"
#include "Window.h"

#ifdef ENGINE_PLATFORM_WINDOWS
    #include <engine/platform/Windows/WindowsWindow.h>
#endif

#ifdef ENGINE_PLATFORM_LINUX
    #include <engine/platform/Linux/LinuxWindow.h>
#endif

namespace engine {

    std::unique_ptr<Window> Window::createWindow(const WindowProperties& _props) {
        #if defined(ENGINE_PLATFORM_WINDOW)
                return std::make_unique<WindowsWindow>(_props);
        #elif defined(ENGINE_PLATFORM_LINUX)
            return std::make_unique<LinuxWindow>(_props);
        #else
                ENGINE_CORE_ASSERT(false, "Unknown platform!");
                return nullptr;
        #endif
    }

}
