#====================================================================
# The object library
#====================================================================
add_library(
    GEM_Object
    SHARED
    logger.hpp
    Object.hpp
    Object.cpp
)

target_link_libraries(
    GEM_Object
    PUBLIC
    glad
    glfw
    glm
    UTIL_IO
    UTIL_Logger
    GEM_Renderer_Mesh
    GEM_Renderer_Shader
    GEM_Renderer_Texture
)