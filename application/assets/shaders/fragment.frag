#version 330 core

// The input texture
uniform sampler2D ourTexture;

// The input from the vertex shader (same type and name)
in vec4 vertexColor;
in vec2 textureCoord;

// The output color
out vec4 fragmentColor;

void main() {
    vec4 textureColor = texture(ourTexture, textureCoord);
    fragmentColor = textureColor * vertexColor;
}
