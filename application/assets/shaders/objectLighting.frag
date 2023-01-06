#version 330 core

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

struct AmbientLight {
    vec3 color;
    float strength;
};

struct Light {
    vec3 worldPosition;
    vec3 diffuseColor;
    vec3 specularColor;
};

// The position of the camera
uniform vec3 cameraPosition;

// Ambient light
// uniform vec3 ambientLightColor;
// uniform float ambientLightStrength;
uniform AmbientLight ambientLight;

// The color of the point source light
// uniform vec3 lightColor;
// uniform vec3 lightPosition;
uniform Light light;

// The material of the object
uniform Material objectMaterial;

// The input position and normal vector of the current fragment
in vec3 fragmentPosition;
in vec3 fragmentNormal;

// The output color
out vec4 fragmentColor;

void main() {

    // ----- calculate the ambient light amount ----- //

    vec3 ambientResult = (ambientLight.strength * ambientLight.color) * objectMaterial.ambientColor;

    // ----- calculate the diffuse light amount ----- //

    vec3 normal = normalize(fragmentNormal);
    vec3 lightDirection = normalize(light.worldPosition - fragmentPosition);
    
    float diffuseImpact = max(
        dot(normal, lightDirection),
        0.0
    );
    vec3 diffuseResult = light.diffuseColor * (diffuseImpact * objectMaterial.diffuseColor);

    // ----- calculate the specular light amount ----- //

    vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    float specularImpact = pow(
        max(
            dot(cameraDirection, reflectionDirection),
            0.0
        ),
        objectMaterial.shininess
    );
    vec3 specularResult = light.specularColor * (specularImpact * objectMaterial.specularColor);

    // ----- calculate the result ----- //

    fragmentColor = vec4(ambientResult + diffuseResult + specularResult, 1.0);

}
