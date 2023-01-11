#version 330 core

// --------------------------------------------------------------------------------
// The structs representing the data we need
// --------------------------------------------------------------------------------

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

// --------------------------------------------------------------------------------
// The input uniforms from the cpu
// --------------------------------------------------------------------------------

// The position of the camera
uniform vec3 cameraPosition;

// Ambient light
uniform AmbientLight ambientLight;

// The lights in the scene
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

// The material of the object
uniform Material objectMaterial;

// --------------------------------------------------------------------------------
// The input values from the vertex shader
// --------------------------------------------------------------------------------

// The input position and normal vector of the current fragment
in vec3 fragmentPosition;
in vec3 fragmentNormal;

// The input texture coordinates of the current fragment
in vec2 diffuseMapCoords;
in vec2 specularMapCoords;
in vec2 emissionMapCoords;

// The output color
out vec4 fragmentColor;

// --------------------------------------------------------------------------------
// Functions to calculate how much color a given light contributes to this fragment
// --------------------------------------------------------------------------------

vec3 getDiffuseContribution(
    vec3 normal,
    vec3 fragmentToLightDirection,
    vec3 lightDiffuseColor,
    vec3 objectDiffuseColor
);

vec3 getSpecularContribution(
    vec3 normal,
    vec3 lightToFragmentDirection,
    vec3 fragmentToCameraDirection,
    vec3 lightSpecularColor,
    vec3 objectSpecularIntensity,
    float objectShininess
);

float calculateAttenuation(
    vec3 lightWorldPosition,
    vec3 fragmentPosition,
    float lightConstant,
    float lightLinear,
    float lightQuadratic
);

float calculateDirectionalIntensity(
    vec3 lightDirection,
    vec3 fragmentToLightDirection,
    float innerCutOffRadiusDegrees,
    float outerCutOffRadiusDegrees
);

vec3 getAmbientLightContribution(
    AmbientLight currentAmbientLight,
    vec3 objectDiffuseColor
);

vec3 getDirectionalLightContribution(
    DirectionalLight currentDirectionalLight,
    vec3 fragmentPosition,
    vec3 cameraPosition,
    vec3 normal,
    vec3 objectDiffuseColor,
    vec3 objectSpecularIntensity,
    float objectShininess
);

vec3 getPointLightContribution(
    PointLight currentPointLight,
    vec3 fragmentPosition,
    vec3 cameraPosition,
    vec3 normal,
    vec3 objectDiffuseColor,
    vec3 objectSpecularIntensity,
    float objectShininess
);

vec3 getSpotLightContribution(
    SpotLight currentSpotLight,
    vec3 fragmentPosition,
    vec3 cameraPosition,
    vec3 normal,
    vec3 objectDiffuseColor,
    vec3 objectSpecularIntensity,
    float objectShininess
);

// --------------------------------------------------------------------------------
// The main function
// --------------------------------------------------------------------------------

void main() {

    // ----- get values from textures ----- //

    vec3 objectDiffuseColor = vec3(texture(objectMaterial.diffuseMap, diffuseMapCoords));
    // vec3 objectSpecularIntensity = vec3(texture(objectMaterial.specularMap, specularMapCoords));
    vec3 objectSpecularIntensity = vec3(1.0, 1.0, 1.0);
    vec3 normal = normalize(fragmentNormal);

    // ----- calculate the ambient light amount ----- //

    vec3 ambientResult = getAmbientLightContribution(ambientLight, objectDiffuseColor);

    // ----- calculate contribution of directional lights ----- //

    vec3 directionalLightContribution = getDirectionalLightContribution(
        directionalLight,
        fragmentPosition,
        cameraPosition,
        normal,
        objectDiffuseColor,
        objectSpecularIntensity,
        objectMaterial.shininess
    );

    // ----- calculate contribution of point lights ----- //

    vec3 pointLightContribution = getPointLightContribution(
        pointLight,
        fragmentPosition,
        cameraPosition,
        normal,
        objectDiffuseColor,
        objectSpecularIntensity,
        objectMaterial.shininess
    );

    // ----- calculate contribution of spot lights ----- //

    vec3 spotLightContribution = getSpotLightContribution(
        spotLight,
        fragmentPosition,
        cameraPosition,
        normal,
        objectDiffuseColor,
        objectSpecularIntensity,
        objectMaterial.shininess
    );

    // ----- calculate the emission amount ----- //

    vec3 emissionResult = vec3(0.0, 0.0, 0.0);
    if (objectSpecularIntensity == vec3(0.0, 0.0, 0.0)) {
        emissionResult = texture(objectMaterial.emissionMap, emissionMapCoords).rgb;
    }

    // ----- calculate the result ----- //

    vec3 fullResult = vec3(0.0, 0.0, 0.0);
    fullResult = ambientResult;
    // fullResult += directionalLightContribution;
    // fullResult += pointLightContribution;
    fullResult += spotLightContribution;
    fullResult += emissionResult;
    fragmentColor = vec4(fullResult, 1.0);
}

// --------------------------------------------------------------------------------
// The function to determine how much the light's diffuse color is effecting the
// object
// --------------------------------------------------------------------------------

vec3 getDiffuseContribution(
    vec3 normal,
    vec3 fragmentToLightDirection,
    vec3 lightDiffuseColor,
    vec3 objectDiffuseColor
) {
    float diffuseImpact = max(
        dot(normal, fragmentToLightDirection),
        0.0
    );

    vec3 diffuseResult = lightDiffuseColor * (diffuseImpact * objectDiffuseColor);

    return diffuseResult;
}

// --------------------------------------------------------------------------------
// The function to determine how much the specular light contributes to this
// fragment's color
// --------------------------------------------------------------------------------

vec3 getSpecularContribution(
    vec3 normal,
    vec3 lightToFragmentDirection,
    vec3 fragmentToCameraDirection,
    vec3 lightSpecularColor,
    vec3 objectSpecularIntensity,
    float objectShininess
) {
    vec3 reflectionDirection = reflect(lightToFragmentDirection, normal);

    float specularImpact = pow(
        max(
            dot(fragmentToCameraDirection, reflectionDirection),
            0.0
        ),
        objectShininess
    );

    vec3 specularResult = lightSpecularColor * (specularImpact * objectSpecularIntensity);

    return specularResult;
}

// --------------------------------------------------------------------------------
// The function to determine how much the attenuation value so we can properly
// scale how much a light effects fragments further away
// --------------------------------------------------------------------------------

float calculateAttenuation(
    vec3 lightWorldPosition,
    vec3 fragmentPosition,
    float lightConstant,
    float lightLinear,
    float lightQuadratic
) {
    float fragmentToLightDistance = length(lightWorldPosition - fragmentPosition);
    float attenuation = 1.0 / (
        (lightConstant) +
        (lightLinear * fragmentToLightDistance) +
        (lightQuadratic * (fragmentToLightDistance * fragmentToLightDistance))
    );

    return attenuation;
}

// --------------------------------------------------------------------------------
// The function to determine how much intensity the directional spot light has
// on the current fragment based on its orientation
// --------------------------------------------------------------------------------

float calculateDirectionalIntensity(
    vec3 lightPointingDirection,
    vec3 fragmentToLightDirection,
    float innerCutOffRadiusDegrees,
    float outerCutOffRadiusDegrees
) {
    float theta = dot(
        fragmentToLightDirection,
        normalize(-lightPointingDirection)
    );

    float innerCutOffTheta = cos(radians(innerCutOffRadiusDegrees));
    float outerCutOffTheta = cos(radians(outerCutOffRadiusDegrees));
    float cutOffEpsilon = innerCutOffTheta - outerCutOffTheta;

    float intensity = clamp(
        (theta - outerCutOffTheta) / cutOffEpsilon,
        0.0,
        1.0
    );

    return intensity;
}

// --------------------------------------------------------------------------------
// The function to determine how much the ambient light contributes to this
// fragment's color
// --------------------------------------------------------------------------------

vec3 getAmbientLightContribution(
    AmbientLight currentAmbientLight,
    vec3 objectDiffuseColor
) {
    vec3 ambientResult = (currentAmbientLight.strength * currentAmbientLight.color) * objectDiffuseColor;
    return ambientResult;
}

// --------------------------------------------------------------------------------
// The function to determine how much the directional light contributes to this
// fragment's color
// --------------------------------------------------------------------------------

vec3 getDirectionalLightContribution(
    DirectionalLight currentDirectionalLight,
    vec3 fragmentPosition,
    vec3 cameraPosition,
    vec3 normal,
    vec3 objectDiffuseColor,
    vec3 objectSpecularIntensity,
    float objectShininess
) {
    vec3 fragmentToLightDirection = normalize(-currentDirectionalLight.direction);
    vec3 lightToFragmentDirection = normalize(currentDirectionalLight.direction);
    vec3 fragmentToCameraDirection = normalize(cameraPosition - fragmentPosition);

    // ----- diffuse contribution ----- //

    vec3 diffuseResult = getDiffuseContribution(
        normal,
        fragmentToLightDirection,
        currentDirectionalLight.diffuseColor,
        objectDiffuseColor
    );
    
    // ----- specular contribution ----- //
    
    vec3 specularResult = getSpecularContribution(
        normal,
        lightToFragmentDirection,
        fragmentToCameraDirection,
        currentDirectionalLight.specularColor,
        objectSpecularIntensity,
        objectShininess
    );

    // ----- calculate total contribution ----- //

    return (diffuseResult + specularResult);
}

// --------------------------------------------------------------------------------
// The function to determine how much the point light contributes to this
// fragment's color
// --------------------------------------------------------------------------------

vec3 getPointLightContribution(
    PointLight currentPointLight,
    vec3 fragmentPosition,
    vec3 cameraPosition,
    vec3 normal,
    vec3 objectDiffuseColor,
    vec3 objectSpecularIntensity,
    float objectShininess
) {
    vec3 fragmentToLightDirection = normalize(currentPointLight.worldPosition - fragmentPosition);
    vec3 lightToFragmentDirection = -fragmentToLightDirection;
    vec3 fragmentToCameraDirection = normalize(cameraPosition - fragmentPosition);

    // ----- diffuse contribution ----- //

    vec3 diffuseResult = getDiffuseContribution(
        normal,
        fragmentToLightDirection,
        currentPointLight.diffuseColor,
        objectDiffuseColor
    );
    
    // ----- specular contribution ----- //
    
    vec3 specularResult = getSpecularContribution(
        normal,
        lightToFragmentDirection,
        fragmentToCameraDirection,
        currentPointLight.specularColor,
        objectSpecularIntensity,
        objectShininess
    );

    // ----- calculate attenuation and adjust for it ----- //

    float attenuation = calculateAttenuation(
        currentPointLight.worldPosition,
        fragmentPosition,
        currentPointLight.constant,
        currentPointLight.linear,
        currentPointLight.quadratic
    );

    diffuseResult *= attenuation;
    specularResult *= attenuation;

    // ----- calculate total contribution ----- //

    return (diffuseResult + specularResult);
}

// --------------------------------------------------------------------------------
// The function to determine how much the spot light contributes to this
// fragment's color
// --------------------------------------------------------------------------------

vec3 getSpotLightContribution(
    SpotLight currentSpotLight,
    vec3 fragmentPosition,
    vec3 cameraPosition,
    vec3 normal,
    vec3 objectDiffuseColor,
    vec3 objectSpecularIntensity,
    float objectShininess
) {
    vec3 fragmentToLightDirection = normalize(currentSpotLight.worldPosition - fragmentPosition);
    vec3 lightToFragmentDirection = -fragmentToLightDirection;
    vec3 fragmentToCameraDirection = normalize(cameraPosition - fragmentPosition);

    // ----- diffuse contribution ----- //

    vec3 diffuseResult = getDiffuseContribution(
        normal,
        fragmentToLightDirection,
        currentSpotLight.diffuseColor,
        objectDiffuseColor
    );
    
    // ----- specular contribution ----- //
    
    vec3 specularResult = getSpecularContribution(
        normal,
        lightToFragmentDirection,
        fragmentToCameraDirection,
        currentSpotLight.specularColor,
        objectSpecularIntensity,
        objectShininess
    );

    // ----- calculate attenuation and adjust for it ----- //

    float attenuation = calculateAttenuation(
        currentSpotLight.worldPosition,
        fragmentPosition,
        currentSpotLight.constant,
        currentSpotLight.linear,
        currentSpotLight.quadratic
    );

    diffuseResult *= attenuation;
    specularResult *= attenuation;
    
    // ----- calculate directional intensity and adjust for it ----- //

    float intensity = calculateDirectionalIntensity(
        currentSpotLight.direction,
        fragmentToLightDirection,
        currentSpotLight.innerCutOffRadiusDegrees,
        currentSpotLight.outerCutOffRadiusDegrees
    );

    diffuseResult *= intensity;
    specularResult *= intensity;

    // ----- calculate total contribution ----- //

    return (diffuseResult + specularResult);
}
