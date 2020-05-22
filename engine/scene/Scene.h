#pragma once

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H


#include <engine/render/layer/LayerStack.h>

namespace engine {

    class Scene {
        private:
            std::string name;
            LayerStack stack;

        public:
            explicit Scene(const std::string& _name) : name(_name) {  };
            ~Scene();

            virtual void onInit()               = 0;
            virtual void onEvent(Event& _e)     = 0;
            virtual void onUpdate(Timestep _dt) = 0;
            virtual void onRender(Timestep _dt) = 0;
            virtual void onEnd()                = 0;
    };

}

#endif //ENGINE_SCENE_H
