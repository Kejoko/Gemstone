#include "gemstone/managers/input/InputManager.hpp"

#include <functional>
#include <map>
#include <memory>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"

/* ------------------------------ public static variables ------------------------------ */

/* ------------------------------ private static variables ------------------------------ */

std::map<GLFWwindow* const, std::shared_ptr<GEM::InputManager>> GEM::InputManager::glfwWindowPtrMap;

/* ------------------------------ public static functions ------------------------------ */

std::shared_ptr<GEM::InputManager> GEM::InputManager::getPtr(GLFWwindow* const p_glfwWindow) {
    // Check if an instance has been created for this window
    if (GEM::InputManager::glfwWindowPtrMap.count(p_glfwWindow)) {
        // There is an instance for this window, great, let's use it
        return GEM::InputManager::glfwWindowPtrMap[p_glfwWindow];
    }

    // There is no instance for this window yet
    {
        // Let's create one
        GEM::InputManager inputManager(p_glfwWindow);

        // Input it for fuure use
        GEM::InputManager::glfwWindowPtrMap.insert({
            p_glfwWindow,
            std::make_shared<GEM::InputManager>(std::move(inputManager))
        });
    }

    // Use the newly created one
    return GEM::InputManager::glfwWindowPtrMap[p_glfwWindow];
}

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

void GEM::InputManager::collectInput() {
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
    m_polygonWireframePressed(false),
    m_polygonFillPressed(false)
{}