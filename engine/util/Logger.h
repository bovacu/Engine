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

//namespace engine {
//
//	class Logger {
//		public:
//			time_t 		_now;
//			tm* 		_ltm;
//			std::string _time;
//	};
//
//	// time_t      Logger::_now    = time(0);
//    // tm*         Logger::_ltm    = localtime(&_now);
//    // std::string Logger::_time   = "";
//}
//
//	#define RST   "\x1B[0m"
//	#define KRED  "\x1B[31m"
//	#define KGRN  "\x1B[32m"
//	#define KYEL  "\x1B[33m"
//	#define KBLU  "\x1B[34m"
//	#define KMAG  "\x1B[35m"
//	#define KCYN  "\x1B[36m"
//	#define KWHT  "\x1B[37m"
//
//	#define FRED(x) KRED << x << RST
//	#define FGRN(x) KGRN << x << RST
//	#define FYEL(x) KYEL << x << RST
//	#define FBLU(x) KBLU << x << RST
//	#define FMAG(x) KMAG << x << RST
//	#define FCYN(x) KCYN << x << RST
//	#define FWHT(x) KWHT << x << RST
//
//	#define BOLD(x) "\x1B[1m" << x << RST
//	#define UNDL(x) "\x1B[4m" << x << RST
//
//	#define LOG_INFO(x)    	_logger._now = time(0); 												\
//							_logger._ltm = localtime(&_logger._now); 								\
//							_logger._time = "["; 													\
//							_logger._time.append(std::to_string(_logger._ltm->tm_hour));			\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_min));				\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_sec)); 			\
//							_logger._time.append("] "); 											\
//							std::cout << KCYN << _logger._time << RST << BOLD(FGRN(x)) << std::endl;\
//							_logger._time.clear();
//
//	#define LOG_WARN(x)		_logger._now = time(0); 												\
//							_logger._ltm = localtime(&_logger._now); 								\
//							_logger._time = "["; 													\
//							_logger._time.append(std::to_string(_logger._ltm->tm_hour));			\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_min)); 			\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_sec)); 			\
//							_logger._time.append("] "); 											\
//							std::cout << KCYN << _logger._time << RST << BOLD(FYEL(x)) << std::endl;
//
//	#define LOG_ERROR(x)   	_logger._now = time(0); 												\
//							_logger._ltm = localtime(&_logger._now); 								\
//							_logger._time = "["; 													\
//							_logger._time.append(std::to_string(_logger._ltm->tm_hour));			\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_min)); 			\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_sec)); 			\
//							_logger._time.append("] "); 											\
//							std::cout << KCYN << _logger._time << RST << BOLD(FRED(x)) << std::endl;
//
//	#define LOG_TRACE(x)   	_logger._now = time(0); 												\
//							_logger._ltm = localtime(&_logger._now); 								\
//							_logger._time = "["; 													\
//							_logger._time.append(std::to_string(_logger._ltm->tm_hour));			\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_min)); 			\
//							_logger._time.append(":"); 												\
//							_logger._time.append(std::to_string(_logger._ltm->tm_sec)); 			\
//							_logger._time.append("] "); 											\
//							std::cout << KCYN << _logger._time << RST << BOLD(FWHT(x)) << std::endl;

#endif // LOGGER_H


