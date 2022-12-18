#====================================================================
# The stb library
#====================================================================
add_library(
    stb
    SHARED
    include/stb/stb_image.h
    src/stb_image.cpp
)

target_include_directories(
    stb
    PUBLIC
    include
)