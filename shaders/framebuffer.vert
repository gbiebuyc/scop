#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = (aPos + 1.0f) / 2.0f;
}
