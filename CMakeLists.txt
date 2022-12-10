#====================================================================
# Gemstone and Application
#====================================================================
set(PROJECT_NAME "Gemstone")
set(GEMSTONE_MAJOR_VERSION 0)
set(GEMSTONE_MINOR_VERSION 0)
set(GEMSTONE_PATCH_VERSION 1)

set(APPLICATION_NAME "App")
set(APPLICATION_MAJOR_VERSION 0)
set(APPLICATION_MINOR_VERSION 0)
set(APPLICATION_PATCH_VERSION 0)

cmake_minimum_required(VERSION 3.14)

project(
    ${PROJECT_NAME}
    VERSION ${GEMSTONE_MAJOR_VERSION}.${GEMSTONE_MINOR_VERSION}.${GEMSTONE_PATCH_VERSION}
    LANGUAGES CXX C
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#====================================================================
# C++ 17 Support and compiler flags
#====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

#======================================================================
# Subdirectories containing source code
#======================================================================

# ---------- root level ---------- #

# The two base level source directories
set(APPLICATION_ROOT_DIR "${PROJECT_SOURCE_DIR}/application")
set(GEMSTONE_ROOT_DIR "${PROJECT_SOURCE_DIR}/gemstone")

# ----- within application ----- #

# The source directories within the application root directory and the
# asset directory
set(APPLICATION_SOURCE_DIR "${APPLICATION_ROOT_DIR}/application")
set(APPLICATION_ASSET_DIR "${APPLICATION_ROOT_DIR}/assets")

# Asset directories
set(APPLICATION_SHADER_SOURCE_DIR "${APPLICATION_ASSET_DIR}/shaders")

# ----- within gemstone ----- #

# The source directories within the gemstone root directory
set(GEMSTONE_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/gemstone")
set(UTIL_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/util")
set(VENDOR_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/vendor")

# The source directories within the gemstone/vendor directory
set(GLAD_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glad")
set(GLFW_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glfw")
set(SPDLOG_SOURCE_DIR "${VENDOR_SOURCE_DIR}/spdlog")

# The source directories within the gemstone/util directory
set(UTIL_LOGGER_SOURCE_DIR "${UTIL_SOURCE_DIR}/logger")

# Within gemstone/gemstone
set(GEMSTONE_SHADER_SOURCE_DIR "${GEMSTONE_SOURCE_DIR}/shader")

#====================================================================
# Third party vendor libraries
#
# @todo We should do these all in .cmake modules to not pollute this file
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

# SpdLog
add_subdirectory(${SPDLOG_SOURCE_DIR})
list(APPEND VENDOR_INCLUDE_DIRS ${SPDLOG_SOURCE_DIR}/include)
list(APPEND VENDOR_LIBS spdlog)

#====================================================================
# Juniper utility libraries
#====================================================================
add_library(
    UTIL_Logger
    SHARED
    ${UTIL_LOGGER_SOURCE_DIR}/Logger.hpp
    ${UTIL_LOGGER_SOURCE_DIR}/Logger.cpp
)
list(APPEND UTIL_LIBS UTIL_Logger)

#====================================================================
# Gemstone libraries
#====================================================================
add_library(
    GEM_Shader
    SHARED
    ${GEMSTONE_SHADER_SOURCE_DIR}/logger.hpp
    ${GEMSTONE_SHADER_SOURCE_DIR}/CompiledShader.hpp
    ${GEMSTONE_SHADER_SOURCE_DIR}/CompiledShader.cpp
    ${GEMSTONE_SHADER_SOURCE_DIR}/ShaderProgram.hpp
    ${GEMSTONE_SHADER_SOURCE_DIR}/ShaderProgram.cpp
)
list(APPEND GEMSTONE_LIBS GEM_Shader)

#====================================================================
# Create the target
#====================================================================
add_executable(
    ${APPLICATION_NAME} ${APPLICATION_SOURCE_DIR}/main.cpp
)

#====================================================================
# Specify include directories for gemstone and the application
#====================================================================
# We want everything to be able to include from these directories
include_directories(
    ${GEMSTONE_ROOT_DIR}
    ${VENDOR_INCLUDE_DIRS}
)

# We only want the application to be able to include from the application directory.
# This is so Gemstone doesn't include application source and become dependent upon
# the application
target_include_directories(
    ${APPLICATION_NAME}
    PUBLIC ${APPLICATION_ROOT_DIR}
)

#====================================================================
# Link libraries
#====================================================================
# Link util and vendor libs to gemstone
target_link_libraries(
    ${GEMSTONE_LIBS}
    PRIVATE ${VENDOR_LIBS}
    PRIVATE ${UTIL_LIBS}
)

# Link everything to the application
target_link_libraries(
    ${APPLICATION_NAME}
    PRIVATE ${VENDOR_LIBS}
    PRIVATE ${UTIL_LIBS}
    PRIVATE ${GEMSTONE_LIBS}
)

#====================================================================
# Convert the glsl shaders to raw string literal files so we can
# nicely include them in C++ strings
#====================================================================
include(CreateRawStringFile)
create_raw_string_file(${APPLICATION_SHADER_SOURCE_DIR}/vertex.vert)
create_raw_string_file(${APPLICATION_SHADER_SOURCE_DIR}/fragment.frag)
create_raw_string_file(${APPLICATION_SHADER_SOURCE_DIR}/fragment2.frag)

#====================================================================
# Add preprocessor directives
#====================================================================
# @todo: debug mode by default, release mode if `cmake -DCMAKE_BUILD_TYPE=Release`
# @todo: we want to have a way to sepcify debug for Gemstone and for App seperately, an example
#        case is where we want to debug Application while Gemstone is in release version. This
#        will require us to have two different release mode macros (EX: GEM_DEBUG , APP_DEBUG) 
add_compile_definitions(DEBUG)

add_compile_definitions(
    GEMSTONE_VERSION
    GEMSTONE_MAJOR_VERSION=${GEMSTONE_MAJOR_VERSION}
    GEMSTONE_MINOR_VERSION=${GEMSTONE_MINOR_VERSION}
    GEMSTONE_PATCH_VERSION=${GEMSTONE_PATCH_VERSION}
)

target_compile_definitions(
    ${APPLICATION_NAME}
    PUBLIC APPLICATION_VERSION
    PUBLIC APPLICATION_MINOR_VERSION=${APPLICATION_MINOR_VERSION}
    PUBLIC APPLICATION_PATCH_VERSION=${APPLICATION_PATCH_VERSION}
    PUBLIC APPLICATION_MAJOR_VERSION=${APPLICATION_MAJOR_VERSION}
)