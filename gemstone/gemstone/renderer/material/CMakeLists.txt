#====================================================================
# The mesh library
#====================================================================
add_library(
    GEM_Renderer_Material
    SHARED
    logger.hpp
    Material.hpp
    Material.cpp
)

target_link_libraries(
    GEM_Renderer_Material
    PUBLIC
    UTIL_IO
    UTIL_Logger
    GEM_Renderer_Shader
    GEM_Renderer_Texture
)