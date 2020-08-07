#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace engine {

    class Logger {
        private:
            static std::shared_ptr<spdlog::logger> coreLogger;
            static std::shared_ptr<spdlog::logger> clientLogger;

        public:
            static void init();

            static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
            static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }
    };

}

    // Core log macros
    #define LOG_TRACE_CORE(...)    ::engine::Logger::getCoreLogger()->trace(__VA_ARGS__)
    #define LOG_INFO_CORE(...)     ::engine::Logger::getCoreLogger()->info(__VA_ARGS__)
    #define LOG_WARN_CORE(...)     ::engine::Logger::getCoreLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR_CORE(...)    ::engine::Logger::getCoreLogger()->error(__VA_ARGS__)
    #define LOG_CRITICAL_CORE(...) ::engine::Logger::getCoreLogger()->critical(__VA_ARGS__)

    // Client log macros
    #define LOG_TRACE(...)         ::engine::Logger::getClientLogger()->trace(__VA_ARGS__)
    #define LOG_INFO(...)          ::engine::Logger::getClientLogger()->info(__VA_ARGS__)
    #define LOG_WARN(...)          ::engine::Logger::getClientLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR(...)         ::engine::Logger::getClientLogger()->error(__VA_ARGS__)
    #define LOG_CRITICAL(...)      ::engine::Logger::getClientLogger()->critical(__VA_ARGS__)

#endif // LOGGER_H


