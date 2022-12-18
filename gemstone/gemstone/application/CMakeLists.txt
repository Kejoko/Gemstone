#====================================================================
# The application library
#====================================================================
add_library(
    GEM_Application
    SHARED
    logger.hpp
    Application.hpp
    Application.cpp
)

target_link_libraries(
    GEM_Application
    PUBLIC
    UTIL_Logger
    GEM_Scene
)