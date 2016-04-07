#version 330

//
uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;



layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 color;



//out vec3 colorV;

void main()
{
    mat4 depthMVP = gl_ProjectionMatrix * gl_ModelViewMatrix;
    gl_Position = depthMVP * vec4(position, 1);
}