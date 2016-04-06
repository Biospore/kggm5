#version 330

in vec3 colorV;

out vec3 outputF;

void main()
{
//    outputF = vec4(0,1,0,1);
    outputF = colorV;
}