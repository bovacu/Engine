#pragma once

#ifndef CLOCK_H
#define CLOCK_H

namespace engine {

    class Clock {
        private:
            float startTime;

        public:
            explicit Clock();

            float getElapsedTimeSc() const;
            float getElapsedTimeMs() const;

            float restart();
    };

}

#endif //CLOCK_H
