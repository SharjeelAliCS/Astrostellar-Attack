/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef GEOM_H_
#define GEOM_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <transformation.h>
#include "camera.h"
#include <set>
#include <SOIL/SOIL.h> // read image file
#include <math.h>
#include <rotation.h>

namespace game {

	// This piece of geometry is very important. It's basically handles all of the basic geometry shapes shown below. 
	//and handles the actual drawing of them as well. 
	class Geom {

	public:
		// Create game object
		Geom(GLuint shader, Transformation* m);
		Geom();
		void CreateTorus(float loopsize, float rad, int loopsamples, int circlesamples);
		void CreateCylinder(float h, float rad, int heightsamples, int circlesamples);
		
		//The cube is my own work. 
		void CreateCube(float w, float h, float d);

		//I used the sphere from the new scenedemo code. 
		void CreateSphere(float radius = 0.6, int num_samples_theta = 90, int num_samples_phi = 45);


		// Destructor
		~Geom();

		// Get node attributes
		glm::vec3 GetPosition(void) const;
		glm::quat GetOrientation(void) const;
		glm::vec3 GetScale(void) const;
		// Set node attributes

		void SetPosition(glm::vec3 position);
		void SetOrientation(glm::quat orientation);
		void SetOrientation(float angle, glm::vec3 normal);
		void SetScale(glm::vec3 scale);
		// Perform transformations on node
		void RotateOverTime(float rotation_speed, glm::vec3 rotation_normal);
		// Draw the node according to scene parameters in 'camera'
		// variable
		void Draw(Camera *camera);
		void Update(double deltaTime);
		// OpenGL variables

		GLuint GetArrayBuffer(void) const;
		GLuint GetElementArrayBuffer(void) const;
		GLsizei GetSize(void) const;


	protected:
		float movement_speed;

		std::string name_; // Name of the scene node
		GLuint array_buffer_; // References to geometry: vertex and array buffers
		GLuint element_array_buffer_;

		GLsizei size_; // Number of primitives in geometry

		glm::vec3 scale_;
		glm::vec3 position_; // Position of node
		Rotation orientation_; // Orientation of node
		
		float rotation_speed_;
		glm::vec3 rotation_normal_;

		GLuint vbo_; // ebo for drawing
		GLuint ebo_; // vbo for drawing
	//	GLuint size_; // number of primitives to draw
		GLuint program; // shader program to use for drawing

		// Set matrices that transform the node in a shader program
		void SetShaderAttributes(GLuint program);

		//Taken from textbook, page 105

		//transformation model storing the stack. 
		Transformation* model;

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
