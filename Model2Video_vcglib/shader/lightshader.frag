#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace0;
in vec3 LightDirection_cameraspace1;
in vec3 LightDirection_cameraspace2;
in vec3 LightDirection_cameraspace3;
in vec3 LightDirection_cameraspace4;
in vec3 LightDirection_cameraspace5;
in vec3 LightDirection_cameraspace6;
in vec3 LightDirection_cameraspace7;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 MV;
uniform vec3 LightPosition_worldspace0;
uniform vec3 LightPosition_worldspace1;
uniform vec3 LightPosition_worldspace2;
uniform vec3 LightPosition_worldspace3;
uniform vec3 LightPosition_worldspace4;
uniform vec3 LightPosition_worldspace5;
uniform vec3 LightPosition_worldspace6;
uniform vec3 LightPosition_worldspace7;

void main(){

        // Light emission properties
        // You probably want to put them as uniforms
        vec3 LightColor = vec3(1,1,1);
        float LightPower = 0.35f;
        float LightPower1 = 0.25f;
//        float LightPower1 = 0.f;
        float LightPower2 = 0.32f;
        float LightPower3 = 0.27f;
        float LightPower4 = 0.1f;
        float LightPower5 = 0.1f;
        float LightPower6 = 0.2f;
        float LightPower7 = 0.1f;

        // Material properties
        vec3 MaterialDiffuseColor = vec3(0.752);
        vec3 MaterialAmbientColor = vec3(0.2,0.2,0.2) * MaterialDiffuseColor;
//        vec3 MaterialAmbientColor = vec3(1.0,1.0,1.0) * MaterialDiffuseColor;
        vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

        // Distance to the light
        float distance = length( LightPosition_worldspace0 - Position_worldspace );

        // Normal of the computed fragment, in camera space
        vec3 n = normalize( Normal_cameraspace );
        // Direction of the light (from the fragment to the light)
        vec3 l = normalize( LightDirection_cameraspace0 );
        // Cosine of the angle between the normal and the light direction,
        // clamped above 0
        //  - light is at the vertical of the triangle -> 1
        //  - light is perpendicular to the triangle -> 0
        //  - light is behind the triangle -> 0
        float cosTheta = clamp( dot( n,l ), 0,1 );

        vec3 l1 = normalize( LightDirection_cameraspace1 );
        float cosTheta1 = clamp( dot( n,l1 ), 0,1 );

        vec3 l2 = normalize( LightDirection_cameraspace2 );
        float cosTheta2 = clamp( dot( n,l2 ), 0,1 );

        vec3 l3 = normalize( LightDirection_cameraspace3 );
        float cosTheta3 = clamp( dot( n,l3 ), 0,1 );

        vec3 l4 = normalize( LightDirection_cameraspace1 );
        float cosTheta4 = clamp( dot( n,l4 ), 0,1 );

        vec3 l5 = normalize( LightDirection_cameraspace1 );
        float cosTheta5 = clamp( dot( n,l5 ), 0,1 );

        vec3 l6 = normalize( LightDirection_cameraspace1 );
        float cosTheta6 = clamp( dot( n,l6 ), 0,1 );

        vec3 l7 = normalize( LightDirection_cameraspace1 );
        float cosTheta7 = clamp( dot( n,l7 ), 0,1 );

        // Eye vector (towards the camera)
        vec3 E = normalize(EyeDirection_cameraspace);
        // Direction in which the triangle reflects the light
        vec3 R = reflect(-l,n);
        // Cosine of the angle between the Eye vector and the Reflect vector,
        // clamped to 0
        //  - Looking into the reflection -> 1
        //  - Looking elsewhere -> < 1
        float cosAlpha = clamp( dot( E,R ), 0,1 );

        color =
                // Diffuse : "color" of the object
//                 MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance);
                 MaterialDiffuseColor * LightColor * LightPower * cosTheta
                +MaterialDiffuseColor * LightColor * LightPower1 * cosTheta1
                +MaterialDiffuseColor * LightColor * LightPower2 * cosTheta2
                +MaterialDiffuseColor * LightColor * LightPower3 * cosTheta3
                +MaterialDiffuseColor * LightColor * LightPower4 * cosTheta4
                +MaterialDiffuseColor * LightColor * LightPower5 * cosTheta5
                +MaterialDiffuseColor * LightColor * LightPower6 * cosTheta6
                +MaterialDiffuseColor * LightColor * LightPower7 * cosTheta7;

        //color = vec3(0.752);

}
