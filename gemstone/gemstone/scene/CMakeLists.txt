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
    UTIL_Logger
    GEM_Mesh
)