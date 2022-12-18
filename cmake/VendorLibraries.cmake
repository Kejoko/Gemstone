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

# The source directories within the gemstone/vendor directory
set(VENDOR_SOURCE_DIR "${PROJECT_SOURCE_DIR}/gemstone/vendor")
set(GLAD_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glad")
set(GLFW_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glfw")
set(GLM_SOURCE_DIR "${VENDOR_SOURCE_DIR}/glm")
set(SPDLOG_SOURCE_DIR "${VENDOR_SOURCE_DIR}/spdlog")
set(STB_SOURCE_DIR "${VENDOR_SOURCE_DIR}/stb")

# OpenGL
find_package(OpenGL REQUIRED)

# Glad
add_subdirectory(${GLAD_SOURCE_DIR})
# list(APPEND VENDOR_INCLUDE_DIRS ${GLAD_SOURCE_DIR}/include)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_SOURCE_DIR})
# list(APPEND VENDOR_INCLUDE_DIRS ${GLFW_SOURCE_DIR}/include)

# GLM
add_subdirectory(${GLM_SOURCE_DIR})
# list(APPEND VENDOR_INCLUDE_DIRS ${GLM_SOURCE_DIR})

# SpdLog
add_subdirectory(${SPDLOG_SOURCE_DIR})
# list(APPEND VENDOR_INCLUDE_DIRS ${SPDLOG_SOURCE_DIR}/include)

# STB image
add_subdirectory(${STB_SOURCE_DIR})
# list(APPEND VENDOR_INCLUDE_DIRS ${STB_SOURCE_DIR}/include)