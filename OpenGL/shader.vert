#version 330 core
layout (location = 0) in vec3 position;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
}