#version 330 core

// Position variable has attribute position 0 and color has attribute position 1
layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec3 i_color;
layout (location = 3) in vec2 i_textureCoord;

// Input transformation matrix
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Output the vertex normal for the fragment shader to use for lighting
out vec3 fragmentPosition;
out vec3 fragmentNormal;

void main() {
    // Giving all of aPosition to the constructor saves us from manually writing x, y, and z
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(i_position, 1.0f);
    
    fragmentPosition = vec3(modelMatrix * vec4(i_position, 1.0));
    fragmentNormal = mat3(transpose(inverse(modelMatrix))) * i_normal;
}
