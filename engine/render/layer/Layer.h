#pragma once

#ifndef LAYER_H
#define LAYER_H

#include <engine/main/Core.h>
#include <engine/util/Delta.h>
#include <engine/event/Event.h>

namespace engine {

    class Layer {
        protected:
            std::string debugName;

        public:
            explicit Layer(const std::string& _debugName = "Layer")  : debugName(_debugName) {  }
            virtual ~Layer() = default;

            virtual void onInit()                       = 0;
            virtual void onEvent(Event& _event)         = 0;
            virtual void onUpdate(Delta _dt)         = 0;
            virtual void onFixedUpdate(Delta _dt)    {  }
            virtual void onRender(Delta _dt)         = 0;
            virtual void onImGuiRender(Delta _dt)    {  }
            virtual void onEnd()                        {  }

            [[nodiscard]] const std::string& getName() const { return this->debugName; }
    };

}

#endif //LAYER_H
