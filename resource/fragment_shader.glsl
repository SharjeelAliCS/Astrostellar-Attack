#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 tc;

void main()
{
    //gl_FragColor = texture(samp, tc);
    gl_FragColor = color_interp;
}
