#version 330 core
out vec4 FragColor;
in vec3 pos;
in vec3 normal;
in vec2 texCoord;
in vec3 posWorldSpace;
in vec3 posCamSpace;
//uniform sampler2D ourTexture;
uniform samplerCube ourTexture;
uniform float mix_value;
uniform int transition[2];
uniform vec3 cameraPos;

vec4 effect_0() {
	float factor = gl_PrimitiveID % 5 * 0.1 + 0.05;
	return (vec4(factor, factor, factor, 1));
}

vec4 effect_1(vec3 theNormal) {
	vec3 I = normalize(posWorldSpace - cameraPos);
	vec3 R = reflect(I, normalize(theNormal));
	return texture(ourTexture, R);
	vec2 p;
	if (isnan(texCoord.x)) {
		float tex_scale = 3.0f;
		p = vec2(pos.z, -pos.y) * tex_scale;
		p.y += 0.5f;
	} else {
		p = vec2(texCoord.x, -texCoord.y);
	}
	//return (texture(ourTexture, p));
	return vec4(1,0,0,0);
}

vec4 effect_2(vec3 theNormal) {
	//vec3 color = vec3(.5, .5, .5);
	vec3 color = effect_1(theNormal).rgb;
	vec3 lightPos  = vec3(0, 1, 3);
	vec3 lightColor = vec3(1, 1, 1);
	vec3 lightDir = normalize(lightPos  - posCamSpace);
	float diff = max(dot(theNormal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	float specularStrength = 0.5;
	vec3 viewDir = normalize( - posCamSpace);
	vec3 reflectDir = reflect(-lightDir, theNormal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	return (vec4(((ambient + diffuse + specular) * color), 1));
}

void main() {
	vec3 theNormal;
	if (isnan(normal.x)) {
		vec3 xTangent = dFdx( posCamSpace );
		vec3 yTangent = dFdy( posCamSpace );
		theNormal = normalize( cross( xTangent, yTangent ) );
	} else {
		theNormal = normal;
	}
	vec4 results[3];
	results[0] = effect_0();
	results[1] = effect_1(theNormal);
	results[2] = effect_2(theNormal);
	FragColor = mix(results[transition[1]], results[transition[0]], mix_value);
}
