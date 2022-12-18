#====================================================================
# The io library
#====================================================================
add_library(
    UTIL_IO
    SHARED
    logger.hpp
    FileSystem.hpp
    FileSystem.cpp
)

target_link_libraries(
    UTIL_IO
    PUBLIC
    UTIL_Logger
)