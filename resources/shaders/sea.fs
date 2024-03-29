#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D t0;

void main()
{
	FragColor = texture(t0, TexCoord);
}