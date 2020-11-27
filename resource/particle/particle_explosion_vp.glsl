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
uniform float start_time;
uniform float duration = 0.5;

// Attributes forwarded to the geometry shader
out vec4 vertex_color;
out float timestep;
out float particle_id;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.3, 0.8, 0.8);
float grav = 0.5; // Gravity
float speed = 10; // Allows to control the speed of the explosion
float upward = 1.0; // additional y velocity for all particles

void main()
{
	particle_id = color.r; // Derived from the particle color. We use the id to keep track of particles
    // Let time cycle every four seconds
    //float circtime = timer - duration * floor(timer / duration);
    //float t = circtime/duration; // Our time parameter
    float t= mod(timer-start_time, duration)/duration;
	vec3 vert = vec3(0);
    // Let's first work in model space (apply only world matrix)
    vec4 position = vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 1.0);

    // Move point along normal and down with t*t (acceleration under gravity)

    position.x += normal.x*t*speed;
    position.y += normal.y*t*speed;
    position.z += normal.z*t*speed;
    
    // Now apply view transformation
    gl_Position = view_mat * world_mat *position;
        
	vertex_color = vec4(1-t,1-t*0.2, 1.0,1-t);
    // Forward time step to geometry shader
    timestep = t;
}
