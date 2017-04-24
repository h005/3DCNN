#version 330 core

layout(location = 0) in vec3 vertexNormal_modelspace;
layout(location = 1) in vec3 vertexPosition_modelspace;


// Output data ; will be interpolated for each fragment.
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 LightDirection_cameraspace1;
out vec3 LightDirection_cameraspace2;
out vec3 LightDirection_cameraspace3;
out vec3 LightDirection_cameraspace4;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 normalMatrix;
uniform vec3 LightPosition_worldspace;
uniform vec3 LightPosition_worldspace1;
uniform vec3 LightPosition_worldspace2;
uniform vec3 LightPosition_worldspace3;
uniform vec3 LightPosition_worldspace4;


void main()
{
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

    // Position of the vertex, in worldspace : M * position
    Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
    vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,0)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace1,0)).xyz;
    LightDirection_cameraspace1 = LightPosition_cameraspace + EyeDirection_cameraspace;

    LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace2,0)).xyz;
    LightDirection_cameraspace2 = LightPosition_cameraspace + EyeDirection_cameraspace;

    LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace3,0)).xyz;
    LightDirection_cameraspace3 = LightPosition_cameraspace + EyeDirection_cameraspace;

    LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace4,0)).xyz;
    LightDirection_cameraspace4 = LightPosition_cameraspace + EyeDirection_cameraspace;

    // Normal of the the vertex, in camera space
    Normal_cameraspace = ( normalMatrix * vec4(vertexNormal_modelspace,0)).xyz;
}
