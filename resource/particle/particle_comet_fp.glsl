#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;

// Uniform (global) buffer
uniform sampler2D tex_samp;
// Simulation parameters (constants)
uniform vec3 color = vec3(0.8, 0.4, 0.03);
void main (void)
{
   // Get pixel from texture
    vec4 outval = texture(tex_samp, tex_coord);
    // Adjust specified object color according to the grayscale texture value
    outval = vec4(outval.r*color.r, outval.g*color.g, outval.b*color.b, frag_color.a*0.5);
    // Set output fragment color
    gl_FragColor = outval;
}
