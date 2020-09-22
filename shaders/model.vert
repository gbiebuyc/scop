#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 pos;
out vec3 normal;
out vec2 texCoord;
out vec3 posWorldSpace;
out vec3 posCamSpace;
out vec3 v_barycentric; // barycentric coordinate inside the triangle

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	pos = aPos;
	posWorldSpace = vec3(model * vec4(aPos, 1.0));
	posCamSpace = vec3(view * model * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(model))) * aNormal;
	texCoord = aTexCoord;
	if (gl_VertexID % 3 == 0)
		v_barycentric = vec3(1, 0, 0);
	else if (gl_VertexID % 3 == 1)
		v_barycentric = vec3(0, 1, 0);
	else if (gl_VertexID % 3 == 2)
		v_barycentric = vec3(0, 0, 1);
}
