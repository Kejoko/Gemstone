#include "gemstone/managers/input/InputManager.hpp"

#include <functional>
#include <map>
#include <memory>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"

/* ------------------------------ public static variables ------------------------------ */

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief The map of window contexts to input managers
 */
std::map<GLFWwindow* const, std::shared_ptr<GEM::InputManager>> GEM::InputManager::inputManagerMap;

std::map<GLFWwindow* const, GEM::InputManager::CallbackHelper::CursorPosition> GEM::InputManager::CallbackHelper::lastCursorPositionMap;

/* ------------------------------ public static functions ------------------------------ */

/**
 * @brief Get a shared pointer to an instance of an input manager given the context.
 * If an input manager already exists for this context, then use that one. If not, create
 * a new one and add it to the map.
 * 
 * @param p_glfwWindow The pointer to the glfw window for which the input manager is
 * going to be responsible
 * @return std::shared_ptr<GEM::InputManager> The shared pointer to the input manager
 */
std::shared_ptr<GEM::InputManager> GEM::InputManager::getPtr(GLFWwindow* const p_glfwWindow) {
    // Check if an instance has been created for this window
    if (GEM::InputManager::inputManagerMap.count(p_glfwWindow)) {
        // There is an instance for this window, great, let's use it
        return GEM::InputManager::inputManagerMap[p_glfwWindow];
    }

    // There is no instance for this window yet
    {
        // Let's create one
        GEM::InputManager inputManager(p_glfwWindow);

        // Input it for fuure use
        GEM::InputManager::inputManagerMap.insert({
            p_glfwWindow,
            std::make_shared<GEM::InputManager>(std::move(inputManager))
        });
    }

    // Set the mouse input and scrolling callback helpers for this context
    glfwSetCursorPosCallback(p_glfwWindow, GEM::InputManager::CallbackHelper::cursorPositionInputCallback);

    // Use the newly created one
    return GEM::InputManager::inputManagerMap[p_glfwWindow];
}

void GEM::InputManager::CallbackHelper::cursorPositionInputCallback(GLFWwindow* p_glfwWindow, double currCursorXPos, double currCursorYPos) {
    // Get the instance corresponding to this glfw window pointer
    std::shared_ptr<GEM::InputManager> p_inputManager = GEM::InputManager::getPtr(p_glfwWindow);

    // Get the last positions stored in the map
    if (GEM::InputManager::CallbackHelper::lastCursorPositionMap.count(p_glfwWindow) == 0) {
        // This is the first time the mouse has moved so there isn't a previous position stored
        // This means the offset is 0 and we must initialize the last position as the current position
        GEM::InputManager::CallbackHelper::lastCursorPositionMap.insert({
            p_glfwWindow,
            { static_cast<float>(currCursorXPos), static_cast<float>(currCursorYPos) }
        });
    }

    // Calculate the offset using the last position
    GEM::InputManager::CallbackHelper::CursorPosition lastPosition = GEM::InputManager::CallbackHelper::lastCursorPositionMap[p_glfwWindow];
    p_inputManager->m_cursorXPosOffset = lastPosition.xPos - currCursorXPos;
    p_inputManager->m_cursorYPosOffset = lastPosition.yPos - currCursorYPos;

    // Update the last positions in the map to be the current position
    GEM::InputManager::CallbackHelper::lastCursorPositionMap[p_glfwWindow] = {
        static_cast<float>(currCursorXPos),
        static_cast<float>(currCursorYPos)
    };
}

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Update the input states. Collect the key pressed and other input information from
 * the user so other components may use it
 */
void GEM::InputManager::collectInput() {
    // Reset the offsets to 0 before we collect mouse input, in case there is no mouse input this frame
    m_cursorXPosOffset = 0.0f;
    m_cursorYPosOffset = 0.0f;
    m_mouseXScrollOffset = 0.0f;
    m_mouseYScrollOffset = 0.0f;

    glfwPollEvents();

    m_pausePressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_ESCAPE);
    m_quitPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_Q);

    m_forwardsPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_W);
    m_backwardsPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_S);
    m_leftPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_A);
    m_rightPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_D);
    m_jumpPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_SPACE);
    m_crouchPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_LEFT_SHIFT);

    m_polygonWireframePressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_2);
    m_polygonFillPressed = glfwGetKey(mp_glfwWindow, GLFW_KEY_1);
}

/* ------------------------------ private member functions ------------------------------ */

/**
 * @brief Construct a new GEM::InputManager::InputManager object from a given context
 * 
 * @param p_glfwWindow The context for which the input manager will be responsible
 */
GEM::InputManager::InputManager(GLFWwindow* const p_glfwWindow) :
    mp_glfwWindow(p_glfwWindow),
    m_pausePressed(false),
    m_quitPressed(false),
    m_forwardsPressed(false),
    m_backwardsPressed(false),
    m_leftPressed(false),
    m_rightPressed(false),
    m_jumpPressed(false),
    m_crouchPressed(false),
    m_cursorXPosOffset(0.0f),
    m_cursorYPosOffset(0.0f),
    m_mouseXScrollOffset(0.0f),
    m_mouseYScrollOffset(0.0f),
    m_polygonWireframePressed(false),
    m_polygonFillPressed(false)
{}