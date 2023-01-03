#include <string>
#include <vector>

#include "util/logger/Logger.hpp"

#include "gemstone/application/logger.hpp"
#include "gemstone/application/Application.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Application class uses
 */
const std::string GEM::Application::LOGGER_NAME = APPLICATION_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::Application::Application(const std::string& name, const GEM::Scene& scene) :
    m_name(name),
    m_scene(scene)
{}

GEM::Application::~Application() {}

void GEM::Application::run() {
    
    // Loop
        // User input

        // Update camera

        // Update world objects

        // Draw

}

/* ------------------------------ private member functions ------------------------------ */

