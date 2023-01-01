#pragma once

#include <map>
#include <memory>
#include <string>

#include <GLFW/glfw3.h>

namespace GEM {
    class Context;
}

/**
 * @brief A singleton-esque class representing the glfw context. The context is created with the createPtr function.
 * After this, if you wish to get an already created context you must use the getPtr function.
 * At the end of the program's runtime we must call clean to get rid of all of the context's remaining in the maps.
 * 
 */
class GEM::Context {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public static functions
    static std::shared_ptr<Context> createPtr(const std::string& name, int initialWindowWidthPixels, int initialWindowHeightPixels);
    static std::shared_ptr<Context> getPtr(const std::string& name);
    static void clean();

public: // public member functions
    Context(const Context& other) = delete;
    void operator=(const Context& other) = delete;

    Context(Context&& other) = default;
    
    ~Context();

    std::string getName() const { return m_name; }
    int getWindowWidthPixels() const { return m_windowWidthPixels; }
    int getWindowHeightPixels() const { return m_windowHeightPixels; }
    std::shared_ptr<GLFWwindow> getGLFWWindowPtr() const { return mp_glfwWindow; }

private: // private static classes and enums
    class CallbackHelper {
    public: // public static functions
        static void windowResizeCallback(GLFWwindow* p_glfwWindow, int updatedWindowWidthPixels, int updatedWindowHeightPixels);

    public: // public member functions
        CallbackHelper() = delete;
    };

    friend class CallbackHelper;

private: // private static functions
    static std::shared_ptr<GLFWwindow> glfwInitCreateWindow(
        const std::string& windowTitle,
        int initialWindowWidthPixels,
        int initialWindowHeightPixels,
        const std::shared_ptr<GLFWmonitor> p_glfwMonitor,
        const std::shared_ptr<GLFWwindow> p_glfwSharedWindow
    );

private: // private static variables
    static bool glfwInitialized;
    static std::map<const std::string, std::shared_ptr<GEM::Context>> contextPtrMap;
    static std::map<GLFWwindow* const, std::shared_ptr<GEM::Context>> contextPtrCallbackMap;

private: // private member functions
    Context(const std::string& name, int initialWindowWidthPixels, int initialWindowHeightPixels);

private: // private member variables
    const std::string m_name;
    int m_windowWidthPixels;
    int m_windowHeightPixels;

    const std::shared_ptr<GLFWmonitor> mp_glfwMonitor;
    const std::shared_ptr<GLFWwindow> mp_glfwSharedWindow;
    const std::shared_ptr<GLFWwindow> mp_glfwWindow;
};
