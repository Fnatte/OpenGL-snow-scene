#version 150

in vec4 lightSourceCoord;
in vec2 fragTexCoord;
in vec3 fragPosition;
in vec3 fragNormal;

uniform vec3 cameraPosition;
uniform mat4 model;
uniform sampler2D material;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

out vec4 out_Color;

uniform struct Light {
    vec3 position;
    vec3 intensities;
    float attenuation;
    float ambientCoefficient;
    float coneAngle;
    vec3 coneDirection;
} light;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    float materialShininess = 1.0;
    vec3 materialSpecularColor = vec3(1, 1, 1);

    vec3 surfaceToLight = normalize(light.position - surfacePos);
    float distanceToLight = length(light.position - surfacePos);
    float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

   // Cone restrictions
   float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
   if(lightToSurfaceAngle > light.coneAngle) {
    attenuation = 0.0;
   }

   float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
   float specularCoefficient = diffuseCoefficient > 0 ?
        pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess) : 0;

   vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;
   vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
   vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

   return ambient + attenuation * (diffuse + specular);
}

float GetShadow() {
    // Perform perspective division to get the actual texture position
    vec4 shadowCoordinateWdivide = lightSourceCoord / lightSourceCoord.w;

    // So this sucks. But as of now we're not sure how to clamp the shadow
    if(shadowCoordinateWdivide.x < 0 || shadowCoordinateWdivide.y < 0 || shadowCoordinateWdivide.x > 1 || shadowCoordinateWdivide.y > 1) {
        return 1.0;
    }

    // Used to lower moire' pattern and self-shadowing
    // The optimal value here will vary with different GPU's depending on their Z buffer resolution.
    shadowCoordinateWdivide.z -= 0.002;

    // Look up the depth value
    float distanceFromLight = texture(shadowMap, shadowCoordinateWdivide.st).x;
    distanceFromLight = (distanceFromLight-0.5) * 2.0;

    // NOTE: This distance look-up disturbs me. It is too simple. It should really
    // use the camera parameters to correctly restore the actual distance.
    // For the moment I don't have time to fix this. The demo works, but it
    // may have hit some constants that are correct more by luck than skill.
    // This is regrettable and I will correct this when I have time. In the meantime
    // I do not want to withhold the demo. /Ingemar

    // Compare
    float shadow = 1.0; // 1.0 = no shadow

    if (lightSourceCoord.w > 0.0)
        if (distanceFromLight < shadowCoordinateWdivide.z) // shadow
            shadow = 0.2;

    return shadow;
}

void main()
{
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);

    vec3 surfacePos = vec3(model * vec4(fragPosition, 1));
    vec4 surfaceColor = texture(material, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

	out_Color = vec4(
        GetShadow() * ApplyLight(light, surfaceColor.rgb, normal, surfacePos, surfaceToCamera),
        surfaceColor.a
    );
}
