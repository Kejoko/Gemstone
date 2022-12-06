#version 330 core

// The input from the vertex shader (same type and name)
in vec4 vertexColor;

// The output color
out vec4 fragmentColor;

void main() {
    fragmentColor = vertexColor;
}
