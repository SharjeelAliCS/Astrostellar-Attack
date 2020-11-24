#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;
uniform vec3 glow = vec3(0,0,0);

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
	
	color.r +=glow.r;
	color.g +=glow.g;
	color.b +=glow.b;

}  