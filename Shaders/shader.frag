#version 330

out vec4 colour;
in vec4 vCol;
in vec2 TexCoord;

uniform sampler2D texture2D;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;
    colour = texture(texture2D, TexCoord) * vec4(ambient, 1.0);
}
