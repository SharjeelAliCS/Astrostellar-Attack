#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in vec3 uv;
in float t_d;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;

// Attributes forwarded to the geometry shader
out vec4 vertex_color;
out float timestep;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.3, 0.8, 0.8);
float grav = 0.5; // Gravity
float speed = 20; // Allows to control the speed of the explosion
float upward = 1.0; // additional y velocity for all particles

void main()
{
    // Let time cycle every four seconds
	float duration = 1.5*t_d;
	timer;
    float circtime = timer - duration * floor(timer / duration);
    float t = circtime; // Our time parameter
    
    // Let's first work in model space (apply only world matrix)
    vec4 position =  world_mat *vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 1.0);

    // Move point along normal and down with t*t (acceleration under gravity)
    position.x += norm.x*t*speed;
    position.y -= norm.y*t*speed*0.5;
    position.z += norm.z*t*speed;
    
    // Now apply view transformation
    gl_Position = view_mat *position;
        
    // Define outputs
    // Define color of vertex
    //vertex_color = color.rgb; // Color defined during the construction of the particles
    //vertex_color = object_color; // Uniform color 
	//vertex_color = vec3(t_d);
    //vertex_color = vec3(t, 0.0, 1-t); // red-purple dynamic color
    //vertex_color = vec3(1-t, 0.4+(t)*0.4, t); // red-yellow dynamic color
	vertex_color = vec4(1-t,1-t*0.2, 1.0,1-t);
    // Forward time step to geometry shader
    timestep = t;
}
