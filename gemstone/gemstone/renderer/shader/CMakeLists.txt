#====================================================================
# The shader library
#====================================================================
add_library(
    GEM_Shader
    SHARED
    logger.hpp
    CompiledShader.hpp
    CompiledShader.cpp
    ShaderProgram.hpp
    ShaderProgram.cpp
)

target_link_libraries(
    GEM_Shader
    PUBLIC
    glad
    glm
    UTIL_Logger
    GEM_Texture
)