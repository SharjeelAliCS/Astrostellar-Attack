#version 400

// Definition of the geometry shader
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// Attributes passed from the vertex shader
in vec4 vertex_color[];
in float timestep[];
in float particle_id[];

// Uniform (global) buffer
uniform mat4 projection_mat;

// Simulation parameters (constants)
uniform float particle_size = 5;

// Attributes passed to the fragment shader
out vec4 frag_color;
out vec2 tex_coord;
// add any other attributes you want here

void main(void){

    // Get the position of the particle
    vec4 position = gl_in[0].gl_Position;

    // Define particle size
    float p_size = particle_size*timestep[0]; //

    // Define the positions of the four vertices that will form a quad 
    // The positions are based on the position of the particle and its size
    // We simply add offsets to the position (we can think of it as the center of the particle),
    // since we are already in camera space
    vec4 v[4];
    v[0] = vec4(position.x - 0.5*p_size, position.y - 0.5*p_size, position.z, 1.0);
    v[1] = vec4(position.x + 0.5*p_size, position.y - 0.5*p_size, position.z, 1.0);
    v[2] = vec4(position.x - 0.5*p_size, position.y + 0.5*p_size, position.z, 1.0);
    v[3] = vec4(position.x + 0.5*p_size, position.y + 0.5*p_size, position.z, 1.0);
	
    // Create the new geometry: a quad with four vertices from the vector v
    int fid = int(floor(particle_id[0] * 4.0)); // 0-3 used to pick sector from flame 2x2 drawing
    for (int i = 0; i < 4; i++){
		tex_coord = vec2(floor(i / 2)*0.5 + 0.5*(fid / 2), (i % 2)*0.5 + 0.5*(fid % 2));
        gl_Position = projection_mat * v[i];
        frag_color = vertex_color[0];//vec4(vertex_color[0], 1.0);
        EmitVertex();
     }

     EndPrimitive();
}
