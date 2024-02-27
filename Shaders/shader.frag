#version 330

out vec4 colour;
in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture2D;
uniform vec3 lightColor;
uniform vec3 lightPos;

vec3 ambientLight() {
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
    return ambient;
}

vec3 diffuseLight() {
    float diffuseStrength = 0.8f;
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = lightColor * diff * diffuseStrength;
    return diffuse;
}

void main()
{
    colour = texture(texture2D, TexCoord) * vec4(ambientLight() + diffuseLight(), 1.0f);
}
