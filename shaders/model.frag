#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;
in vec3 pos;
in vec3 normal;
in vec2 texCoord;
in vec3 posWorldSpace;
in vec3 posCamSpace;
uniform sampler2D ourTexture;
uniform samplerCube skybox;
uniform float mix_value;
uniform int transition[2];
uniform vec3 cameraPos;
in vec3 vBC;
in vec3 normalCamSpace;

vec3 get_normal() {
	if (isnan(normal.x)) {
		vec3 xTangent = dFdx( posCamSpace );
		vec3 yTangent = dFdy( posCamSpace );
		return normalize( cross( xTangent, yTangent ) );
	} else {
		return normal;
	}
}

vec4 effect_shades_of_grey() {
	Normal = normalCamSpace;
	vec3 viewDirection = normalize(posWorldSpace - cameraPos);
	vec3 normalDirection = normalize(get_normal());
	float CameraFacingPercentage = abs(dot(viewDirection, normalDirection));
	vec4 ColorA = vec4(1);
	vec4 ColorB = vec4(0);
	return ColorA * CameraFacingPercentage + ColorB * (1 - CameraFacingPercentage);
	if (abs(dot(viewDirection, normalDirection)) < 0.1f)
		return vec4(0);
	return vec4(1);
	float factor = gl_PrimitiveID % 5 * 0.1 + 0.05;
	return (vec4(factor, factor, factor, 1));
}

vec4 effect_texture() {
	vec2 p;
	if (isnan(texCoord.x)) {
		float tex_scale = 3.0f;
		p = vec2(pos.z, -pos.y) * tex_scale;
		p.y += 0.5f;
	} else {
		p = vec2(texCoord.x, -texCoord.y);
	}
	return (texture(ourTexture, p));
}

vec4 effect_lighting(vec3 color) {
	vec3 theNormal = get_normal();
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

vec4 effect_reflection() {
	vec3 I = normalize(posWorldSpace - cameraPos);
	vec3 R = reflect(I, normalize(get_normal()));
	return texture(skybox, R);
}

vec4 effect_refraction() {
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(posWorldSpace - cameraPos);
	vec3 R = refract(I, normalize(get_normal()), ratio);
	return texture(skybox, R);
}

vec4 effect_wireframe(){
	vec3 d = fwidth(vBC);
	vec3 a3 = smoothstep(vec3(0.0), d*1.5, vBC);
	float factor = 1 - min(min(a3.x, a3.y), a3.z);
	return vec4(0, 0, 0, factor*0.3f);
}
