#version 330
layout (location = 0) in vec3 pos;

out vec4 fragPosLightSpace;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightProjection;
uniform mat4 lightView;

void main()
{
    vec4 fragPos = model * vec4(pos, 1.0);
    fragPosLightSpace = lightProjection * lightView * fragPos;
    gl_Position = projection * view * fragPos;
}