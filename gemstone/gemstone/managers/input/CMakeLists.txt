#====================================================================
# The input manager library
#====================================================================
add_library(
    GEM_Managers_InputManager
    SHARED
    InputManager.hpp
    InputManager.cpp
)

target_link_libraries(
    GEM_Managers_InputManager
    PUBLIC
    glfw
)