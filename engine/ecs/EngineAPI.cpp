#include "pch.h"
#include "EngineAPI.h"

#if defined(USING_INSPECTOR)
    #include <engineInspector/Inspector.h>
#endif

namespace engine {


    void api::Console::log(const std::string& _log) {
        #if defined(USING_INSPECTOR)
            auto _time = getTime();
            Inspector::logger.addLog("[%i:%i:%i] %s\n", _time.hour, _time.min, _time.sec, _log.c_str());
        #elif
            LOG_INFO(_log);
        #endif
    }

    void api::Console::logError(const std::string& _log) {
        #if defined(USING_INSPECTOR)
            auto _time = getTime();
            Inspector::logger.addLog("[%i:%i:%i]~[ERROR] %s\n", _time.hour, _time.min, _time.sec, _log.c_str());
        #elif
            LOG_ERROR(_log);
        #endif
    }

    void api::Console::logWarning(const std::string& _log) {
        #if defined(USING_INSPECTOR)
            auto _time = getTime();
            Inspector::logger.addLog("[%i:%i:%i]~[WARNING] %s\n", _time.hour, _time.min, _time.sec, _log.c_str());
        #elif
            LOG_WARN(_log);
        #endif
    }
}