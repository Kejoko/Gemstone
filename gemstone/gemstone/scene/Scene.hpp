#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gemstone/camera/Camera.hpp"
#include "gemstone/light/DirectionalLight.hpp"
#include "gemstone/light/PointLight.hpp"
#include "gemstone/light/SpotLight.hpp"
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
    const std::vector<std::shared_ptr<GEM::DirectionalLight>>& getDirectionalLightPtrs() const { return m_directionalLightPtrs; }
    const std::vector<std::shared_ptr<GEM::PointLight>>& getPointLightPtrs() const { return m_pointLightPtrs; }
    const std::vector<std::shared_ptr<GEM::SpotLight>>& getSpotLightPtrs() const { return m_spotLightPtrs; }
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
    static std::vector<std::shared_ptr<GEM::DirectionalLight>> loadDirectionalLights(
        const std::string& filename,
        const char* lightVertexShaderSource,
        const char* lightFragmentShaderSource
    );
    static std::vector<std::shared_ptr<GEM::PointLight>> loadPointLights(
        const std::string& filename,
        const char* lightVertexShaderSource,
        const char* lightFragmentShaderSource
    );
    static std::vector<std::shared_ptr<GEM::SpotLight>> loadSpotLights(
        const std::string& filename,
        const char* lightVertexShaderSource,
        const char* lightFragmentShaderSource
    );
    static std::vector<std::shared_ptr<GEM::Object>> loadObjects(
        const std::string& filename,
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
    std::vector<std::shared_ptr<GEM::DirectionalLight>> m_directionalLightPtrs;
    std::vector<std::shared_ptr<GEM::PointLight>> m_pointLightPtrs;
    std::vector<std::shared_ptr<GEM::SpotLight>> m_spotLightPtrs;
    std::vector<std::shared_ptr<GEM::Object>> m_objectPtrs;
};