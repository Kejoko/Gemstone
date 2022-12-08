#pragma once

#include <iostream>
#include <memory>

#include "spdlog/async.h"
#include "spdlog/spdlog.h"

#include "util/macros.hpp"

namespace GEM {
    class Logger;
}

/**
 * @brief A singleton logger wrapping a spdlog async logger
 * 
 * @todo Allow for the ability to have different loggers. Perhaps one for each component (ex:
 * one for renderer, one for physics, one for networking, etc...) which we can manually change
 * the levels for each so we don't get all logs from everything. All this will require is
 * mainting a map of strings/indices to shared pointer instead of a single shared pointer
 * 
 * @todo Allow us to specify the format of the logs and the log file upon creation in case we
 * want to use a different thing than what we are doing now
 * 
 * @todo ALlow us to set stdout levels vs file levels differently. Currently file is always at warn
 * while stdout is what changes based on build mode
 */
class GEM::Logger {
public: // public classes and enums
    enum class Level {
        trace,
        debug,
        info,
        warning,
        error,
        critical
    };

    class Scoper {
    public: // public static functions
        static uint32_t getIndentationCount() { return GEM::Logger::Scoper::indentationCount; }

    private: // private static variables
        static uint32_t indentationCount;

    public: // public member functions
        Scoper(const GEM::Logger::Level level);
        ~Scoper();

    private: // private member variables
        const GEM::Logger::Level m_level;
    };

public: // public static functions
    static void init();

    static void trace(const std::string& message);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);

    static void log(const GEM::Logger::Level level, const std::string& message);

private: // private static functions
    static void assertInitialized();

    static std::string createIndentationString();

public: // public members
    Logger() = delete;

private: // private static members

    static bool m_initialized;
    static std::shared_ptr<spdlog::async_logger> mp_logger;
};

// ----- log a scope change ----- //

#define LOG_SCOPE_CHANGE_TRACE() \
    const GEM::Logger::Scoper UNIQUE_NAME(scoper)(GEM::Logger::Level::trace)

#define LOG_SCOPE_CHANGE_DEBUG() \
    const GEM::Logger::Scoper UNIQUE_NAME(scoper)(GEM::Logger::Level::debug)

#define LOG_SCOPE_CHANGE_INFO() \
    const GEM::Logger::Scoper UNIQUE_NAME(scoper)(GEM::Logger::Level::info)

#define LOG_SCOPE_CHANGE_WARNING() \
    const GEM::Logger::Scoper UNIQUE_NAME(scoper)(GEM::Logger::Level::warning)

#define LOG_SCOPE_CHANGE_ERROR() \
    const GEM::Logger::Scoper UNIQUE_NAME(scoper)(GEM::Logger::Level::error)

#define LOG_SCOPE_CHANGE_CRITICAL() \
    const GEM::Logger::Scoper UNIQUE_NAME(scoper)(GEM::Logger::Level::critical)

/**
 * @brief log a function call *WITHOUT* scope change
 */

#define LOG_FUNCTION_ENTRY_TRACE(message) \
    GEM::Logger::trace(__PRETTY_FUNCTION__ + std::string(" [ ") + message + std::string(" ]"))

#define LOG_FUNCTION_ENTRY_DEBUG(message) \
    GEM::Logger::debug(__PRETTY_FUNCTION__ + std::string(" [ ") + message + std::string(" ]"))

#define LOG_FUNCTION_ENTRY_INFO(message) \
    GEM::Logger::info(__PRETTY_FUNCTION__ + std::string(" [ ") + message + std::string(" ]"))

#define LOG_FUNCTION_ENTRY_WARNING(message) \
    GEM::Logger::warning(__PRETTY_FUNCTION__ + std::string(" [ ") + message + std::string(" ]"))

#define LOG_FUNCTION_ENTRY_ERROR(message) \
    GEM::Logger::error(__PRETTY_FUNCTION__ + std::string(" [ ") + message + std::string(" ]"))

#define LOG_FUNCTION_ENTRY_CRITICAL(message) \
    GEM::Logger::critical(__PRETTY_FUNCTION__ + std::string(" [ ") + message + std::string(" ]"))

/**
 * @brief log a function call and its scope change
 */

#define LOG_FUNCTION_CALL_TRACE(message) \
    LOG_FUNCTION_ENTRY_TRACE(message); \
    LOG_SCOPE_CHANGE_TRACE()
    
#define LOG_FUNCTION_CALL_DEBUG(message) \
    LOG_FUNCTION_ENTRY_DEBUG(message); \
    LOG_SCOPE_CHANGE_DEBUG()
    
#define LOG_FUNCTION_CALL_INFO(message) \
    LOG_FUNCTION_ENTRY_INFO(message); \
    LOG_SCOPE_CHANGE_INFO()
    
#define LOG_FUNCTION_CALL_WARNING(message) \
    LOG_FUNCTION_ENTRY_WARNING(message); \
    LOG_SCOPE_CHANGE_WARNING()
    
#define LOG_FUNCTION_CALL_ERROR(message) \
    LOG_FUNCTION_ENTRY_ERROR(message); \
    LOG_SCOPE_CHANGE_ERROR()
    
#define LOG_FUNCTION_CALL_CRITICAL(message) \
    LOG_FUNCTION_ENTRY_CRITICAL(message); \
    LOG_SCOPE_CHANGE_CRITICAL()

    