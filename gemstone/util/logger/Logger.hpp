#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "spdlog/async.h"
#include "spdlog/spdlog.h"

#include "util/macros.hpp"

namespace GEM {
namespace util {
    class Logger;
}
}

/**
 * @brief A singleton-esque logger wrapping a spdlog async logger. This exposes a map
 * of loggers, each of which has their own logging level associated with it. To use it
 * be sure to register a logger with the registerLogger function.
 * 
 * @note If you wish to make use of the nice macros at the bottom of this file, be sure
 * to define some LOGGER_NAME string to whatever logger you desire to use. The macros
 * assume that LOGGER_NAME is already defined. If LOGGER_NAME is not defined and you wish
 * to programatically change the logger being used (perhaps in a function taking a logger
 * name parameter) you must use the GEM::util::Logger functions directly
 * 
 * @todo Clamp log levels based on debug vs release mode or something?
 * 
 * @todo ALlow us to set stdout levels vs file levels differently?
 */
class GEM::util::Logger {
public: // public classes and enums
    /**
     * @brief An enum class representing each of the different logging levels
     */
    enum class Level {
        trace,
        debug,
        info,
        warning,
        error,
        critical
    };

    /**
     * @brief A simple struct used for registering loggers. This pairs a logger's
     * name with its desired logging level. This is essentially just a pair with
     * named elements
     */
    struct RegistrationInfo {
        const std::string loggerName;
        const GEM::util::Logger::Level level;
    };

    /**
     * @brief A class to manage the "scope" (indentation) of the logging statements. Constructing an
     * instance of one of these will log an opening curly brace and increment the indentation count
     * for all logging statements until the Scoper instance falls out of scope, where then the
     * indendation count is decremented and a closing curly brace is logged
     */
    class Scoper {
    public: // public static functions
        static uint32_t getIndentationCount() { return GEM::util::Logger::Scoper::indentationCount; }

    public: // public member functions
        Scoper(const std::string& loggerName, const GEM::util::Logger::Level level);
        ~Scoper();

    private: // private static variables
        static uint32_t indentationCount;

    private: // private member variables
        const std::string m_loggerName;
        const GEM::util::Logger::Level m_level;
    };

public: // public static functions
    static void init();
    static void registerLogger(const std::string& loggerName, const GEM::util::Logger::Level level);
    static void registerLoggers(const std::vector<const GEM::util::Logger::RegistrationInfo>& loggerInfos);

    template<typename... Args>
    static void trace(const std::string& loggerName, const std::string& format, Args&&... args) {
        assertInitialized();
        GEM::util::Logger::loggerPtrMap[loggerName]->trace(GEM::util::Logger::createIndentationString() + format, args...);
    }

    template<typename... Args>
    static void debug(const std::string& loggerName, const std::string& format, Args&&... args) {
        assertInitialized();
        GEM::util::Logger::loggerPtrMap[loggerName]->debug(GEM::util::Logger::createIndentationString() + format, args...);
    }

    template<typename... Args>
    static void info(const std::string& loggerName, const std::string& format, Args&&... args) {
        assertInitialized();
        GEM::util::Logger::loggerPtrMap[loggerName]->info(GEM::util::Logger::createIndentationString() + format, args...);
    }

    template<typename... Args>
    static void warning(const std::string& loggerName, const std::string& format, Args&&... args) {
        assertInitialized();
        GEM::util::Logger::loggerPtrMap[loggerName]->warn(GEM::util::Logger::createIndentationString() + format, args...);
    }

    template<typename... Args>
    static void error(const std::string& loggerName, const std::string& format, Args&&... args) {
        assertInitialized();
        GEM::util::Logger::loggerPtrMap[loggerName]->error(GEM::util::Logger::createIndentationString() + format, args...);
    }

    template<typename... Args>
    static void critical(const std::string& loggerName, const std::string& format, Args&&... args) {
        assertInitialized();
        GEM::util::Logger::loggerPtrMap[loggerName]->critical(GEM::util::Logger::createIndentationString() + format, args...);
    }

    /**
     * @brief Log a message with the correct logger at the correct level.
     * 
     * @tparam Args The types of the variadic arguments we are using
     * @param loggerName The name of the logger to use
     * @param level The logging level to log the message at
     * @param format The formatting string
     * @param args All of the variadic arguments to log
     */
    template<typename... Args>
    static void log(const std::string& loggerName, const GEM::util::Logger::Level level, const std::string& format, Args&&... args) {
        switch (level) {
            case GEM::util::Logger::Level::trace:
                GEM::util::Logger::trace(loggerName, format, args...);
                break;
            case GEM::util::Logger::Level::debug:
                GEM::util::Logger::debug(loggerName, format, args...);
                break;
            case GEM::util::Logger::Level::info:
                GEM::util::Logger::info(loggerName, format, args...);
                break;
            case GEM::util::Logger::Level::warning:
                GEM::util::Logger::warning(loggerName, format, args...);
                break;
            case GEM::util::Logger::Level::error:
                GEM::util::Logger::error(loggerName, format, args...);
                break;
            case GEM::util::Logger::Level::critical:
                GEM::util::Logger::critical(loggerName, format, args...);
                break;
        }
    }

public: // public member functions
    Logger() = delete;

private: // private static functions
    static void assertInitialized();

    static std::string createIndentationString();

private: // private static member variables
    static bool initialized;
    static std::shared_ptr<spdlog::details::thread_pool> threadPool;
    static std::vector<spdlog::sink_ptr> sinkPtrs;
    static std::map<std::string, std::shared_ptr<spdlog::async_logger>> loggerPtrMap;
};

/**
 * @brief Log something
 * 
 * @note To use this LOGGER_NAME must be defined and the first argument of the variadic
 * arguments must be a formatting string
 */

#define LOG_TRACE(...) \
    GEM::util::Logger::trace(LOGGER_NAME, __VA_ARGS__)

#define LOG_DEBUG(...) \
    GEM::util::Logger::debug(LOGGER_NAME, __VA_ARGS__)

#define LOG_INFO(...) \
    GEM::util::Logger::info(LOGGER_NAME, __VA_ARGS__)

#define LOG_WARNING(...) \
    GEM::util::Logger::warning(LOGGER_NAME, __VA_ARGS__)
    
#define LOG_ERROR(...) \
    GEM::util::Logger::error(LOGGER_NAME, __VA_ARGS__)

#define LOG_CRITICAL(...) \
    GEM::util::Logger::critical(LOGGER_NAME, __VA_ARGS__)

/**
 * @brief Log a scope change
 * 
 * @note To use this LOGGER_NAME must be defined
 */

#define LOG_SCOPE_CHANGE_TRACE() \
    const GEM::util::Logger::Scoper UNIQUE_NAME(scoper)(LOGGER_NAME, GEM::util::Logger::Level::trace)

#define LOG_SCOPE_CHANGE_DEBUG() \
    const GEM::util::Logger::Scoper UNIQUE_NAME(scoper)(LOGGER_NAME, GEM::util::Logger::Level::debug)

#define LOG_SCOPE_CHANGE_INFO() \
    const GEM::util::Logger::Scoper UNIQUE_NAME(scoper)(LOGGER_NAME, GEM::util::Logger::Level::info)

#define LOG_SCOPE_CHANGE_WARNING() \
    const GEM::util::Logger::Scoper UNIQUE_NAME(scoper)(LOGGER_NAME, GEM::util::Logger::Level::warning)

#define LOG_SCOPE_CHANGE_ERROR() \
    const GEM::util::Logger::Scoper UNIQUE_NAME(scoper)(LOGGER_NAME, GEM::util::Logger::Level::error)

#define LOG_SCOPE_CHANGE_CRITICAL() \
    const GEM::util::Logger::Scoper UNIQUE_NAME(scoper)(LOGGER_NAME, GEM::util::Logger::Level::critical)

/**
 * @brief log a function call *WITHOUT* scope change
 * 
 * @note To use this LOGGER_NAME must be defined
 */

#define LOG_FUNCTION_ENTRY_TRACE(format, ...) \
    GEM::util::Logger::trace(LOGGER_NAME, __PRETTY_FUNCTION__ + std::string(" [ ") + format + std::string(" ]"), __VA_ARGS__)

#define LOG_FUNCTION_ENTRY_DEBUG(format, ...) \
    GEM::util::Logger::debug(LOGGER_NAME, __PRETTY_FUNCTION__ + std::string(" [ ") + format + std::string(" ]"), __VA_ARGS__)

#define LOG_FUNCTION_ENTRY_INFO(format, ...) \
    GEM::util::Logger::info(LOGGER_NAME, __PRETTY_FUNCTION__ + std::string(" [ ") + format + std::string(" ]"), __VA_ARGS__)

#define LOG_FUNCTION_ENTRY_WARNING(format, ...) \
    GEM::util::Logger::warning(LOGGER_NAME, __PRETTY_FUNCTION__ + std::string(" [ ") + format + std::string(" ]"), __VA_ARGS__)

#define LOG_FUNCTION_ENTRY_ERROR(format, ...) \
    GEM::util::Logger::error(LOGGER_NAME, __PRETTY_FUNCTION__ + std::string(" [ ") + format + std::string(" ]"), __VA_ARGS__)

#define LOG_FUNCTION_ENTRY_CRITICAL(format, ...) \
    GEM::util::Logger::critical(LOGGER_NAME, __PRETTY_FUNCTION__ + std::string(" [ ") + format + std::string(" ]"), __VA_ARGS__)

/**
 * @brief log a function call and its scope change
 * 
 * @note To use this LOGGER_NAME must be defined
 */

#define LOG_FUNCTION_CALL_TRACE(format, ...) \
    LOG_FUNCTION_ENTRY_TRACE(format, __VA_ARGS__); \
    LOG_SCOPE_CHANGE_TRACE()
    
#define LOG_FUNCTION_CALL_DEBUG(format, ...) \
    LOG_FUNCTION_ENTRY_DEBUG(format, __VA_ARGS__); \
    LOG_SCOPE_CHANGE_DEBUG()
    
#define LOG_FUNCTION_CALL_INFO(format, ...) \
    LOG_FUNCTION_ENTRY_INFO(format, __VA_ARGS__); \
    LOG_SCOPE_CHANGE_INFO()
    
#define LOG_FUNCTION_CALL_WARNING(format, ...) \
    LOG_FUNCTION_ENTRY_WARNING(format, __VA_ARGS__); \
    LOG_SCOPE_CHANGE_WARNING()
    
#define LOG_FUNCTION_CALL_ERROR(format, ...) \
    LOG_FUNCTION_ENTRY_ERROR(format, __VA_ARGS__); \
    LOG_SCOPE_CHANGE_ERROR()
    
#define LOG_FUNCTION_CALL_CRITICAL(format, ...) \
    LOG_FUNCTION_ENTRY_CRITICAL(format, __VA_ARGS__); \
    LOG_SCOPE_CHANGE_CRITICAL()

    