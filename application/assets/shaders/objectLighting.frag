#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
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
uniform AmbientLight ambientLight;

// The color of the point source light
uniform Light light;

// The material of the object
uniform Material objectMaterial;

// The input position and normal vector of the current fragment
in vec3 fragmentPosition;
in vec3 fragmentNormal;

// The input texture coordinates of the current fragment
in vec2 diffuseMapCoords;
in vec2 specularMapCoords;

// The output color
out vec4 fragmentColor;

void main() {

    vec3 diffuseColor = vec3(texture(objectMaterial.diffuseMap, diffuseMapCoords));
    vec3 specularIntensity = vec3(texture(objectMaterial.specularMap, specularMapCoords));

    // ----- calculate the ambient light amount ----- //

    vec3 ambientResult = (ambientLight.strength * ambientLight.color) * diffuseColor;

    // ----- calculate the diffuse light amount ----- //

    vec3 normal = normalize(fragmentNormal);
    vec3 lightDirection = normalize(light.worldPosition - fragmentPosition);
    
    float diffuseImpact = max(
        dot(normal, lightDirection),
        0.0
    );

    vec3 diffuseResult = light.diffuseColor * (diffuseImpact * diffuseColor);

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
    vec3 specularResult = light.specularColor * (specularImpact * specularIntensity);

    // ----- calculate the result ----- //

    fragmentColor = vec4(ambientResult + diffuseResult + specularResult, 1.0);

}
