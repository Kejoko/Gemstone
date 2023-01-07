#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gemstone/camera/Camera.hpp"
#include "gemstone/light/Light.hpp"
#include "gemstone/object/Object.hpp"
#include "gemstone/managers/input/InputManager.hpp"
#include "gemstone/renderer/context/Context.hpp"
#include "gemstone/renderer/mesh/Mesh.hpp"
#include "gemstone/renderer/shader/ShaderProgram.hpp"

namespace GEM {
    class Scene;
}

class GEM::Scene {
public: // public static classes and enums
    struct AmbientLight {
        glm::vec3 color;
        float strength;
    };

public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    Scene(
        std::shared_ptr<GEM::Renderer::Context> p_context,
        std::shared_ptr<GEM::Managers::InputManager> p_inputManager,
        const std::string& filename,
        std::shared_ptr<GEM::Renderer::ShaderProgram> p_lightShader,
        std::shared_ptr<GEM::Renderer::ShaderProgram> p_objectShader,
        const char* lightVertexShaderSource,
        const char* lightFragmentShaderSource,
        const char* objectVertexShaderSource,
        const char* objectFragmentShaderSource
    );
    ~Scene();

    uint32_t getID() const { return m_id; }
    std::string getFilename() const { return m_filename; }
    std::string getName() const { return m_name; }

    std::shared_ptr<const GEM::Camera> getCameraPtr() const { return mp_camera; }
    GEM::Scene::AmbientLight getAmbientLight() const { return m_ambientLight; }
    const std::vector<std::shared_ptr<GEM::Light>>& getLightPtrs() const { return m_lightPtrs; }
    const std::vector<std::shared_ptr<GEM::Object>>& getObjectPtrs() const { return m_objectPtrs; }

    void update();

private: // private static functions
    static std::string loadName(const std::string& filename);
    static std::shared_ptr<GEM::Camera> loadCamera(
        std::shared_ptr<GEM::Renderer::Context> p_context,
        std::shared_ptr<GEM::Managers::InputManager> p_inputManager,
        const std::string& filename
    );
    static GEM::Scene::AmbientLight loadAmbientLight(const std::string& filename);
    static std::vector<std::shared_ptr<GEM::Light>> loadLights(
        const std::string& filename,
        std::shared_ptr<GEM::Renderer::ShaderProgram> p_lightShader,
        const char* lightVertexShaderSource,
        const char* lightFragmentShaderSource
    );
    static std::vector<std::shared_ptr<GEM::Object>> loadObjects(
        const std::string& filename,
        std::shared_ptr<GEM::Renderer::ShaderProgram> p_objectShader,
        const char* objectVertexShaderSource,
        const char* objectFragmentShaderSource
    );

private: // private static variables
    static uint32_t sceneCount;

private: // private member variables
    const uint32_t m_id;
    const std::string m_filename;
    const std::string m_name;

    const std::shared_ptr<GEM::Renderer::Context> mp_context;
    const std::shared_ptr<GEM::Managers::InputManager> mp_inputManager;
    
    std::shared_ptr<GEM::Camera> mp_camera;
    GEM::Scene::AmbientLight m_ambientLight;
    std::vector<std::shared_ptr<GEM::Light>> m_lightPtrs;
    std::vector<std::shared_ptr<GEM::Object>> m_objectPtrs;
};