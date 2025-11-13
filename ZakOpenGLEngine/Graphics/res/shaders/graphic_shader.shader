#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
uniform float time;
void main()
{
	float y = sin(aPos.x + time);
	gl_Position = vec4(aPos.x, y, 0.0, 1.0);
}
	

#shader fragment
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Зеленый цвет
}