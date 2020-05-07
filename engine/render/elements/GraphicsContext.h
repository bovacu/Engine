#pragma once

#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "pch.h"

namespace engine {

    class GraphicsContext {
        public:
            virtual void init() = 0;
            virtual void swapBuffers() = 0;

            static std::shared_ptr<GraphicsContext> create(void* window);
    };

}


#endif // GRAPHICS_CONTEXT_H
