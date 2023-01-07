#====================================================================
# The mesh library
#====================================================================
add_library(
    GEM_Renderer_Model
    SHARED
    logger.hpp
    Model.hpp
    Model.cpp
)

target_link_libraries(
    GEM_Renderer_Model
    PUBLIC
    UTIL_Logger
    GEM_Renderer_Material
    GEM_Renderer_Mesh
)