#pragma once

#include <string>

#include <glm/glm.hpp>

#include "gemstone/light/Light.hpp"

namespace GEM {
    class DirectionalLight;
};

class GEM::DirectionalLight : public GEM::Light {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    DirectionalLight(
        const glm::vec3& initialDiffuseColor,
        const glm::vec3& initialSpecularColor,
        const glm::vec3& initialDirection
    );
    ~DirectionalLight();

    glm::vec3 getDirection() const { return m_direction; }

protected: // private member variables
    glm::vec3 m_direction;
};