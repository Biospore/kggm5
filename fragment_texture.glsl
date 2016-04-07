#version 330

//in vec3 colorV;

out vec3 outputF;
//layout(location = 0) out float fragmentdepth;

void main()
{
//    outputF = vec3(0,1,0);
    outputF = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);
//      fragmentdepth = gl_FragCoord.z + 10;
}