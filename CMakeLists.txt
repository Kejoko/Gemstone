#====================================================================
# Gemstone and Application
#====================================================================
set(PROJECT_NAME "Gemstone")
set(GEMSTONE_MAJOR_VERSION 0)
set(GEMSTONE_MINOR_VERSION 0)
set(GEMSTONE_PATCH_VERSION 1)

cmake_minimum_required(VERSION 3.14)

project(
    ${PROJECT_NAME}
    VERSION ${GEMSTONE_MAJOR_VERSION}.${GEMSTONE_MINOR_VERSION}.${GEMSTONE_PATCH_VERSION}
    LANGUAGES CXX C
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# The root directories containing source code
set(APPLICATION_ROOT_DIR "${PROJECT_SOURCE_DIR}/application")
set(GEMSTONE_ROOT_DIR "${PROJECT_SOURCE_DIR}/gemstone")

#====================================================================
# C++ 17 Support and compiler flags
#====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

#====================================================================
# Third party vendor libraries
#====================================================================
set(VENDOR_ROOT_DIR "${PROJECT_SOURCE_DIR}/vendor")
add_subdirectory("${VENDOR_ROOT_DIR}")
list(APPEND VENDOR_LIBS glad)
list(APPEND VENDOR_LIBS glfw)
list(APPEND VENDOR_LIBS glm)
list(APPEND VENDOR_LIBS spdlog)
list(APPEND VENDOR_LIBS stb)

#====================================================================
# Add preprocessor directives for the gemstone libraries
#====================================================================
# @todo: only add these for gemstone libraries. We don't want to be setting these for the
#        vendor libraries. These should only be visible for gemstone and application
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

include_directories(
    ${GEMSTONE_ROOT_DIR}
)

#====================================================================
# Utility libraries
#====================================================================
set(UTIL_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/util")

add_subdirectory("${UTIL_SOURCE_DIR}")
list(APPEND UTIL_LIBS UTIL_IO)
list(APPEND UTIL_LIBS UTIL_Logger)

#====================================================================
# Gemstone libraries
#====================================================================
set(GEMSTONE_SOURCE_DIR "${GEMSTONE_ROOT_DIR}/gemstone")

add_subdirectory("${GEMSTONE_SOURCE_DIR}")
list(APPEND GEMSTONE_LIBS GEM_Application)
list(APPEND GEMSTONE_LIBS GEM_Camera)
list(APPEND GEMSTONE_LIBS GEM_Mesh)
list(APPEND GEMSTONE_LIBS GEM_Scene)
list(APPEND GEMSTONE_LIBS GEM_Shader)
list(APPEND GEMSTONE_LIBS GEM_Texture)

#====================================================================
# The assets for the application
#====================================================================
set(APPLICATION_ASSET_DIR "${APPLICATION_ROOT_DIR}/assets")
set(APPLICATION_SHADER_SOURCE_DIR "${APPLICATION_ASSET_DIR}/shaders")

include(CreateRawStringFile)
create_raw_string_file(${APPLICATION_SHADER_SOURCE_DIR}/vertex.vert)
create_raw_string_file(${APPLICATION_SHADER_SOURCE_DIR}/fragment.frag)
create_raw_string_file(${APPLICATION_SHADER_SOURCE_DIR}/fragment2.frag)

#====================================================================
# The application and its assets
#====================================================================
set(APPLICATION_SOURCE_DIR "${APPLICATION_ROOT_DIR}/application")
add_subdirectory("${APPLICATION_SOURCE_DIR}")