#pragma once

#ifndef ENGINE_ENGINEAPI_H
#define ENGINE_ENGINEAPI_H

namespace engine::api {

    class EngineAPI {

    };

    class Console {
        private:
            struct Time {
                int hour, min, sec;
            };

            static Time getTime() {
                                  // Get the current time
                #if defined(ENGINE_PLATFORM_WINDOWS)
                    time_t currentTime;
                    struct tm localTime {};
                    time( &currentTime );
                    localtime_s(&localTime, &currentTime );
                    return Time{localTime.tm_hour, localTime.tm_min, localTime.tm_sec};
                #elif defined(ENGINE_PLATFORM_LINUX)
                    time_t currentTime;
                    struct tm *localTime;
                    time( &currentTime );
                    localTime = localtime( &currentTime );
                    return {localTime->tm_hour, localTime->tm_min, localTime->tm_sec};
                #endif

            }

        public:
            static void log(const std::string& _log);

            static void logError(const std::string& _log);

            static void logWarning(const std::string& _log);
    };

}

#endif //ENGINE_ENGINEAPI_H
