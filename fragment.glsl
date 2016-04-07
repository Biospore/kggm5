#version 330

in vec3 colorV;
in vec4 shadowCoord;

out vec3 outputF;

uniform sampler2D shadowMap;


vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

void main()
{
//    outputF = vec3(0,1,0);
    float visibility = 1.0;
    float bias = 0.002;

//    bias = clamp(bias, 0, 0.003);
//    if (texture ( shadowMap, shadowCoord.xy ).z < shadowCoord.z - bias)
//    {
//        visibility = 0.7;
//    }
    for (int i = 0; i < 4; i++)
    {
        if ( texture( shadowMap, shadowCoord.xy + poissonDisk[i]/600.0 ).z  <  shadowCoord.z-bias ){
            visibility-=0.2;
          }
    }
    outputF = colorV * visibility;

//    outputF = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);
}