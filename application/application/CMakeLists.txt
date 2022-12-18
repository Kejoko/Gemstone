#====================================================================
# The executable target application (the *game* oooooh)
#====================================================================
set(APPLICATION_NAME "App")
set(APPLICATION_MAJOR_VERSION 0)
set(APPLICATION_MINOR_VERSION 0)
set(APPLICATION_PATCH_VERSION 0)

add_executable(
    ${APPLICATION_NAME} ${APPLICATION_SOURCE_DIR}/main.cpp
)

target_include_directories(
    ${APPLICATION_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Link everything to the application
target_link_libraries(
    ${APPLICATION_NAME}
    PRIVATE glad glfw glm
    PRIVATE UTIL_IO UTIL_Logger
    PRIVATE GEM_Application GEM_Mesh GEM_Scene GEM_Shader GEM_Texture
)

# pre compile definitions for the target
target_compile_definitions(
    ${APPLICATION_NAME}
    PUBLIC APPLICATION_VERSION
    PUBLIC APPLICATION_MINOR_VERSION=${APPLICATION_MINOR_VERSION}
    PUBLIC APPLICATION_PATCH_VERSION=${APPLICATION_PATCH_VERSION}
    PUBLIC APPLICATION_MAJOR_VERSION=${APPLICATION_MAJOR_VERSION}
)