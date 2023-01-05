#version 330 core

// The position of the camera
uniform vec3 cameraPosition;

// Ambient light
uniform vec3 ambientLightColor;
uniform float ambientLightStrength;

// The color of the point source light
uniform vec3 lightColor;
uniform vec3 lightPosition;

// The color of the object
uniform vec3 objectColor;
uniform float objectShininess;

// The input position and normal vector of the current fragment
in vec3 fragmentPosition;
in vec3 fragmentNormal;

// The output color
out vec4 fragmentColor;

void main() {
    // ----- calculate the ambient light amount ----- //

    vec3 ambient = ambientLightColor * ambientLightStrength;

    // ----- calculate the diffuse light amount ----- //

    vec3 normal = normalize(fragmentNormal);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    
    float diffuseImpact = max(
        dot(normal, lightDirection),
        0.0
    );
    vec3 diffuse = lightColor * diffuseImpact;

    // ----- calculate the specular light amount ----- //

    const float specularStrength = 0.5;

    vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    float specularImpact = pow(
        max(
            dot(cameraDirection, reflectionDirection),
            0.0
        ),
        objectShininess
    );
    vec3 specular = lightColor * specularImpact * specularStrength;

    // ----- calculate the result ----- //

    fragmentColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}
