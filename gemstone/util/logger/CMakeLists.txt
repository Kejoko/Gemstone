#====================================================================
# The logger library
#====================================================================
add_library(
    UTIL_Logger
    SHARED
    Logger.hpp
    Logger.cpp
)

target_link_libraries(
    UTIL_Logger
    PUBLIC
    spdlog
)