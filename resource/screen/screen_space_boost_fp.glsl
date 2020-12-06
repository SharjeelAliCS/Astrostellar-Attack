#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform float time_diff;
uniform sampler2D texture_map;
void main() 
{
	vec4 pixel;
	vec2 pos = uv0;
	vec2 center_pos =  pos*2-1;
	pixel = texture(texture_map,uv0);

	//SPATIAL PART OF THE EFFECT
	//use the radius and check if the uv coordiante is within that radius (the radius is a product of the time_diff uniform, which increases over time giving a more "distorted" effect for the screen space effect). 
	float radius_max = 1.5-time_diff;
	float radius_min = 1.4-time_diff;
	float radius = radius_max-radius_min;

	float scale_min = max(0, length(center_pos)-radius_min);
	float scale =min( scale_min/radius,1);

	//The offset is used to give it a "wavy" feel to make it seem that the player is hallucinating due to all the fumes and chemicals being released from the spaceship. 
	float offset  = 0.01*(sin(timer*5.0+8.0*pos.y));
    pos.x = pos.x + offset*scale;
	
	//LOCAL PART OF THE EFFECT
	//this part uses an array called tBlur to hold all the texture lookup values to create a bluring affect on the screen. In this case, I simply use four different texture lookups defined below and then get the average out of all of them. 
	const int num = 8;
	vec4 tBlur[num];
	float dist = 0.003*scale;
	tBlur[0] =texture(texture_map,pos+vec2(dist,0));//right side
	tBlur[1] =texture(texture_map,pos+vec2(0,dist));//top side
	tBlur[2] =texture(texture_map,pos+vec2(-dist,0));//left side
	tBlur[3] =texture(texture_map,pos+vec2(0,-dist));//bottom side
	tBlur[4] =texture(texture_map,pos+vec2(dist,dist));//top right side
	tBlur[5] =texture(texture_map,pos+vec2(-dist,dist));//top left side
	tBlur[6] =texture(texture_map,pos+vec2(-dist,-dist));//bottom left side
	tBlur[7] =texture(texture_map,pos+vec2(dist,-dist));//bottom right side
	
	//calculate the new pixel value. 
	pixel =pixel/(num+1);
	for(int i =0; i<num; i++)pixel += tBlur[i]/(num+1);

	//get the current average color for the pixel. 
	float g = (pixel.r+pixel.g+pixel.b)/3.0;

	//this creates the "green" tint affect, which also increases over time. 
	pixel.r = pixel.r+g*.2*scale_min/radius;
	pixel.g = pixel.g+g*0.5*scale_min/radius;
	pixel.b = pixel.b-g*.4*scale_min/radius;

    gl_FragColor = pixel;
}
