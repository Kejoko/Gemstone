#pragma once

#include <string>
#include <vector>

#include "gemstone/mesh/Mesh.hpp"

namespace GEM {
    class Scene;
}

class GEM::Scene {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    Scene(const std::string& name, const std::vector<GEM::Mesh>& meshes);
    ~Scene();

private: // private static variables
    static uint32_t sceneCount;

private: // private member variables
    const uint32_t m_id;
    const std::string m_name;
    
    std::vector<GEM::Mesh> m_meshes;
};