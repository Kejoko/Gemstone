#====================================================================
# The window context library
#====================================================================
add_library(
    GEM_Renderer_Context
    SHARED
    logger.hpp
    Context.hpp
    Context.cpp
)

target_link_libraries(
    GEM_Renderer_Context
    PUBLIC
    glad
    glfw
    UTIL_Logger
)