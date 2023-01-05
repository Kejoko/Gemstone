#version 330 core

// Ambient light
uniform vec3 ambientLightColor;
uniform float ambientLightStrength;

// The color of the point source light
uniform vec3 lightColor;
uniform vec3 lightPosition;

// The color of the object
uniform vec3 objectColor;

// The input position and normal vector of the current fragment
in vec3 fragmentPosition;
in vec3 fragmentNormal;

// The output color
out vec4 fragmentColor;

void main() {
    vec3 ambient = ambientLightColor * ambientLightStrength;

    vec3 normal = normalize(fragmentNormal);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    
    float diffuseImpact = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = lightColor * diffuseImpact;

    fragmentColor = vec4((ambient + diffuse) * objectColor, 1.0);
}
