#pragma once

#ifndef ENGINE_SCRIPTABLEOBJECT_H
#define ENGINE_SCRIPTABLEOBJECT_H


#include <engine/ecs/GameObject.h>

namespace engine {

    class ScriptableObject {
        friend class Scene;

        public:
            template<typename T>
            T& getComponent() {
                return this->gameObject.getComponent<T>();
            }

        private:
            GameObject gameObject;
    };

}

#endif //ENGINE_SCRIPTABLEOBJECT_H
