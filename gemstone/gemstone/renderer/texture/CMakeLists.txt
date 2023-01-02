#====================================================================
# The texture library
#====================================================================
add_library(
    GEM_Texture
    SHARED
    logger.hpp
    Texture.hpp
    Texture.cpp
)

target_link_libraries(
    GEM_Texture
    PUBLIC
    glad
    stb
    UTIL_Logger
)