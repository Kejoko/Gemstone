#====================================================================
# The scene library
#====================================================================
add_library(
    GEM_Scene
    SHARED
    logger.hpp
    Scene.hpp
    Scene.cpp
)

target_link_libraries(
    GEM_Scene
    PUBLIC
    glm
    UTIL_IO
    UTIL_Logger
    GEM_Camera
    GEM_Object
    GEM_Managers_InputManager
    GEM_Renderer_Mesh
    GEM_Renderer_Context
)