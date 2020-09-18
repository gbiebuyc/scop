#version 330 core
out vec4 FragColor;
in vec3 pos;
in vec3 FragPos;
uniform sampler2D ourTexture;
uniform float mix_value;
uniform int transition[2];

vec4 effect_0() {
	float factor = gl_PrimitiveID % 5 * 0.1 + 0.05;
	return (vec4(factor, factor, factor, 1));
}

vec4 effect_1() {
	return (texture(ourTexture, vec2(pos.z, -pos.y)));
}

vec4 effect_2() {
	vec3 color = vec3(.5, .5, .5);
	vec3 xTangent = dFdx( FragPos );
	vec3 yTangent = dFdy( FragPos );
	vec3 faceNormal = normalize( cross( xTangent, yTangent ) );
	vec3 lightPos  = vec3(0, 1, 3);
	vec3 lightColor = vec3(1, 1, 1);
	vec3 lightDir = normalize(lightPos  - FragPos);
	float diff = max(dot(faceNormal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	float specularStrength = 0.5;
	vec3 viewDir = normalize( - FragPos);
	vec3 reflectDir = reflect(-lightDir, faceNormal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	return (vec4(((ambient + diffuse + specular) * color), 1));
}

void main() {
	vec4 results[3];
	results[0] = effect_0();
	results[1] = effect_1();
	results[2] = effect_2();
	FragColor = mix(results[transition[1]], results[transition[0]], mix_value);
}
