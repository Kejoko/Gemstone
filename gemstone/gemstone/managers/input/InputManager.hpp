#pragma once

#include <iostream>
#include <map>
#include <memory>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"

namespace GEM {
    class InputManager;
}

/**
 * @brief A singleton based input manager class. Each glfw window should have one
 * of these for managing inputs. There is a map of glfw window pointers to
 * instances of input managers so if we try to create an instance for which one
 * is already created we can just access the already created one.
 */
class GEM::InputManager {
public: // public classes and enums

public: // public static functions
    static std::shared_ptr<InputManager> getPtr(GLFWwindow* const p_glfwWindow);

public: // public member functions
    // Delete copy constructor and assignment operator so we aren't accidentally
    // duplicating instances
    InputManager(const InputManager& other) = delete;
    void operator=(const InputManager& other) = delete;

    // Want to be able to move into pointers and whatnot
    InputManager(InputManager&& other) = default;

    void collectInput();

    bool getPausePressed() { return m_pausePressed; }
    bool getQuitPressed() { return m_quitPressed; }

    bool getForwardsPressed() { return m_forwardsPressed; }
    bool getBackwardsPressed() { return m_backwardsPressed; }
    bool getLeftPressed() { return m_leftPressed; }
    bool getRightPressed() { return m_rightPressed; }
    bool getJumpPressed() { return m_jumpPressed; }
    bool getCrouchPressed() { return m_crouchPressed; }

    float getMouseXPosOffset() { return m_mouseXPosOffset; }
    float getMouseYPosOffset() { return m_mouseYPosOffset; }

    bool getPolygonWireframePressed() { return m_polygonWireframePressed; }
    bool getPolygonFillPressed() { return m_polygonFillPressed; }

private: // private classes and enums
    /**
     * @brief A class to wrap around the mouse input callback function. This is to allow
     * us to modify instance level member variables within the InputManager from a static
     * callback.
     */
    class MouseInputCallbackHelper {
    public: // public classes and enums
        struct Position {
            float xPos;
            float yPos;
        };

    public: // public static functions
        static void mouseInputCallback(GLFWwindow* p_glfwWindow, double mouseXPos, double mouseYPos) {
            // Get the instance corresponding to this glfw window pointer
            std::shared_ptr<GEM::InputManager> p_inputManager = GEM::InputManager::getPtr(p_glfwWindow);

            // Get the last positions stored in the map
            if (GEM::InputManager::MouseInputCallbackHelper::lastPositionMap.count(p_glfwWindow) == 0) {
                std::cout << "inserting initial last position into map\n";
                // This is the first time the mouse has moved so there isn't a previous position stored
                // This means the offset is 0 and we must initialize the last position as the current position
                GEM::InputManager::MouseInputCallbackHelper::lastPositionMap.insert({
                    p_glfwWindow,
                    { static_cast<float>(mouseXPos), static_cast<float>(mouseYPos) }
                });
            }

            // Calculate the offset using the last position
            GEM::InputManager::MouseInputCallbackHelper::Position lastPosition = GEM::InputManager::MouseInputCallbackHelper::lastPositionMap[p_glfwWindow];
            p_inputManager->m_mouseXPosOffset = lastPosition.xPos - mouseXPos;
            p_inputManager->m_mouseYPosOffset = lastPosition.yPos - mouseYPos;

            std::cout << "offest: x ( " << p_inputManager->m_mouseXPosOffset << " ) , y ( " << p_inputManager->m_mouseYPosOffset << " )\n";

            // Update the last positions in the map to be the current position
            GEM::InputManager::MouseInputCallbackHelper::lastPositionMap[p_glfwWindow] = {
                static_cast<float>(mouseXPos),
                static_cast<float>(mouseYPos)
            };
        }

    public: // public member functions
        MouseInputCallbackHelper() = delete;

    private: // private static variables
        static std::map<GLFWwindow* const, GEM::InputManager::MouseInputCallbackHelper::Position> lastPositionMap;
    };

    friend class MouseInputCallbackHelper;

private: // private static variables
    // The mapping of glfw windows to input managers
    static std::map<GLFWwindow* const, std::shared_ptr<GEM::InputManager>> inputManagerMap;

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

    // Player orientation (mouse and scroll)
    float m_mouseXPosOffset;
    float m_mouseYPosOffset;
    float m_mouseXScrollOffset;
    float m_mouseYScrollOffset;

    // Debugging
    bool m_polygonWireframePressed;
    bool m_polygonFillPressed;
};