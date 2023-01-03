#====================================================================
# The glad library
#====================================================================
add_library(
    glad
    SHARED
    include/glad/glad.h
    src/glad.c
)

target_include_directories(
    glad
    PUBLIC
    include
)
