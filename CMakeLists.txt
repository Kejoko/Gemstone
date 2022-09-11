#====================================================================
# Gemstone
#====================================================================
set(PROJECT_NAME "Gemstone")
set(GEMSTONE_MAJOR_VER 0)
set(GEMSTONE_MINOR_VER 0)
set(GEMSTONE_PATCH_VER 1)

set(APPLICATION_NAME "App")
set(APPLICATION_MAJOR_VER 0)
set(APPLICATION_MINOR_VER 0)
set(APPLICATION_PATCH_VER 0)

cmake_minimum_required(VERSION 3.14)

project(
    ${PROJECT_NAME}
    VERSION ${GEMSTONE_MAJOR_VER}.${GEMSTONE_MINOR_VER}.${GEMSTONE_PATCH_VER}
    LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#====================================================================
# C++ 17 Support
#====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#====================================================================
# Create the target(s)
#====================================================================
set(GEMSTONE_ROOT_DIR "${PROJECT_SOURCE_DIR}/gemstone")
set(APPLICATION_ROOT_DIR "${PROJECT_SOURCE_DIR}/application")

set(UTIL_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/util")
set(GEMSTONE_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/gemstone")
set(APPLICATION_SOURCE_DIR "${APPLICATION_ROOT_DIR}/application")

add_executable(
    ${APPLICATION_NAME} ${APPLICATION_SOURCE_DIR}/main.cpp
)

#====================================================================
# Specify include directories
#====================================================================
include_directories(
    ${GEMSTONE_ROOT_DIR}
)

target_include_directories(
    ${APPLICATION_NAME}
    PUBLIC ${APPLICATION_ROOT_DIR}
)

#====================================================================
# Add preprocessor directives
#====================================================================
# @todo: debug mode by default, release mode if `cmake -DCMAKE_BUILD_TYPE=Release`
# @todo: we want to have a way to sepcify debug for Gemstone and for App seperately, an example
# case is where we want to debug App while Gemstone is in release version
add_compile_definitions(DEBUG)

add_compile_definitions(
    GEMSTONE_VER
    GEMSTONE_MAJOR_VER=${GEMSTONE_MAJOR_VER}
    GEMSTONE_MINOR_VER=${GEMSTONE_MINOR_VER}
    GEMSTONE_PATCH_VER=${GEMSTONE_PATCH_VER}
)

target_compile_definitions(
    ${APPLICATION_NAME}
    PUBLIC APPLICATION_VER
    PUBLIC APPLICATION_MINOR_VER=${APPLICATION_MINOR_VER}
    PUBLIC APPLICATION_PATCH_VER=${APPLICATION_PATCH_VER}
    PUBLIC APPLICATION_MAJOR_VER=${APPLICATION_MAJOR_VER}
)