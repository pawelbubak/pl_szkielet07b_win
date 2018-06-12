#version 330 core

in vec4 aPos;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{
    TexCoords = aPos.xyz;

    gl_Position = P*V*vec4(aPos.xyz,0.5);
}


