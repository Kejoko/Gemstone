#pragma once

#include <iostream>
#include <memory>

#include "spdlog/async.h"
#include "spdlog/spdlog.h"

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
public: // public classes
    class Scoper {
    public: // public static
        static uint32_t getIndentationCount() { return GEM::Logger::Scoper::indentationCount; }

    private: // private static
        static uint32_t indentationCount;

    public: // public members
        Scoper() {
            // Log an opening curly brace to the correct logger at the correct level
            GEM::Logger::trace("{");

            // Increment the indentation count so the logger knows how much to indent before printing
            GEM::Logger::Scoper::indentationCount++;
        }

        ~Scoper() {
            // Decrement the indentation count so the logger knows how much to indent before printing
            GEM::Logger::Scoper::indentationCount--;

            // Log a closing curly brace to the correct logger at the correct level
            GEM::Logger::trace("}");
        }
    };

public: // public static
    /**
     * @brief A function allowing us to initialize the logger so we don't need to do a
     * conditional check for initialization everytime we want to log something
     * time we want to 
     */
    static void init();

    static void trace(const std::string& message);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);

private: // private static
    /**
     * @brief A function allowing us to ensure that we have been initialized prior to attempting
     * to log anything.
     * 
     * @todo Should we be doing this? Should we use c++'s assert instead?
     */
    static void assertInitialized();

    static std::string createIndentationString();

    static bool m_initialized;
    static std::shared_ptr<spdlog::async_logger> mp_logger;

public: // public members
    Logger() = delete;
};

// ----- logging a scope ----- //

// With the correct logger and at the correct logging level:
// 1. Log an opening curly brace {
// 2. Increment the indentation count
// ...
// 3. Decrement the indentation count
// 4. Log a closing curly brace }

#define LOG_SCOPE_CHANGE() \
    const GEM::Logger::Scoper UNIQUE_NAME(scoper)

// ----- logging a function call ----- //

// With the correct logger and at the correct logging level:
// 1. Log the function information
// 2. Log a scope

#define LOG_FUNCTION_CALL(message) \
    GEM::Logger::info(__PRETTY_FUNCTION__ + std::string(" [ ") + message + std::string(" ]")); \
    LOG_SCOPE_CHANGE()
    