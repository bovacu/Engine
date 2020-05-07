#pragma once

#ifndef UI_H
#define UI_H


#include <engine/event/Event.h>
#include <engine/util/Timestep.h>

namespace engine {

    class Ui {
        public:
            Ui() = default;
            virtual ~Ui() = default;

            virtual void onInit()               = 0;
            virtual void onEvent(Event& _e)     = 0;
            virtual void onUpdate(Timestep _dt) = 0;
            virtual void onRender()             = 0;
            virtual void onClose()              = 0;
    };

    namespace Ptr {
        typedef std::shared_ptr<Ui> UiPtr;
    }
}

#endif //UI_H
