#pragma once

#include <string>
#include <vector>

#include "gemstone/scene/Scene.hpp"

namespace GEM {
    class Application;
}

class GEM::Application {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    Application(const std::string& name, const GEM::Scene& scenes);
    ~Application();

    void run();

private: // private member variables
    const std::string m_name;

    GEM::Scene m_scene;
};
