#version 330 core

// Position variable has attribute position 0 and color has attribute position 1
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

// Specify a color output to give the fragment shader
out vec4 vertexColor;

void main() {
    // Giving all of aPosition to the constructor saves us from manually writing x, y, and z
    gl_Position = vec4(aPosition, 1.0);

    // Set the output
    vertexColor = vec4(aColor, 1.0);
}
