#version 330 core

// Color we set and the light color
uniform vec4 objectColor;
uniform vec4 lightColor;

// The output color
out vec4 fragmentColor;

void main() {
    fragmentColor = lightColor * objectColor;
}
