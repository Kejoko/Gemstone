#include "util/logger/Logger.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include "spdlog/async.h"
#include "spdlog/sinks/dist_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

/* ------------------------------ public static variables ------------------------------ */

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief A variable for us to determine how many indentaions we want to use for the message
 * we are logging
 */
uint32_t GEM::util::Logger::Scoper::indentationCount = 0;

/**
 * @brief A flag representing whether or not the spdlog stuff has been initialized
 */
bool GEM::util::Logger::initialized = false;

/**
 * @brief The spdlog sink pointers we initialize and givse to each of our loggers
 */
std::vector<spdlog::sink_ptr> GEM::util::Logger::sinkPtrs;

/**
 * @brief The map that is actually tracking the spdlog loggers we're using based on
 * the strings representing the logger names
 */
std::map<std::string, std::shared_ptr<spdlog::async_logger>> GEM::util::Logger::loggerPtrMap;

/* ------------------------------ public static functions ------------------------------ */

/**
 * @brief A function allowing us to initialize the logger so we don't need to do a
 * conditional check for initialization everytime we want to log something
 * time we want to 
 */
void GEM::util::Logger::init() {
    spdlog::init_thread_pool(8192, 1);

    // Create the stdout sink
    // Use trace as max detail (allow all types to go to stdout log)
    spdlog::sink_ptr p_stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    p_stdoutSink->set_level(spdlog::level::trace);

    // Create the file sink with the name determined by the current date and time
    // Use warnings as max detail (only warnings, errors, and critical go to file log)
    time_t time = std::time(nullptr);
    struct tm* p_currTime = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(p_currTime, "%Y.%m.%d.%H.%M.%S");
    std::string dateString = oss.str();
    std::string fileName = "GEMlog." + dateString + ".log";
    spdlog::sink_ptr p_fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName, true);
    p_fileSink->set_level(spdlog::level::warn);

    // Put both the stdout and file sink as sinks we want to use
    GEM::util::Logger::sinkPtrs.push_back(p_stdoutSink);
    GEM::util::Logger::sinkPtrs.push_back(p_fileSink);

    // Now that our sink pointers are initialized we are good to go
    GEM::util::Logger::initialized = true;
}

void GEM::util::Logger::registerLogger(const std::string& loggerName, const GEM::util::Logger::Level level) {
    if (!GEM::util::Logger::initialized) {
        GEM::util::Logger::init();
    }

    // Create the spdlog logger we are going to use
    std::shared_ptr<spdlog::async_logger> p_logger = std::make_shared<spdlog::async_logger>(
        loggerName,
        GEM::util::Logger::sinkPtrs.begin(),
        GEM::util::Logger::sinkPtrs.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );

    /**
     * @todo Use the DEBUG, TEST, RELEASE macros to clamp the logging levels to
     * a certain minimum value?
     */

    // Set the logging level
    switch (level) {
        case GEM::util::Logger::Level::trace:
            p_logger->set_level(spdlog::level::trace);
            break;
        case GEM::util::Logger::Level::debug:
            p_logger->set_level(spdlog::level::debug);
            break;
        case GEM::util::Logger::Level::info:
            p_logger->set_level(spdlog::level::info);
            break;
        case GEM::util::Logger::Level::warning:
            p_logger->set_level(spdlog::level::warn);
            break;
        case GEM::util::Logger::Level::error:
            p_logger->set_level(spdlog::level::err);
            break;
        case GEM::util::Logger::Level::critical:
            p_logger->set_level(spdlog::level::critical);
            break;
    }

    spdlog::register_logger(p_logger);

    // Set the format for the sinks
    spdlog::set_pattern("[%T:%e] [%=10n] [%^%=10l%$] %v");

    p_logger->info("Logger {} initialized", loggerName);

    GEM::util::Logger::loggerPtrMap[loggerName] = p_logger;
}

/**
 * @brief Register a new logger to the logger name and logger level contained in the info struct
 * 
 * @param loggerInfos The information (name and max level) pertaining to the logger
 */
void GEM::util::Logger::registerLoggers(const std::vector<const GEM::util::Logger::Info>& loggerInfos) {
    for (const GEM::util::Logger::Info& loggerInfo : loggerInfos) {
        GEM::util::Logger::registerLogger(loggerInfo.loggerName, loggerInfo.level);
    }
}

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief A function allowing us to ensure that we have been initialized prior to attempting
 * to log anything.
 * 
 * @todo Should we be doing this? Should we use c++'s assert instead?
 */
void GEM::util::Logger::assertInitialized() {
    if (!GEM::util::Logger::initialized) {
        std::string initializationErrorMessage = "GEM::util::Logger not initialized";
        #if defined(BUILD_DEBUG) || defined(BUILD_TEST)
        std::cerr << initializationErrorMessage << "\n";
        #endif
        throw std::runtime_error(initializationErrorMessage);
    }
}

/**
 * @brief Create a string to prepend to the message based on how many indents we
 * should be using
 * 
 * @return std::string The whitespace string 
 */
std::string GEM::util::Logger::createIndentationString() {
    return std::string(GEM::util::Logger::Scoper::getIndentationCount() * 4, ' ');
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new GEM::util::Logger::Scoper::Scoper object to denote a change
 * in scope when we are logging. This logs an opening curly brace and increments
 * the indentation counter so we know how much whitespace to print when logging
 * a message
 * 
 * @param level The level at which to log the opening/closing braces
 */
GEM::util::Logger::Scoper::Scoper(const std::string& loggerName, const GEM::util::Logger::Level level) :
    m_loggerName(loggerName),
    m_level(level)
{
    GEM::util::Logger::log(m_loggerName, m_level, "{");
    GEM::util::Logger::Scoper::indentationCount++;
}

/**
 * @brief Destroy the GEM::util::Logger::Scoper::Scoper object to denote that a scope
 * is ending. Decrement the indentation count and log a closing curly brace to denote
 * this
 */
GEM::util::Logger::Scoper::~Scoper() {
    GEM::util::Logger::Scoper::indentationCount--;
    GEM::util::Logger::log(m_loggerName, m_level, "}");
}

/* ------------------------------ private member functions ------------------------------ */
