#====================================================================
# The light library
#====================================================================
add_library(
    GEM_Light
    SHARED
    logger.hpp
    Light.hpp
    Light.cpp
    DirectionalLight.hpp
    DirectionalLight.cpp
    ObjectLight.hpp
    ObjectLight.cpp
    PointLight.hpp
    PointLight.cpp
    SpotLight.hpp
    SpotLight.cpp
)

target_link_libraries(
    GEM_Light
    PUBLIC
    glm
    UTIL_Logger
    GEM_Object
)