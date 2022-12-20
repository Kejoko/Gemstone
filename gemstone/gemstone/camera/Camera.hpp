#pragma once

#include <string>

namespace GEM {
    class Camera;
}

class GEM::Camera {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    Camera();
    ~Camera();

private: // private member variables
};
