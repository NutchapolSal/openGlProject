#version 330

out vec4 colour;
in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 ambientLight() {
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
    return vec3(texture(diffuseMap, TexCoord)) * ambient;
}

vec3 diffuseLight() {
    float diffuseStrength = 0.8f;
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = lightColor * diff * diffuseStrength;
    return vec3(texture(diffuseMap, TexCoord)) * diffuse;
}

vec3 specularLight() {
    float specularStrength = 0.8f;
    float shininess = 256.0f;

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 halfDir = (viewDir + lightDir) / 2.0f;
    float spec = pow(max(dot(halfDir, norm), 0.0f), shininess);

    vec3 specular = specularStrength * spec * lightColor;

    return vec3(texture(specularMap, TexCoord)) * specular;
}

void main()
{
    colour =  vec4(ambientLight() + diffuseLight() + specularLight(), 1.0f);
}
