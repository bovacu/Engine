#include "pch.h"
#include "Clock.h"

#include <GLFW/glfw3.h>
#include "engine/util/Timestep.h"

namespace engine {

    Clock::Clock() : startTime((float)glfwGetTime()) {  }

    float Clock::getElapsedTimeSc() const {
        Timestep _now = (float)glfwGetTime();
        return _now - this->startTime;
    }

    float Clock::getElapsedTimeMs() const {
        Timestep _now = (float)glfwGetTime();
        return (_now - this->startTime) * 1000;
    }

    float Clock::restart() {
        Timestep _now = (float)glfwGetTime();
        float _elapsed = _now - this->startTime;
        this->startTime = _now;
        return _elapsed;
    }
}