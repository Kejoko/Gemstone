#version 330 core

// Ambient light
uniform vec3 ambientLightColor;
uniform float ambientLightStrength;

// // The color of the point source light
// uniform vec4 lightColor;
// uniform vec4 lightPosition;

// The color of the object
uniform vec3 objectColor;

// The output color
out vec4 fragmentColor;

void main() {
    vec3 ambientLight = ambientLightColor * ambientLightStrength;
    fragmentColor = vec4(ambientLight * objectColor, 1.0);

    // fragmentColor = lightColor * objectColor;
}
