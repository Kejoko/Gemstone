#====================================================================
# The light library
#====================================================================
add_library(
    GEM_Light
    SHARED
    logger.hpp
    Light.hpp
    Light.cpp
)

target_link_libraries(
    GEM_Light
    PUBLIC
    glm
    UTIL_Logger
    GEM_Object
)