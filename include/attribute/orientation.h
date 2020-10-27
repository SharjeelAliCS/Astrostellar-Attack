/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef ORIENTATION_H_
#define ORIENTATION_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace game {

	// Abstraction of a Orientation. I created this seperate class simply because it was easier instead of copying/pasting the
	//orientation calculations for every object that needed them. 
	struct Orientation {

	public:
		Orientation(void);
		~Orientation();

		glm::quat GetOrientation(void) const;

		void SetOrientation(glm::quat orientation);

		void Rotate(glm::quat rot);

		// Get relative attributes of Orientation
		glm::vec3 GetForward(void) const;
		glm::vec3 GetSide(void) const;
		glm::vec3 GetUp(void) const;
		glm::vec3 GetInitForward(void) const;
		glm::vec3 GetInitSide(void) const;

		// Perform relative transformations of Orientation
		void Pitch(float angle);
		void Yaw(float angle);
		void Roll(float angle);

		void SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up);

	private:
		glm::vec3 forward_; // Initial forward vector
		glm::vec3 side_; // Initial side vector

		glm::quat orientation_; // Orientation of Orientation

	}; // class Orientation

} // namespace game

#endif // CAMERA_H_
