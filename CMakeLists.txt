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
set(GLM_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glm")
set(SPDLOG_SOURCE_DIR "${VENDOR_SOURCE_DIR}/spdlog")
set(STB_SOURCE_DIR "${VENDOR_SOURCE_DIR}/stb")

# The source directories within the gemstone/util directory
set(UTIL_IO_SOURCE_DIR "${UTIL_SOURCE_DIR}/io")
set(UTIL_LOGGER_SOURCE_DIR "${UTIL_SOURCE_DIR}/logger")

# Within gemstone/gemstone
set(GEMSTONE_MESH_SOURCE_DIR "${GEMSTONE_SOURCE_DIR}/mesh")
set(GEMSTONE_SHADER_SOURCE_DIR "${GEMSTONE_SOURCE_DIR}/shader")
set(GEMSTONE_TEXTURE_SOURCE_DIR "${GEMSTONE_SOURCE_DIR}/texture")

#====================================================================
# Third party vendor libraries
#====================================================================
include(VendorLibraries)

#====================================================================
# Utility libraries
#====================================================================
add_library(
    UTIL_IO
    SHARED
    ${UTIL_IO_SOURCE_DIR}/logger.hpp
    ${UTIL_IO_SOURCE_DIR}/FileSystem.hpp
    ${UTIL_IO_SOURCE_DIR}/FileSystem.cpp
)
list(APPEND UTIL_LIBS UTIL_IO)

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
    GEM_Mesh
    SHARED
    ${GEMSTONE_MESH_SOURCE_DIR}/logger.hpp
    ${GEMSTONE_MESH_SOURCE_DIR}/Mesh.hpp
    ${GEMSTONE_MESH_SOURCE_DIR}/Mesh.cpp
)
list(APPEND GEMSTONE_LIBS GEM_Mesh)

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

add_library(
    GEM_Texture
    SHARED
    ${GEMSTONE_TEXTURE_SOURCE_DIR}/logger.hpp
    ${GEMSTONE_TEXTURE_SOURCE_DIR}/Texture.hpp
    ${GEMSTONE_TEXTURE_SOURCE_DIR}/Texture.cpp
)
list(APPEND GEMSTONE_LIBS GEM_Texture)

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
# Link util and vendor libs to each gemstone lib
foreach(GEMSTONE_LIB IN LISTS GEMSTONE_LIBS)
    target_link_libraries(
        ${GEMSTONE_LIB}
        PRIVATE ${VENDOR_LIBS}
        PRIVATE ${UTIL_LIBS}
    )
endforeach()


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

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

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