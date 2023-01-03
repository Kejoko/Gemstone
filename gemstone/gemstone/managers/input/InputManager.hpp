#pragma once

#include <iostream>
#include <map>
#include <memory>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"

namespace GEM {
namespace Managers {
    class InputManager;
}
}

/**
 * @brief A singleton based input manager class. Each glfw window should have one
 * of these for managing inputs. There is a map of glfw window pointers to
 * instances of input managers so if we try to create an instance for which one
 * is already created we can just access the already created one. To create the
 * InputManager pointer use createPtr and to subsequently get the InputManager
 * pointer use getPtr. At the end of the program's runtime you should call clean().
 */
class GEM::Managers::InputManager {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public static functions
    static std::shared_ptr<InputManager> createPtr(GLFWwindow* const p_glfwWindow);
    static std::shared_ptr<InputManager> getPtr(GLFWwindow* const p_glfwWindow);
    static void clean();

public: // public member functions
    // Delete copy constructor and assignment operator so we aren't accidentally
    // duplicating instances
    InputManager(const InputManager& other) = delete;
    void operator=(const InputManager& other) = delete;

    // Want to be able to move into pointers and whatnot
    InputManager(InputManager&& other) = default;

    ~InputManager();

    void collectInput();

    bool getPausePressed() { return m_pausePressed; }
    bool getQuitPressed() { return m_quitPressed; }

    bool getForwardsPressed() { return m_forwardsPressed; }
    bool getBackwardsPressed() { return m_backwardsPressed; }
    bool getLeftPressed() { return m_leftPressed; }
    bool getRightPressed() { return m_rightPressed; }
    bool getJumpPressed() { return m_jumpPressed; }
    bool getCrouchPressed() { return m_crouchPressed; }

    float getCursorXPosOffset() { return m_cursorXPosOffset; }
    float getCursorYPosOffset() { return m_cursorYPosOffset; }
    float getScrollXOffset() { return m_scrollXOffset; }
    float getScrollYOffset() { return m_scrollYOffset; }

    bool getPolygonWireframePressed() { return m_polygonWireframePressed; }
    bool getPolygonFillPressed() { return m_polygonFillPressed; }

private: // private classes and enums
    /**
     * @brief A class to wrap around the mouse input callback function. This is to allow
     * us to modify instance level member variables within the InputManager from a static
     * callback.
     */
    class CallbackHelper {
    public: // public classes and enums
        struct CursorPosition {
            float xPos;
            float yPos;
        };

    public: // public static functions
        static void cursorPositionInputCallback(GLFWwindow* p_glfwWindow, double currCursorXPos, double currCursorYPos);
        static void scrollInputCallback(GLFWwindow* p_glfwWindow, double scrollXOffset, double scrollYOffset);

    public: // public member functions
        CallbackHelper() = delete;

    private: // private static variables
        static std::map<GLFWwindow* const, GEM::Managers::InputManager::CallbackHelper::CursorPosition> lastCursorPositionMap;
    };

    friend class CallbackHelper;

private: // private static variables
    // The mapping of glfw windows to input managers
    static std::map<GLFWwindow* const, std::shared_ptr<GEM::Managers::InputManager>> inputManagerPtrCallbackMap;

private: // private member functions
    // Only want to be able to create instances within getPtr function
    InputManager(GLFWwindow* const p_glfwWindow);
 
private: // private member variables
    GLFWwindow* const mp_glfwWindow;

    // Game state
    bool m_pausePressed;
    bool m_quitPressed;

    // Player movement (keys)
    bool m_forwardsPressed;
    bool m_backwardsPressed;
    bool m_leftPressed;
    bool m_rightPressed;
    bool m_jumpPressed;
    bool m_crouchPressed;

    // Mouse cursor and scroll information
    float m_cursorXPosOffset;
    float m_cursorYPosOffset;
    float m_scrollXOffset;
    float m_scrollYOffset;

    // Debugging
    bool m_polygonWireframePressed;
    bool m_polygonFillPressed;
};