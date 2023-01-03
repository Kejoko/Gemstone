#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "gemstone/renderer/mesh/Mesh.hpp"
#include "gemstone/renderer/shader/ShaderProgram.hpp"
#include "gemstone/renderer/texture/Texture.hpp"

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
        const std::string& textureFilename,
        const std::string& textureFilename2,
        std::shared_ptr<GEM::Renderer::ShaderProgram> p_shaderProgram,
        const glm::vec3& initialWorldPosition,
        const glm::vec3& initialScale,
        const glm::vec3& initialRotationAxis,
        const float initialRotationAmountDegrees
    );
    ~Object();

    glm::vec3 getWorldPosition() const { return m_worldPosition; }
    glm::vec3 getScale() const { return m_scale; }
    glm::mat4 getModelMatrix() const;

    std::shared_ptr<const GEM::Renderer::Texture> getTexturePtr() const { return mp_texture; }
    std::shared_ptr<const GEM::Renderer::Texture> getTexture2Ptr() const { return mp_texture2; }
    std::shared_ptr<GEM::Renderer::ShaderProgram> getShaderProgramPtr() const { return mp_shaderProgram; }

    void update();
    void draw();

private: // private member functions
    std::shared_ptr<GEM::Renderer::Mesh> loadMesh(const std::string& meshFilename);
    std::shared_ptr<GEM::Renderer::Texture> loadTexture(const std::string& textureFilename, const uint32_t index);

private: // private member variables
    const uint32_t m_id;
    const std::string m_meshFilename;
    const std::string m_textureFilename;
    const std::string m_textureFilename2;

    std::shared_ptr<GEM::Renderer::Mesh> mp_mesh;
    std::shared_ptr<GEM::Renderer::Texture> mp_texture;
    std::shared_ptr<GEM::Renderer::Texture> mp_texture2;
    std::shared_ptr<GEM::Renderer::ShaderProgram> mp_shaderProgram;

    glm::vec3 m_worldPosition;
    glm::vec3 m_scale;
    glm::vec3 m_rotationAxis;
    float m_rotationAmountDegrees;
};