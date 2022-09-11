#====================================================================
# Gemstone and Application
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
    LANGUAGES CXX C
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#====================================================================
# C++ 17 Support
#====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================================================================
# Subdirectories containing source code
#======================================================================
# The two base level source directories
set(APPLICATION_ROOT_DIR "${PROJECT_SOURCE_DIR}/application")
set(GEMSTONE_ROOT_DIR "${PROJECT_SOURCE_DIR}/gemstone")

# The source directories within the application root directory
set(APPLICATION_SOURCE_DIR "${APPLICATION_ROOT_DIR}/application")

# The source directories within the gemstone root directory
set(GEMSTONE_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/gemstone")
set(UTIL_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/util")
set(VENDOR_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/vendor")

# The source directories within the util directory
set(LOGGER_SOURCE_DIR "${UTIL_SOURCE_DIR}/logger")

# The source directories within the vendor directory
set(GLAD_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glad")
set(GLFW_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glfw")
set(SPDLOG_SOURCE_DIR "${VENDOR_SOURCE_DIR}/spdlog")

#====================================================================
# Third party vendor libraries
#
# @todo We should do these all in .cmake modules to not pollute this file
#====================================================================
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

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

# SpdLog
add_subdirectory(${SPDLOG_SOURCE_DIR})
list(APPEND VENDOR_INCLUDE_DIRS ${SPDLOG_SOURCE_DIR}/include)
list(APPEND VENDOR_LIBS spdlog)

#====================================================================
# Juniper utility libraries
#====================================================================
add_library(
    GEM_Logger
    SHARED
    ${LOGGER_SOURCE_DIR}/Logger.hpp
    ${LOGGER_SOURCE_DIR}/Logger.cpp
)
list(APPEND UTIL_LIBS GEM_Logger)

#====================================================================
# Create the target
#====================================================================
add_executable(
    ${APPLICATION_NAME} ${APPLICATION_SOURCE_DIR}/main.cpp
)

#====================================================================
# Specify include directories
#====================================================================
# We want everything to be able to include from these directories
include_directories(
    ${GEMSTONE_ROOT_DIR}
    ${VENDOR_INCLUDE_DIRS}
)

# We only want the application to be able to include from the application directory
target_include_directories(
    ${APPLICATION_NAME}
    PUBLIC ${APPLICATION_ROOT_DIR}
)

#====================================================================
# Link libraries
#====================================================================
target_link_libraries(
    ${APPLICATION_NAME}
    PRIVATE ${VENDOR_LIBS}
    PRIVATE ${UTIL_LIBS}
)

#====================================================================
# Add preprocessor directives
#====================================================================
# @todo: debug mode by default, release mode if `cmake -DCMAKE_BUILD_TYPE=Release`
# @todo: we want to have a way to sepcify debug for Gemstone and for App seperately, an example
# case is where we want to debug Application while Gemstone is in release version
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