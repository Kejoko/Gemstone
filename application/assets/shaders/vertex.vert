#version 330 core

// Position variable has attribute position 0 and color has attribute position 1
layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_color;
layout (location = 2) in vec2 i_textureCoord;

// Specify a color output to give the fragment shader
out vec4 vertexColor;
out vec2 textureCoord;

// Input transformation matrix
uniform mat4 transformationMatrix;

void main() {
    // Giving all of aPosition to the constructor saves us from manually writing x, y, and z
    gl_Position = transformationMatrix * vec4(i_position, 1.0f);

    // Set the output
    vertexColor = vec4(i_color, 1.0);
    textureCoord = i_textureCoord;
}
