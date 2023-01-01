#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/managers/input/logger.hpp"
#include "gemstone/managers/input/InputManager.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the input manager class uses
 */
const std::string GEM::InputManager::LOGGER_NAME = INPUT_MANAGER_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief The map of window contexts to input managers
 */
std::map<GLFWwindow* const, std::shared_ptr<GEM::InputManager>> GEM::InputManager::inputManagerPtrCallbackMap;

/**
 * @brief A map allowing the callback helper class to keep track of where each of the cursors was before
 * their most recent movement
 */
std::map<GLFWwindow* const, GEM::InputManager::CallbackHelper::CursorPosition> GEM::InputManager::CallbackHelper::lastCursorPositionMap;

/* ------------------------------ public static functions ------------------------------ */

/**
 * @brief Create a new InputManager. This adds the newly ceated InputManager pointer to the
 * map for the callback to use
 * 
 * @note This will throw if there is already a InputManager created for this glfw window
 * 
 * @param p_glfwWindow The glfw window pointer to craete the InputManager for
 * @return std::shared_ptr<GEM::InputManager> The shared pointer to the InputManager
 */
std::shared_ptr<GEM::InputManager> GEM::InputManager::createPtr(GLFWwindow* const p_glfwWindow) {
    LOG_FUNCTION_CALL_INFO("GLFW Window ptr {}", static_cast<void*>(p_glfwWindow));

    // Check if an instance has been created for this window so 
    if (GEM::InputManager::inputManagerPtrCallbackMap.count(p_glfwWindow) > 0) {
        // There already exists an InputManager for this window so we can't create one for it, uh oh D: :((((
        std::stringstream ss;
        ss << "InputManager with window [" << p_glfwWindow << "] already exists. Not creating new one";
        LOG_CRITICAL(ss.str());
        throw std::runtime_error(ss.str());
    }

    // There is no instance for this window yet
    {
        // Let's create one
        GEM::InputManager inputManager(p_glfwWindow);

        // Input it for fuure use
        GEM::InputManager::inputManagerPtrCallbackMap.insert({
            p_glfwWindow,
            std::make_shared<GEM::InputManager>(std::move(inputManager))
        });
    }

    LOG_TRACE("Updating input mode and input callbacks");

    // Make sure that the cursor is disabled when we have the context active
    glfwSetInputMode(p_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse input and scrolling callback helpers for this context
    glfwSetCursorPosCallback(p_glfwWindow, GEM::InputManager::CallbackHelper::cursorPositionInputCallback);
    glfwSetScrollCallback(p_glfwWindow, GEM::InputManager::CallbackHelper::scrollInputCallback);

    // Use the newly created one
    return GEM::InputManager::inputManagerPtrCallbackMap[p_glfwWindow];
}

/**
 * @brief Get the pointer to the already careated input manager associated with the GLFW Window pointer
 * 
 * @note If there is not InputManager associated with the GLFW Window pointer this will throw
 * 
 * @param p_glfwWindow The GLFW Window pointer for which we would like to find the associated
 * and already created input manager
 * @return std::shared_ptr<GEM::InputManager> The shared pointer to the already created InputManager
 */
std::shared_ptr<GEM::InputManager> GEM::InputManager::getPtr(GLFWwindow* const p_glfwWindow) {
    // Check if an instance has been created for this window
    if (GEM::InputManager::inputManagerPtrCallbackMap.count(p_glfwWindow) <= 0) {
        // No instance for this window, uh oh
        std::stringstream ss;
        ss << "InputManager with window [" << p_glfwWindow << "] does not exist";
        LOG_CRITICAL(ss.str());
        throw std::runtime_error(ss.str());
    }

    // Use the newly created one
    return GEM::InputManager::inputManagerPtrCallbackMap[p_glfwWindow];
}

/**
 * @brief Clear the maps of the input manager pointers
 */
void GEM::InputManager::clean() {
    LOG_TRACE("Clearing InputManager maps");
    GEM::InputManager::inputManagerPtrCallbackMap.clear();
}

/**
 * @brief The callback function opengl will be using whenever it polls a cursor movement event. This updates the necessary values in
 * the input manager responsible for the context
 * 
 * @param p_glfwWindow The context
 * @param currCursorXPos The current x position of the cursor
 * @param currCursorYPos The current y position of the cursor
 */
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

/**
 * @brief The callback function opengl will be using whenever it polls a scrolling event. This updates the necessary values in
 * the input manager responsible for the context
 * 
 * @param p_glfwWindow The context this callback is coming from
 * @param scrollXOffset The amount of scroll in the x direction
 * @param scrollYOffset The amount of scroll in the y direction
 */
void GEM::InputManager::CallbackHelper::scrollInputCallback(GLFWwindow* p_glfwWindow, double scrollXOffset, double scrollYOffset) {
    // Get the instance corresponding to this glfw window pointer
    std::shared_ptr<GEM::InputManager> p_inputManager = GEM::InputManager::getPtr(p_glfwWindow);

    // Update the offsets in the input manager
    p_inputManager->m_scrollXOffset = scrollXOffset;
    p_inputManager->m_scrollYOffset = scrollYOffset;
}

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Destroy the GEM::InputManager::InputManager object
 */
GEM::InputManager::~InputManager() {
    LOG_FUNCTION_CALL_TRACE("this ptr {}", static_cast<void*>(this));
}

/**
 * @brief Update the input states. Collect the key pressed and other input information from
 * the user so other components may use it
 */
void GEM::InputManager::collectInput() {
    // Reset the offsets to 0 before we collect mouse input, in case there is no mouse input this frame
    m_cursorXPosOffset = 0.0f;
    m_cursorYPosOffset = 0.0f;
    m_scrollXOffset = 0.0f;
    m_scrollYOffset = 0.0f;

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
    m_scrollXOffset(0.0f),
    m_scrollYOffset(0.0f),
    m_polygonWireframePressed(false),
    m_polygonFillPressed(false)
{
    LOG_FUNCTION_CALL_INFO("GLFW Window ptr {}", static_cast<void*>(p_glfwWindow));
}