#====================================================================
# Third party vendor libraries. This defines VENDOR_LIBS for us to
# link against when creating libraries and targets
#
# @note: This file is intended to be included after the following
#        variables are defined:
#          * GLAD_SOURCE_DIR
#          * GLFW_SOURCE_DIR
#          * SPDLOG_SOURCE_DIR
#====================================================================
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# OpenGL
find_package(OpenGL REQUIRED)

# Glad
add_library(
    glad
    SHARED
    ${GLAD_SOURCE_DIR}/include/glad/glad.h
    ${GLAD_SOURCE_DIR}/src/glad.c
)
list(APPEND VENDOR_INCLUDE_DIRS ${GLAD_SOURCE_DIR}/include)
list(APPEND VENDOR_LIBS glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_SOURCE_DIR})
list(APPEND VENDOR_INCLUDE_DIRS ${GLFW_SOURCE_DIR}/include)
list(APPEND VENDOR_LIBS glfw)

# GLM
add_subdirectory(${GLM_SOURCE_DIR})
list(APPEND VENDOR_INCLUDE_DIRS ${GLM_SOURCE_DIR})
list(APPEND VENDOR_LIBS glm)

# SpdLog
add_subdirectory(${SPDLOG_SOURCE_DIR})
list(APPEND VENDOR_INCLUDE_DIRS ${SPDLOG_SOURCE_DIR}/include)
list(APPEND VENDOR_LIBS spdlog)

# STB image
add_library(
    stb
    SHARED
    ${STB_SOURCE_DIR}/include/stb/stb_image.h
    ${STB_SOURCE_DIR}/src/stb_image.cpp
)
list(APPEND VENDOR_INCLUDE_DIRS ${STB_SOURCE_DIR}/include)
list(APPEND VENDOR_LIBS stb)