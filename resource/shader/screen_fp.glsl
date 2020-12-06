#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;

// Uniform (global) buffer
uniform sampler2D texture_map;
uniform vec2 progress_size;
uniform vec3 glow = vec3(0,0,0);

void main() 
{
    // Retrieve texture value
	vec4 pixel = texture(texture_map, uv_interp);
	gl_FragColor = pixel;

	gl_FragColor.r +=glow.r;
	gl_FragColor.g +=glow.g;
	gl_FragColor.b +=glow.b;

	if(uv_interp.x > progress_size.x){
		gl_FragColor.a = 0;
	}
	if(uv_interp.y < 1.0- progress_size.y ){
		gl_FragColor.a = 0;
	}
    // Use texture in determining fragment colour
    
}
