#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "gemstone/renderer/model/Model.hpp"

namespace GEM {
    class Object;
}

class GEM::Object {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    Object(
        const uint32_t id,
        const std::string& meshFilename,
        const std::string& diffuseMapFilename,
        const std::string& specularMapFileName,
        const std::string& emissionMapFilename,
        const float shininess,
        const char* vertexShaderSource,
        const char* fragmentShaderSource,
        const glm::vec3& initialWorldPosition,
        const glm::vec3& initialScale,
        const glm::vec3& initialRotationAxis,
        const float initialRotationAmountDegrees
    );
    ~Object();

    glm::vec3 getWorldPosition() const { return m_worldPosition; }
    glm::vec3 getScale() const { return m_scale; }
    glm::mat4 getModelMatrix() const;

    std::shared_ptr<const GEM::Renderer::Model> getModelPtr() const { return mp_model; }

    void update();
    void draw() const;

private: // private member functions
    std::shared_ptr<GEM::Renderer::Mesh> loadMesh(const std::string& meshFilename);
    std::shared_ptr<GEM::Renderer::Texture> loadTexture(const std::string& textureFilename, const uint32_t index);

private: // private member variables
    const uint32_t m_id;
    
    std::shared_ptr<GEM::Renderer::Model> mp_model;

    glm::vec3 m_worldPosition;
    glm::vec3 m_scale;
    glm::vec3 m_rotationAxis;
    float m_rotationAmountDegrees;
};