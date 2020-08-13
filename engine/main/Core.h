#pragma once

#include <memory>
#include <engine/util/Logger.h>

/// Default paths to essential elements, can be modified if needed.
#define DEFAULT_SHADER_PATH "assets/shaders/Texture.glsl"
#define PIXEL_SHADER "assets/shaders/FlatColor.glsl"

/// TODO not sure if I still need this, check
#define ASPECT_RATIO_PIXEL 0.05f

/// Defines the macro ENGINE_DEBUG if needed.
#ifndef NDEBUG
    #define ENGINE_DEBUG
#endif

/// Choosing the OS with macros and CMake.
#ifdef _WIN32
#ifdef _WIN64
    #define ENGINE_PLATFORM_WINDOWS             /// For both WIN32 and WIN64
#else
		#error "x86 Builds are not supported!"  /// Not supported due to some 3th-party libraries
#endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>                     /// Mac not yet supported, copied this code from a tutorial to
        /* TARGET_OS_MAC exists on all the platforms    /// include make as a macro in CMake.
         * so we must check all of them (in this order)
         * to ensure that we're running on MAC
         * and not some other Apple platform */
        #if TARGET_IPHONE_SIMULATOR == 1
            #error "IOS simulator is not supported!"
        #elif TARGET_OS_IPHONE == 1
            #define ENGINE_PLATFORM_IOS
            #error "IOS is not supported!"
        #elif TARGET_OS_MAC == 1
            #define ENGINE_PLATFORM_MACOS
            #error "MacOS is not supported!"
        #else
            #error "Unknown Apple platform!"
        #endif
    /* We also have to check __ANDROID__ before __linux__
     * since android is based on the linux kernel
     * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define ENGINE_PLATFORM_ANDROID     /// Android not yet supported.
	#error "Android is not supported!"
#elif defined(__linux__)
	#define ENGINE_PLATFORM_LINUX /// Macro for Linux operating systems, currently supported.
#else
	#error "Unknown platform!"
#endif // End of platform detection

/// If debug, we have asserts to check internal and critical errors, but implementation varies depending on the SO.
#ifdef ENGINE_DEBUG
    #if defined(ENGINE_PLATFORM_WINDOWS)
		#define ENGINE_DEBUGBREAK() __debugbreak()
	#elif defined(ENGINE_PLATFORM_LINUX)
		#include <signal.h>
		#define ENGINE_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define ENGINE_ENABLE_ASSERTS
#else
    #define ENGINE_DEBUGBREAK()
#endif

/// Asserts for inside the engine(core) and for applications and games made with the engine(not core).
#ifdef ENGINE_ENABLE_ASSERTS
    #define ENGINE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); ENGINE_DEBUGBREAK(); } }
	#define ENGINE_CORE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR_CORE("Assertion Failed: {0}", __VA_ARGS__); ENGINE_DEBUGBREAK(); } }
#else
    #define ENGINE_ASSERT(x, ...)
    #define ENGINE_CORE_ASSERT(x, ...)
#endif

/// This method was initially simpler, I found some performance downgrades with that and CLion gave a warning on not
/// using it. It took some time to found a working solution replacing 'std::bind(&fn, this, std::placeholders::_1)'.
/// This method is used to bind to an specific Event a function to execute during that event.
///
/// This function must much the EventFunction criteria type, that is returning a bool a having an Event& child as
/// parameter.
#define ENGINE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
