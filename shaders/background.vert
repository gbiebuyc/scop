#version 330 core
layout (location = 0) in vec2 pos;
uniform bool isCellShading;
out vec4 color;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
	if (isCellShading) {
		color = vec4(0.6, 0.6, 1.0, 1.0);
	} else {
		color = vec4(-pos.y / 4 + 0.5, -pos.y / 4 + 0.5, 1, 1);
	}
}
