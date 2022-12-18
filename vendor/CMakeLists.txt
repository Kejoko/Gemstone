#====================================================================
# Third party vendor libraries
#====================================================================

# OpenGL
find_package(OpenGL REQUIRED)

# Glad
add_subdirectory(glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# GLM
add_subdirectory(glm)

# SpdLog
add_subdirectory(spdlog)

# STB image
add_subdirectory(stb)