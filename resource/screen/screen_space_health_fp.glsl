#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D texture_map;
uniform float healthPercent;

void main() 
{
	vec4 pixel;
	vec2 pos = uv0;
	pixel = texture(texture_map,uv0);
	

	//gets the distance from the center of the screen, -0.1 so the center isn't effected by the redshift
	float distFromCenter = sqrt(pow(uv0.x-0.5, 2) + pow(uv0.y-0.5, 2));
	float d = distFromCenter - 0.1;
	float hazeIntensity = 0.5*(1-healthPercent)*d;//1.65
	//required texture look up effect. Honestly I don't think this adds much to the effect but it was a requirement so here we are.
	vec4 offx = texture(texture_map,uv0+vec2(0.002,0));
	vec4 offy = texture(texture_map,uv0+vec2(0,0.002));
	vec4 diff = abs(pixel-offx)+abs(pixel-offy);

	//add the haze Intensity to the red attribute
	pixel = vec4(pixel.r+hazeIntensity, pixel.g,pixel.b,1);
	//apply the haze/blurring
	pixel = pixel + 1.1*hazeIntensity*diff;
	

    gl_FragColor = pixel;
}
