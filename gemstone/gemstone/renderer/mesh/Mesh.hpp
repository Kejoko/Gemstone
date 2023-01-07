#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace GEM {
namespace Renderer{
    class Mesh;
}
}

class GEM::Renderer::Mesh {
public: // public static variables
    const static std::string LOGGER_NAME;

public: // public member functions
    Mesh();
    ~Mesh();

    void draw() const;

private: // private static functions
    static std::vector<float> loadVertices();
    static uint32_t createVertexArrayObject();
    static uint32_t createVertexBufferObject(const std::vector<float>& vertices);
    static uint32_t createElementBufferObject(const std::vector<float>& vertices);
    static void configureVertexAttributePointers();

private: // private member variables
    const std::vector<float> m_vertices;
    const uint32_t m_vertexArrayObjectID;
    const uint32_t m_vertexBufferObjectID;
    const uint32_t m_elementBufferObjectID;
};