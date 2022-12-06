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

uint32_t GEM::Logger::Scoper::indentationCount = 0;

bool GEM::Logger::m_initialized = false;
std::shared_ptr<spdlog::async_logger> GEM::Logger::mp_logger = nullptr;

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

void GEM::Logger::assertInitialized() {
    if (!m_initialized) {
        std::string initializationErrorMessage = "GEM::Logger not initialized";
        #if defined(BUILD_DEBUG) || defined(BUILD_TEST)
        std::cerr << initializationErrorMessage << "\n";
        #endif
        throw std::runtime_error(initializationErrorMessage);
    }
}

std::string GEM::Logger::createIndentationString() {
    return std::string(GEM::Logger::Scoper::getIndentationCount() * 4, ' ');
}

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

void GEM::Logger::warn(const std::string& message) {
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