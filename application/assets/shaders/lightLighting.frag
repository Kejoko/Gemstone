#version 330 core

// Color we set and the light color
uniform vec3 lightColor;

// The output color
out vec4 fragmentColor;

void main() {
    // Use a fixed value for the light object
    fragmentColor = vec4(lightColor, 1.0);
}
