#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;

// Uniform (global) buffer
uniform sampler2D texture_map;
uniform vec3 color = vec3(1.0);


void main() 
{
    // Retrieve texture value
    vec4 pixel = texture(texture_map, uv_interp);
	pixel  = vec4(pixel.r*color.r,pixel.g*color.g,pixel.b*color.b,pixel.a);
    // Use texture in determining fragment colour
    gl_FragColor = pixel;
}
