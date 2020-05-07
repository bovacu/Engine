#pragma once

#ifndef TIMESTEP_H
#define TIMESTEP_H


namespace engine {

    class Timestep {
        private:
            float time;

        public:
            Timestep(float _time = 0.0f) : time(_time) {  }

            operator float() const { return this->time; }

            float GetSeconds() const { return this->time; }
            float GetMilliseconds() const { return this->time * 1000.0f; }
    };

}

#endif //TIMESTEP_H
