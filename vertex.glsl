#version 330

//
uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;

uniform mat4 depthBiasMatrix;
//uniform mat4 finalMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;


out vec3 colorV;
out vec4 shadowCoord;


void main()
{
    mat4 finalMatrix = gl_ModelViewMatrix * gl_ProjectionMatrix;
    gl_Position = finalMatrix * vec4(position, 1);
    shadowCoord = depthBiasMatrix * vec4(position, 1);
    colorV = color;
}