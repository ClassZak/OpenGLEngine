#shader vertex
#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoords;

out vec2 v_TexCoords;

void main()
{
	v_TexCoords = aTexCoords;
	gl_Position = vec4(aPosition, 0.0, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 v_TexCoords;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoords);
	FragColor = texColor * u_Color;
}