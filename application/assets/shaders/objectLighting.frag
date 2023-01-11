#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
    float shininess;
};

struct AmbientLight {
    vec3 color;
    float strength;
};

struct DirectionalLight {
    vec3 direction;

    vec3 diffuseColor;
    vec3 specularColor;
};

struct PointLight {
    vec3 worldPosition;

    vec3 diffuseColor;
    vec3 specularColor;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 worldPosition;
    vec3 direction;

    float innerCutOffRadiusDegrees;
    float outerCutOffRadiusDegrees;
    
    vec3 diffuseColor;
    vec3 specularColor;

    float constant;
    float linear;
    float quadratic;
};

// The position of the camera
uniform vec3 cameraPosition;

// Ambient light
uniform AmbientLight ambientLight;

// The lights in the scene
// uniform DirectionalLight directionalLight;
// uniform PointLight pointLight;
uniform SpotLight spotLight;

// The material of the object
uniform Material objectMaterial;

// The input position and normal vector of the current fragment
in vec3 fragmentPosition;
in vec3 fragmentNormal;

// The input texture coordinates of the current fragment
in vec2 diffuseMapCoords;
in vec2 specularMapCoords;
in vec2 emissionMapCoords;

// The output color
out vec4 fragmentColor;

void main() {

    // ----- get values from textures ----- //

    vec3 objectDiffuseColor = vec3(texture(objectMaterial.diffuseMap, diffuseMapCoords));
    // vec3 objectSpecularIntensity = vec3(texture(objectMaterial.specularMap, specularMapCoords));
    vec3 objectSpecularIntensity = vec3(1.0, 1.0, 1.0);

    // ----- calculate fragment orientation information and attenuation ----- //

    vec3 normal = normalize(fragmentNormal);
    // vec3 fragmentToLight = normalize(-directionalLight.direction);
    // vec3 fragmentToLight = normalize(pointLight.worldPosition - fragmentPosition);
    vec3 fragmentToLight = normalize(spotLight.worldPosition - fragmentPosition);
    vec3 lightToFragment = -fragmentToLight;

    float theta = dot(
        fragmentToLight,
        normalize(-spotLight.direction)
    );

    float innerCutOffTheta = cos(radians(spotLight.innerCutOffRadiusDegrees));
    float outerCutOffTheta = cos(radians(spotLight.outerCutOffRadiusDegrees));
    float cutOffEpsilon = innerCutOffTheta - outerCutOffTheta;
    float intensity = clamp(
        (theta - outerCutOffTheta) / cutOffEpsilon,
        0.0,
        1.0
    );

    // float fragmentToLightDistance = length(pointLight.worldPosition - fragmentPosition);
    // float attenuation = 1.0 / (
    //     (pointLight.constant) +
    //     (pointLight.linear * fragmentToLightDistance) +
    //     (pointLight.quadratic * (fragmentToLightDistance * fragmentToLightDistance))
    // );
    float fragmentToLightDistance = length(spotLight.worldPosition - fragmentPosition);
    float attenuation = 1.0 / (
        (spotLight.constant) +
        (spotLight.linear * fragmentToLightDistance) +
        (spotLight.quadratic * (fragmentToLightDistance * fragmentToLightDistance))
    );

    // ----- calculate the ambient light amount ----- //

    vec3 ambientResult = (ambientLight.strength * ambientLight.color) * objectDiffuseColor;

    // ----- calculate the diffuse light amount ----- //
    
    vec3 diffuseResult = vec3(0.0, 0.0, 0.0);

    float diffuseImpact = max(
        dot(normal, fragmentToLight),
        0.0
    );

    // diffuseResult = directionalLight.diffuseColor * (diffuseImpact * objectDiffuseColor);
    // diffuseResult = pointLight.diffuseColor * diffuseImpact * objectDiffuseColor;
    diffuseResult = spotLight.diffuseColor * diffuseImpact * objectDiffuseColor;

    diffuseResult *= attenuation;
    diffuseResult *= intensity;

    // ----- calculate the specular light amount ----- //

    vec3 specularResult = vec3(0.0, 0.0, 0.0);

    vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
    vec3 reflectionDirection = reflect(lightToFragment, normal);

    float specularImpact = pow(
        max(
            dot(cameraDirection, reflectionDirection),
            0.0
        ),
        objectMaterial.shininess
    );

    // specularResult = directionalLight.specularColor * (specularImpact * objectSpecularIntensity);
    // specularResult = pointLight.specularColor * specularImpact * objectSpecularIntensity;
    specularResult = spotLight.specularColor * specularImpact * objectSpecularIntensity;

    specularResult *= attenuation;
    specularResult *= intensity;

    // ----- calculate the emission amount ----- //

    vec3 emissionResult = vec3(0.0, 0.0, 0.0);
    if (objectSpecularIntensity == vec3(0.0, 0.0, 0.0)) {
        emissionResult = texture(objectMaterial.emissionMap, emissionMapCoords).rgb;
    }

    // ----- calculate the result ----- //

    fragmentColor = vec4(ambientResult + diffuseResult + specularResult + emissionResult, 1.0);

}
