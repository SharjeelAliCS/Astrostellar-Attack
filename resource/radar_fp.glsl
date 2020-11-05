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
uniform vec2 dots[100];
void main() 
{
    // Retrieve texture value
	vec4 pixel = texture(texture_map, uv_interp);
	gl_FragColor = pixel;
	for(int i =0; i<arr_size; i++){
		float x = abs(dots[i].x - uv_interp.x);
		float y = abs(dots[i].y - uv_interp.y);
		float dis = distance(dots[i], uv_interp);
		if(dis <0.03){
			gl_FragColor.r = 1;//distance(dots[0], uv_interp);
			gl_FragColor.g =0;//distance(dots[1], uv_interp);
			gl_FragColor.b =0;//distance(dots[2], uv_interp);
		}
	}
    // Use texture in determining fragment colour
    
}
