// Material with no illumination simulation

#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;

uniform vec3 color;

void main() 
{
	gl_FragColor = vec4(color.r,color.g,color.b, 1.0);
}
