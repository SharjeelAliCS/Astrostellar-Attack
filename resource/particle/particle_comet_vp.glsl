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
out float particle_id;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.3, 0.8, 0.8);
float grav = 0.5; // Gravity
float speed = 30; // Allows to control the speed of the explosion
float upward = 1.0; // additional y velocity for all particles

void main()
{
	particle_id = color.r; // Derived from the particle color. We use the id to keep track of particles
    // Let time cycle every four seconds
	float duration = 40*t_d;

    //float circtime = timer - duration * floor(timer / duration);
    float t = mod(timer+particle_id*duration, duration)/duration;//circtime; // Our time parameter
    
    // Let's first work in model space (apply only world matrix)
    vec3 position = vertex;
    vec4 norm = normal_mat * vec4(normal, 1.0);

    // Move the comets particles away in the y direction, and make sure the x and z directions are slowly moving towards 0 to create the thinning effect in the back. 
    position.x -= min(normal.x*t*0.05,position.x);
    position.y -=t*speed*2;
    position.z -=min(normal.z*t*0.05,position.z);

    // Now apply view transformation
     gl_Position = view_mat * world_mat * vec4(position, 1.0);
        
	vertex_color = vec4(1-t,1-t*0.2, 1.0,1-t*5);
    timestep = 1-t;
}
