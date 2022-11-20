#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;

// Uniform (global) buffer
uniform sampler2D texture_map;
uniform int arr_size;
uniform vec2 dots_pos[100];
uniform vec3 dot_colors[100];
void main() 
{
    // Retrieve texture value
	vec4 pixel = texture(texture_map, uv_interp);
	gl_FragColor = pixel;

	//iterate through each dot. Then check if the uv coordiante is near the dot position, if so set its color. 
	for(int i =0; i<arr_size; i++){
		float dis = distance(dots_pos[i], uv_interp);
		if(dis <0.03){
			gl_FragColor.r = dot_colors[i].r;
			gl_FragColor.g =dot_colors[i].g;
			gl_FragColor.b =dot_colors[i].b;
		}
	}
    // Use texture in determining fragment colour
    
}
