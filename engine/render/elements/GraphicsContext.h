#pragma once

#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include <memory>

namespace engine {

    class GraphicsContext;
    typedef std::shared_ptr<GraphicsContext> GraphicsContextPtr;

    class GraphicsContext {
        public:
            virtual void init() = 0;
            virtual void swapBuffers() = 0;

            static GraphicsContextPtr create(void* window);
    };

}


#endif // GRAPHICS_CONTEXT_H
