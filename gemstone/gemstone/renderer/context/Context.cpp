#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/core.hpp"
#include "gemstone/renderer/context/logger.hpp"
#include "gemstone/renderer/context/Context.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the context class uses
 */
const std::string GEM::Renderer::Context::LOGGER_NAME = CONTEXT_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief The flag keeping track of whether or not we have already initialized glfw so we
 * know not to do it on subsequent context creations
 */
bool GEM::Renderer::Context::glfwInitialized = false;

/**
 * @brief The map of window names to contexts
 */
std::map<const std::string, std::shared_ptr<GEM::Renderer::Context>> GEM::Renderer::Context::contextPtrMap;

/**
 * @brief The map of glfw windows to contexts
 */
std::map<GLFWwindow* const, std::shared_ptr<GEM::Renderer::Context>> GEM::Renderer::Context::contextPtrCallbackMap;

/* ------------------------------ public static functions ------------------------------ */

/**
 * @brief Create a new shared pointer to a context object
 * 
 * @details This adds the newly created context pointer to the two maps managed within Context to
 * retrieve already created Contexts by name and by which GLFW Window pointer they are responsible for.
 * This will throw if a context is already created for the given name
 * 
 * @param name The name of the window
 * @param initialWindowWidthPixels The initial width of the window in pixels
 * @param initialWindowHeightPixels The initial width of the window in pixels
 * @return std::shared_ptr<GEM::Renderer::Context> The shared pointer to the newly created context
 */
std::shared_ptr<GEM::Renderer::Context> GEM::Renderer::Context::createPtr(const std::string& name, int initialWindowWidthPixels, int initialWindowHeightPixels) {
    LOG_FUNCTION_CALL_INFO("name {} , initial window width pixels {} , initial window height pixels {}", name, initialWindowWidthPixels, initialWindowHeightPixels);
    
    // Check if one already exists, if it does then just use that one
    if (GEM::Renderer::Context::contextPtrMap.count(name) > 0) {
        const std::string contextExistsError = "Context with name [" + name + "] already exists. Not creating new one";
        LOG_CRITICAL(contextExistsError);
        throw std::runtime_error(contextExistsError);
    }

    // Create a shared pointer to a context object
    std::shared_ptr<GEM::Renderer::Context> p_context;
    {
        GEM::Renderer::Context context(name, initialWindowWidthPixels, initialWindowHeightPixels);
        p_context = std::make_shared<GEM::Renderer::Context>(std::move(context));
    }

    LOG_TRACE("Updating context maps");

    // Update the maps
    GEM::Renderer::Context::contextPtrMap.insert({name, p_context});
    GEM::Renderer::Context::contextPtrCallbackMap.insert({p_context->getGLFWWindowPtr().get(), p_context});

    // Give the shared pointer to the caller
    LOG_DEBUG("Context ptr {}", static_cast<void*>(p_context.get()));
    return p_context;
}

/**
 * @brief Get the context associated with the given name.
 * 
 * @details This will throw if no context exists for the associated name
 * 
 * @param name The name of the desired context
 * @return std::shared_ptr<GEM::Renderer::Context> The shared pointer to the context associated with
 * the given name
 */
std::shared_ptr<GEM::Renderer::Context> GEM::Renderer::Context::getPtr(const std::string& name) {
    LOG_FUNCTION_CALL_INFO("name {}", name);

    if (GEM::Renderer::Context::contextPtrMap.count(name) <= 0) {
        const std::string contextDoesntExistError = "Context with name [" + name + "] doesn't exist" ;
        LOG_ERROR(contextDoesntExistError);
        throw std::runtime_error(contextDoesntExistError);
    }

    // Get the context associated with this name
    return GEM::Renderer::Context::contextPtrMap[name];
}

/**
 * @brief Clear all of the GEM::Renderer::Contexts from the maps so they are no longer being counted and can
 * be destructed. Also call GLFW terminate.
 * 
 * @note This should only be called once! And it should only be called at the end when we not longer
 * need to use glfw!
 */
void GEM::Renderer::Context::clean() {
    LOG_TRACE("Clearing Context maps");
    GEM::Renderer::Context::contextPtrMap.clear();
    GEM::Renderer::Context::contextPtrCallbackMap.clear();

    LOG_TRACE("Terminating GLFW");
    glfwTerminate();
}

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief Create a GLFW window pointer with the corresponding attributes
 * 
 * @param windowTitle The title of the window
 * @param initialWindowWidthPixels The initial width of the window in pixels
 * @param initialWindowHeightPixels The initial height of the window in pixels
 * @param p_glfwMonitor The pointer to the desired GLFW Monitor
 * @param p_glfwSharedWindow The pointer to the GLFW Window this window will be sharing resources with
 * @return std::shared_ptr<GLFWwindow> The resulting GLFW Window pointer in a shared pointer for easy
 * resource management
 */
std::shared_ptr<GLFWwindow> GEM::Renderer::Context::Context::glfwInitCreateWindow(
        const std::string& windowTitle,
        int initialWindowWidthPixels,
        int initialWindowHeightPixels,
        const std::shared_ptr<GLFWmonitor> p_glfwMonitor,
        const std::shared_ptr<GLFWwindow> p_glfwSharedWindow
) {
    LOG_FUNCTION_CALL_INFO(
        "title {} , widght pix {} , height pix {} , monitor ptr {} , shared window ptr {} ",
        windowTitle,
        initialWindowWidthPixels,
        initialWindowHeightPixels,
        static_cast<void*>(p_glfwMonitor.get()),
        static_cast<void*>(p_glfwSharedWindow.get())
    );

    if (!GEM::Renderer::Context::glfwInitialized) {
        LOG_INFO("Initializing GLFW");

        // Initialize glfw
        if (!glfwInit()) {
            const std::string glfwInitializationErrorMessage = "Failed to initialize GLFW";
            LOG_CRITICAL(glfwInitializationErrorMessage);
            throw std::runtime_error(glfwInitializationErrorMessage);
        }

        // Configure glfw
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GEM_GLFW_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GEM_GLFW_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Doesn't actually fix the red/black flickering effect
#ifdef ON_MAC
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // For mac os
#endif

        GEM::Renderer::Context::glfwInitialized = true;
    }

    // Create the window
    std::shared_ptr<GLFWwindow> p_glfwWindow(
        glfwCreateWindow(
            initialWindowWidthPixels,
            initialWindowHeightPixels,
            windowTitle.c_str(),
            p_glfwMonitor.get(),
            p_glfwSharedWindow.get()
        ),
        [](GLFWwindow* p_window) { glfwDestroyWindow(p_window); }
    );
    LOG_TRACE("Created GLFW Window pointer at {}", static_cast<void*>(p_glfwWindow.get()));
    if (p_glfwWindow == nullptr) {
        const std::string glfwWindowCreationErrorMessage = "Failed to create GLFW window";
        LOG_CRITICAL(glfwWindowCreationErrorMessage);
        glfwTerminate();
        throw std::runtime_error(glfwWindowCreationErrorMessage);
    }

    // Make the context the current context
    glfwMakeContextCurrent(p_glfwWindow.get());

    // Tell opengl to use the framebuffer size callback when the window is resized
    // This will be called once upon start up so we don't need to call glViewport right away
    glfwSetFramebufferSizeCallback(p_glfwWindow.get(), GEM::Renderer::Context::CallbackHelper::windowResizeCallback);

    // Initialize glad
    // Give glad the function to load the address of the OS specific OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        const std::string gladInitializationErrorMessage = "Failed to initialize GLAD";
        LOG_CRITICAL(gladInitializationErrorMessage);
        throw std::runtime_error(gladInitializationErrorMessage);
    }

    // For 3d depth buffering
    glEnable(GL_DEPTH_TEST);

    return p_glfwWindow;
}

/**
 * @brief The callback glfw uses when the window is resized. This get's the Gemstone context associated with the given GLFW
 * Window ptr and updates its wdith and height fields to reflect the changes.
 * 
 * @param p_glfwWindow The pointer to the glfw window we want to get the context for
 * @param updatedWindowWidthPixels The updated window width in pixels
 * @param updatedWindowHeightPixels The updated window height in pixels
 */
void GEM::Renderer::Context::CallbackHelper::windowResizeCallback(GLFWwindow* p_glfwWindow, int updatedWindowWidthPixels, int updatedWindowHeightPixels) {
    std::shared_ptr<GEM::Renderer::Context> p_context = GEM::Renderer::Context::contextPtrCallbackMap[p_glfwWindow];

    if (p_context == nullptr) {
        LOG_ERROR("No context to resize window for");
        return;
    }

    p_context->m_windowWidthPixels = updatedWindowWidthPixels;
    p_context->m_windowHeightPixels = updatedWindowHeightPixels;
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Destroy the GEM::Renderer::Context::Context object
 */
GEM::Renderer::Context::~Context() {
    LOG_FUNCTION_CALL_TRACE("this ptr {}", static_cast<void*>(this));
}

/* ------------------------------ private member functions ------------------------------ */

/**
 * @brief Construct a new GEM::Renderer::Context::Context object given the name of the window and the initial width,height in pixels
 * 
 * @param name The name of the window
 * @param initialWindowWidthPixels The initial width of the window in pixels
 * @param initialWindowHeightPixels The initial height of the window in pixels
 */
GEM::Renderer::Context::Context(const std::string& name, int initialWindowWidthPixels, int initialWindowHeightPixels) :
    m_name(name),
    m_windowWidthPixels(initialWindowWidthPixels),
    m_windowHeightPixels(initialWindowHeightPixels),
    mp_glfwMonitor(nullptr),
    mp_glfwSharedWindow(nullptr),
    mp_glfwWindow(GEM::Renderer::Context::glfwInitCreateWindow(
        m_name,
        m_windowWidthPixels,
        m_windowHeightPixels,
        mp_glfwMonitor,
        mp_glfwSharedWindow
    ))
{
    LOG_FUNCTION_CALL_INFO("name {} , initial window width pixels {} , initial window height pixels {}", name, initialWindowWidthPixels, initialWindowHeightPixels);
}