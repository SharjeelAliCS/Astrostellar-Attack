#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D texture_map;
uniform float start_time;
void main() 
{
	vec4 pixel;
	vec2 pos = uv0;
	pixel = texture(texture_map,uv0);

	//the bloom is defined as the max value of each pixel sampled using the texture lookup multipled by its weight. In this case, the same reasoning is being applied. 

	//same as boost fp. In this case, I simply reused the code from there for texture lookups to create the blur. 
	const int num = 8;
	vec4 tBlur[num];
	float dist = 0.003;//set the distance paramater. 

	//Get the overal blur using eight textures lookups:
	tBlur[0] =texture(texture_map,pos+vec2(dist,0));//right side
	tBlur[1] =texture(texture_map,pos+vec2(0,dist));//top side
	tBlur[2] =texture(texture_map,pos+vec2(-dist,0));//left side
	tBlur[3] =texture(texture_map,pos+vec2(0,-dist));//bottom side
	tBlur[4] =texture(texture_map,pos+vec2(dist,dist));//top right side
	tBlur[5] =texture(texture_map,pos+vec2(-dist,dist));//top left side
	tBlur[6] =texture(texture_map,pos+vec2(-dist,-dist));//bottom left side
	tBlur[7] =texture(texture_map,pos+vec2(dist,-dist));//bottom right side
	
	vec4 pix = pixel/(num+1);//get the current pixel multipled by the weight (1/num+1)

	float max_brightness = (pix.r+pix.g+pix.b)/3;//get the maximum brightness of the pixel. 

	//check if the pixel's brightness is larger than the current largest one. if so, save it. 
	for(int i =0; i<num; i++){
		tBlur[i] = tBlur[i]/(num+1);
		float brightness = (tBlur[i].r+tBlur[i].g+tBlur[i].b)/3;
		if(brightness>max_brightness){
			max_brightness = brightness;//save the current pixel's brightness
			pix = tBlur[i];//save the current pixel 
		}
	}
	//use additive blending to add the brightest pixel to the current pixel. 
	pixel += pix*1.3;

    gl_FragColor = pixel;
}
