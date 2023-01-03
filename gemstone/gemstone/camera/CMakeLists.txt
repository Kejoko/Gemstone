#====================================================================
# The camera library
#====================================================================
add_library(
    GEM_Camera
    SHARED
    logger.hpp
    Camera.hpp
    Camera.cpp
)

target_link_libraries(
    GEM_Camera
    PUBLIC
    glm
    UTIL_Logger
    GEM_Managers_InputManager
    GEM_Renderer_Context
)