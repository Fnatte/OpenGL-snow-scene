#version 150

//in vec2 fragTexCoord;
in vec3 fragPosition;
in vec3 fragNormal;
in mat4 fragInstanceTrans;

uniform mat4 model;
//uniform sampler2D material;

out vec4 out_Color;

uniform struct Light {
    vec3 position;
    vec3 intensities;
    vec3 coneDirection;
    float ambientCoefficient;
    float coneAngle;
} light;

vec3 applyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos) {
    float materialShininess = 1.0;
    vec3 materialSpecularColor = vec3(1, 1, 1);

    vec3 surfaceToLight = normalize(light.position - surfacePos);
    float distanceToLight = length(light.position - surfacePos);
    float attenuation = 1.0 / (0.0001 * pow(distanceToLight, 3));

    // Cone restrictions
    float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
    if(lightToSurfaceAngle > light.coneAngle / 2) {
        attenuation = 0.0;
    }

   float diffuseCoefficient = 1.0;
   float specularCoefficient = 0.1;

    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

    return ambient + attenuation * (diffuse + specular);
}

void main(void) {
    mat4 modelInstance = model * fragInstanceTrans;
	vec3 normal = normalize(transpose(inverse(mat3(modelInstance))) * fragNormal);

    vec3 surfacePos = vec3(modelInstance * vec4(fragPosition, 1));
    vec4 surfaceColor = vec4(1); //texture(material, fragTexCoord);

	out_Color = vec4(
        applyLight(light, surfaceColor.rgb, normal, surfacePos),
        surfaceColor.a
    );
}
