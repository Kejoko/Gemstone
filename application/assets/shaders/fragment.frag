#version 330 core

// The input texture
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

// The input from the vertex shader (same type and name)
in vec4 vertexColor;
in vec2 textureCoord;

// The output color
out vec4 fragmentColor;

void main() {
    vec4 textureColor = texture(ourTexture, textureCoord);
    vec4 textureColor2 = texture(ourTexture2, textureCoord);
    fragmentColor = mix(textureColor, textureColor2, 0.2) * vertexColor;
    // fragmentColor = textureColor * vertexColor;
    // fragmentColor = textureColor;
}
