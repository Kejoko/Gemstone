#====================================================================
# The input manager library
#====================================================================
add_library(
    GEM_Managers_InputManager
    SHARED
    logger.hpp
    InputManager.hpp
    InputManager.cpp
)

target_link_libraries(
    GEM_Managers_InputManager
    PUBLIC
    glfw
    UTIL_Logger
)