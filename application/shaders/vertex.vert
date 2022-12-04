#version 330 core

// Position variable has attribute position 0
layout (location = 0) in vec3 aPos;

// Specify a color output to give the fragment shader
out vec4 vertexColor;

void main() {
    // Giving all of aPos to the constructor saves us from manually writing x, y, and z
    gl_Position = vec4(aPos, 1.0);

    // Set the output
    vertexColor = vec4(aPos, 1.0);
}
