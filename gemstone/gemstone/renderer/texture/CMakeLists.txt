#====================================================================
# The texture library
#====================================================================
add_library(
    GEM_Renderer_Texture
    SHARED
    logger.hpp
    Texture.hpp
    Texture.cpp
)

target_link_libraries(
    GEM_Renderer_Texture
    PUBLIC
    glad
    stb
    UTIL_Logger
)