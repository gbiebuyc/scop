#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;
in vec4 color;

void main()
{
    FragColor = color;
	Normal = vec3(0,0,1);
}
