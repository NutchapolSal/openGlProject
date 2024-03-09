#version 330

out vec4 colour;
in vec4 fragPosLightSpace;
uniform vec3 bgColour;
uniform sampler2D shadowMap;
float ShadowCalculation() {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}
void main() {
    float shadow = ShadowCalculation();
    colour = vec4((1.0 - shadow) * bgColour, 1.0f);
}