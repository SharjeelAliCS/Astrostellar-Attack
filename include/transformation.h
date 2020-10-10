/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_


#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <stdexcept>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <stack>


// Stores the transformation stack for applying transformations for objects. 
//The transformation stack is a heirachy of transformation matricies, in that the user has the option to push new matricies to the head of the stack, in which case it will insert the current head of the stack back into the transformation hiearchy and apply a transformation onto it. The user also has the option of popping the stack's head, removing the current transformation. This allows the user full freedom and control over the stack in that they can decide which transformations to apply in which hiearchy and where. 
class Transformation {

public:
	Transformation();
	~Transformation();

	//Translates the transformation stack by the input 3D vector. 
	// trans the 3D vector to translate by. 
	// new_matrix a boolean indicating whether to apply the vector to the current head of the stack, or push a new one. 
	bool Translate(glm::vec3 trans, bool new_matrix = false);

	//Rotates the transformation top by taking in a rotation scale and normal. This will actually create the quaternian from scratch. 
	bool Rotate(float scale, glm::vec3 normal, bool new_matrix = false);

	//Applies the quarternion to the top of the stack. 
	bool Rotate(glm::quat q,bool new_matrix = false);

	//Scales the transformation stack by the input 3D vector. 
	// trans the 3D vector to scale by. 
	// new_matrix a boolean indicating whether to apply the vector to the current head of the stack, or push a new one. 
	bool Scale(glm::vec3 scale, bool new_matrix = false);

	//Applies the given transformation by returning the top of the stack. 
	glm::mat4 Apply(bool pop_matrix = false);

	//Pops the head of the stack
	bool PopMatrix();

	//Pushes the head of the stack back into the stack acting as a "new" head. 
	bool PushMatrix();

	//Pushes a basic identity matrix used as the stack's "root"
	bool PushIdentityMatrix();

	//Returns the head of the stack. 
	glm::mat4 SaveMatrix();

private:

	//The transformation stack
	std::stack<glm::mat4> transformations;


}; // class Transformations


#endif // TRANSFORMATION_H_
