#version 330 core
out vec4 FragColor;
in vec4 color;

void main()
{
    FragColor = color;
    FragColor = vec4(0.6, 0.6, 1.0, 1.0);
}
