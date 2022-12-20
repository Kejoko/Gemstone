#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace GEM {
    class Mesh;
}

class GEM::Mesh {
public: // public static variables
    const static std::string LOGGER_NAME;

public: // public member functions
    Mesh();
    Mesh(const glm::vec3& worldPosition);
    ~Mesh();

    void draw();

    glm::vec3 getWorldPosition() const { return m_worldPosition; }

private: // private static functions
    static std::vector<float> loadVertices();
    static std::vector<float> loadVertices(const glm::vec3& getWorldPosition);
    static uint32_t createVertexArrayObject();
    static uint32_t createVertexBufferObject(const std::vector<float>& vertices);
    static uint32_t createElementBufferObject(const std::vector<float>& vertices);
    static void configureVertexAttributePointers();

private: // private member variables
    const std::vector<float> m_vertices;
    const uint32_t m_vertexArrayObjectID;
    const uint32_t m_vertexBufferObjectID;
    const uint32_t m_elementBufferObjectID;
    const glm::vec3 m_worldPosition;
};