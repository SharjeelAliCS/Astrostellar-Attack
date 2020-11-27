#version 130

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform mat4 normal_mat;

// Attributes forwarded to the fragment shader
out vec2 uv_interp;

// Material attributes (constants)
uniform vec3 light_position = vec3(-0.5, -0.5, 1.5);


void main()
{
    gl_Position = projection_mat * view_mat *world_mat * vec4(vertex, 1.0);
    uv_interp = uv;
}
