#include "util/logger/Logger.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

/* ------------------------------ public static variables ------------------------------ */

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief A variable for us to determine how many indentaions we want to use for the message
 * we are logging
 */
uint32_t GEM::Logger::Scoper::indentationCount = 0;

/**
 * @brief A flag representing whether or not the logger has been initialized
 */
bool GEM::Logger::m_initialized = false;

/**
 * @brief The variable that is actually tracking the spdlog logger we're using
 */
std::shared_ptr<spdlog::async_logger> GEM::Logger::mp_logger = nullptr;

/* ------------------------------ public static functions ------------------------------ */

void GEM::Logger::trace(const std::string& message) {
    assertInitialized();
    mp_logger->trace(GEM::Logger::createIndentationString() + message);
}

void GEM::Logger::debug(const std::string& message) {
    assertInitialized();
    mp_logger->debug(GEM::Logger::createIndentationString() + message);
}

void GEM::Logger::info(const std::string& message) {
    assertInitialized();
    mp_logger->info(GEM::Logger::createIndentationString() + message);
}

void GEM::Logger::warning(const std::string& message) {
    assertInitialized();
    mp_logger->warn(GEM::Logger::createIndentationString() + message);
}

void GEM::Logger::error(const std::string& message) {
    assertInitialized();
    mp_logger->error(GEM::Logger::createIndentationString() + message);
}

void GEM::Logger::critical(const std::string& message) {
    assertInitialized();
    mp_logger->critical(GEM::Logger::createIndentationString() + message);
}

/**
 * @brief Log a message based on a logging level parameter. This is useful for
 * logging functions which might use different levels to log the same thing
 * 
 * @param level The logging level to use
 * @param message The message to log
 */
void GEM::Logger::log(const GEM::Logger::Level level, const std::string& message) {
    switch (level) {
        case GEM::Logger::Level::trace:
            GEM::Logger::trace(message);
            break;
        case GEM::Logger::Level::debug:
            GEM::Logger::debug(message);
            break;
        case GEM::Logger::Level::info:
            GEM::Logger::info(message);
            break;
        case GEM::Logger::Level::warning:
            GEM::Logger::warning(message);
            break;
        case GEM::Logger::Level::error:
            GEM::Logger::error(message);
            break;
        case GEM::Logger::Level::critical:
            GEM::Logger::critical(message);
            break;
    }
}

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief A function allowing us to initialize the logger so we don't need to do a
 * conditional check for initialization everytime we want to log something
 * time we want to 
 */
void GEM::Logger::init() {
    if (m_initialized) {
        return;
    }
    
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
    std::vector<spdlog::sink_ptr> sinkPtrs;
    sinkPtrs.push_back(p_stdoutSink);
    sinkPtrs.push_back(p_fileSink);

    // Create the spdlog logger we are going to use
    mp_logger = std::make_shared<spdlog::async_logger>(
        "master logger",
        sinkPtrs.begin(),
        sinkPtrs.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );

    // Set the logging level
    #if defined(DEBUG)
    mp_logger->set_level(spdlog::level::trace);
    #elif defined(TEST)
    mp_logger->set_level(spdlog::level::info);
    #elif defined(RELEASE)
    mp_logger->set_level(spdlog::level::err);
    #endif

    spdlog::register_logger(mp_logger);

    spdlog::set_pattern("[%T:%e] [%^%=10l%$] %v");

    m_initialized = true;
    mp_logger->info("Logger initialized. Using logfile: " + fileName);
}

/**
 * @brief A function allowing us to ensure that we have been initialized prior to attempting
 * to log anything.
 * 
 * @todo Should we be doing this? Should we use c++'s assert instead?
 */
void GEM::Logger::assertInitialized() {
    if (!m_initialized) {
        std::string initializationErrorMessage = "GEM::Logger not initialized";
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
std::string GEM::Logger::createIndentationString() {
    return std::string(GEM::Logger::Scoper::getIndentationCount() * 4, ' ');
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new GEM::Logger::Scoper::Scoper object to denote a change
 * in scope when we are logging. This logs an opening curly brace and increments
 * the indentation counter so we know how much whitespace to print when logging
 * a message
 * 
 * @param level The level at which to log the opening/closing braces
 */
GEM::Logger::Scoper::Scoper(const GEM::Logger::Level level) :
    m_level(level)
{
    GEM::Logger::log(m_level, "{");
    GEM::Logger::Scoper::indentationCount++;
}

/**
 * @brief Destroy the GEM::Logger::Scoper::Scoper object to denote that a scope
 * is ending. Decrement the indentation count and log a closing curly brace to denote
 * this
 */
GEM::Logger::Scoper::~Scoper() {
    GEM::Logger::Scoper::indentationCount--;
    GEM::Logger::log(m_level, "}");
}

/* ------------------------------ private member functions ------------------------------ */
