#version 330

out vec4 colour;
in vec4 vCol;
in vec2 TexCoord;

uniform sampler2D texture2D;
uniform vec3 lightColor;

vec4 ambientLight() {
    float ambientStrength = 0.3f;
    vec4 ambient = vec4(ambientStrength * lightColor, 1.0);
    return ambient;
}

void main()
{
    colour = texture(texture2D, TexCoord) * ambientLight();
}
