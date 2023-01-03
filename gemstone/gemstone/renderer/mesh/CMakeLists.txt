#====================================================================
# The mesh library
#====================================================================
add_library(
    GEM_Renderer_Mesh
    SHARED
    logger.hpp
    Mesh.hpp
    Mesh.cpp
)

target_link_libraries(
    GEM_Renderer_Mesh
    PUBLIC
    glad
    glm
    UTIL_Logger
)