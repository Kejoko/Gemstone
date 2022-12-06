#pragma once

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
public:
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

    Logger() = delete;

private:
    /**
     * @brief A function allowing us to ensure that we have been initialized prior to attempting
     * to log anything.
     * 
     * @todo Should we be doing this? Should we use c++'s assert instead?
     */
    static void assertInitialized();

    static bool m_initialized;
    static std::shared_ptr<spdlog::async_logger> mp_logger;
};
