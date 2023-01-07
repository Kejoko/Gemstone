#====================================================================
# The executable target application (the *game* oooooh)
#====================================================================
set(APPLICATION_NAME "App")
set(APPLICATION_MAJOR_VERSION 0)
set(APPLICATION_MINOR_VERSION 0)
set(APPLICATION_PATCH_VERSION 0)

add_executable(
    ${APPLICATION_NAME} ${APPLICATION_SOURCE_DIR}/main.cpp
)

target_include_directories(
    ${APPLICATION_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Link everything to the application
target_link_libraries(
    ${APPLICATION_NAME}

    # Vendor
    PRIVATE
    glad
    glfw
    glm

    # Gemstone Utility
    PRIVATE
    UTIL_IO
    UTIL_Logger

    # Gemstone
    PRIVATE
    GEM_Camera
    GEM_Light
    GEM_Object
    GEM_Scene
    GEM_Managers_InputManager
    GEM_Renderer_Material
    GEM_Renderer_Mesh
    GEM_Renderer_Model
    GEM_Renderer_Context
    GEM_Renderer_Shader
    GEM_Renderer_Texture
)

# pre compile definitions for the target
target_compile_definitions(
    ${APPLICATION_NAME}
    PUBLIC APPLICATION_VERSION
    PUBLIC APPLICATION_MINOR_VERSION=${APPLICATION_MINOR_VERSION}
    PUBLIC APPLICATION_PATCH_VERSION=${APPLICATION_PATCH_VERSION}
    PUBLIC APPLICATION_MAJOR_VERSION=${APPLICATION_MAJOR_VERSION}
)