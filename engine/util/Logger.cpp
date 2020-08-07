#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace engine {

    std::shared_ptr<spdlog::logger> Logger::coreLogger;
    std::shared_ptr<spdlog::logger> Logger::clientLogger;

    void Logger::init() {
        std::vector<spdlog::sink_ptr> _logSinks;
        _logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        _logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Engine.log", true));

        _logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        _logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        Logger::coreLogger = std::make_shared<spdlog::logger>("Engine", begin(_logSinks), end(_logSinks));
        spdlog::register_logger(Logger::coreLogger);
        Logger::coreLogger->set_level(spdlog::level::trace);
        Logger::coreLogger->flush_on(spdlog::level::trace);

        Logger::clientLogger = std::make_shared<spdlog::logger>("App", begin(_logSinks), end(_logSinks));
        spdlog::register_logger(Logger::clientLogger);
        Logger::clientLogger->set_level(spdlog::level::trace);
        Logger::clientLogger->flush_on(spdlog::level::trace);
    }

}

