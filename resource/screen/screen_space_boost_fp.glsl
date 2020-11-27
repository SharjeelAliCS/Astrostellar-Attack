#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D texture_map;

void main() 
{
vec4 pixel;
vec2 pos = uv0;
pixel = texture(texture_map,uv0);
	

	if (uv0.x > 0.8 && uv0.y > 0.5)
	{
    // wavering
    vec2 pos = uv0;
	float offset  = 0.05*(sin(timer*5.0+8.0*pos.y));
    pos.x = pos.x + offset;
	
	
    pixel = texture(texture_map, pos) ;
    }
	
	if (uv0.x > 0.5 && uv0.y < 0.5)
	{
	// grey
	float g = (pixel.r+pixel.g+pixel.b)/3.0;
	pixel = vec4(g*.6,g,g*0.7,1);
	}
	
	if (uv0.x < 0.5 && uv0.y > 0.5)
	{
	// edges
	vec4 offx = texture(texture_map,uv0+vec2(0.005,0));
	vec4 offy = texture(texture_map,uv0+vec2(0,0.005));
	vec4 diff = abs(pixel-offx)+abs(pixel-offy);
	float edginess = diff.r+diff.g+diff.b;
	if (edginess > 0.2)
		pixel = vec4(0.8,0.9,1.0,1.0);
	else
		pixel = vec4(0,0,0,1);
	}
	
	if (uv0.x < 0.5 && uv0.y < 0.5)
	{
	// tv blockiness
	float spacing = 0.01;
	float thick = 0.005;
	if ((mod(uv0.y, spacing) < thick) || (mod(uv0.x,spacing) < thick))
		pixel = vec4(0,0,0,1);
	}
	
    gl_FragColor = pixel;
}
