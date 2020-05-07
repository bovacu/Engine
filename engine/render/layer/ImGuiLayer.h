#pragma once

#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "engine/render/layer/Layer.h"

#include "engine/event/WindowEvent.h"
#include "engine/event/KeyEvent.h"
#include "engine/event/MouseEvent.h"

namespace engine {

    class ImGuiLayer : public Layer {
        public:
            ImGuiLayer();
            ~ImGuiLayer() override = default;

            void onInit()                       override;
            void onEvent(Event& _e)             override;
            void onUpdate(Timestep _dt)         override {  }
            void onRender(Timestep _dt)         override {  }
            void onImGuiRender(Timestep _dt)    override {  }
            void onEnd()                        override;

            void begin();
            void end();

            bool onMouseScrolled(MouseScrolledEvent& _e);

            static void drawDebugInfo();

        private:
            static void charToIntSize(const std::string& _size, int* _resolution);
    };

}

#endif //IMGUILAYER_H
