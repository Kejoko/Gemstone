#pragma once

#include <memory>
#include <string>

#include "gemstone/renderer/material/Material.hpp"
#include "gemstone/renderer/mesh/Mesh.hpp"

namespace GEM {
namespace Renderer{
    class Model;
}
}

class GEM::Renderer::Model {
public: // public static variables
    const static std::string LOGGER_NAME;

public: // public member functions
    Model(
        const std::string& meshFilename,
        const std::string& diffuseMapFilename,
        const std::string& specularMapFilename,
        const std::string& emissionMapFilename,
        const float shininess,
        const char* vertexShaderSource,
        const char* fragmentShaderSource
    );
    ~Model();

    std::shared_ptr<const GEM::Renderer::Mesh> getMeshPtr() const { return mp_mesh; }
    std::shared_ptr<const GEM::Renderer::Material> getMaterialPtr() const { return mp_material; }

    void draw() const;

private: // private static variables
    static uint32_t modelCount;

private: // private static functions

private: // private member variables
    const uint32_t m_id;

    std::shared_ptr<GEM::Renderer::Mesh> mp_mesh;
    std::shared_ptr<GEM::Renderer::Material> mp_material;

};