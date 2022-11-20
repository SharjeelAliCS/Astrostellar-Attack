/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains an Orientation class 

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

		/*Input:       void
		  Output:      quat (orientation)
		  Description: Returns the orientation quat
		*/
		glm::quat GetOrientation(void) const;

		/*Input:       quat (orientation)
		  Output:      void
		  Description: Sets the orientation angle
		*/
		void SetOrientation(glm::quat orientation);

		/*Input:       quat (rotation to rotate)
		  Output:      void
		  Description: Rotates the quaternion by a given angle
		*/
		void Rotate(glm::quat rot); 

		/*Input:       angle (angle of rotation), vec3(axis of rotation)
		  Output:      void
		  Description: Rotates the quaternion given angle and axis
		*/
		void Rotate(float angle, glm::vec3 normal);

		/*Input:       None
		  Output:      vec3 (foward angle)
		  Description: Returns the foward angle
		*/
		glm::vec3 GetForward(void) const;

		/*Input:       None
		  Output:      vec3 (side angle)
		  Description: Returns the side angle
		*/
		glm::vec3 GetSide(void) const;

		/*Input:       None
		  Output:      vec3 (up angle)
		  Description: Returns the up angle
		*/
		glm::vec3 GetUp(void) const;

		/*Input:       None
		  Output:      vec3 (inital foward angle)
		  Description: Returns the inital foward angle
		*/
		glm::vec3 GetInitForward(void) const;

		/*Input:       None
		  Output:      vec3 (inital side angle)
		  Description: Returns the inital side angle
		*/
		glm::vec3 GetInitSide(void) const;

		
		/*Input:       angle (angle of rotation)
		  Output:      void
		  Description: Rotate the pitch axis
		*/
		void Pitch(float angle);

		/*Input:       angle (angle of rotation)
		  Output:      void
		  Description: Rotate the Yaw axis
		*/
		void Yaw(float angle);

		/*Input:       angle (angle of rotation)
		  Output:      void
		  Description: Rotate the Roll axis
		*/
		void Roll(float angle);

		/*Input:       position (vec3 position), look_at (look at axis), up (cross product facing up from look at angle)
		  Output:      void
		  Description: Set the view matrix for the camera
		*/
		void SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up);

		/*Input:       cur_pos (current object position), target_pos (target position), rotate (should rotate over time or instantly)
		  Output:      void
		  Description: Faces the object towards a given position by rotating it. 
		  Source:      https://gamedev.net/forums/topic/493428-rotate-an-object-to-face-a-target-in-3d-space/4218536/
		*/
		void FaceTowards(glm::vec3 cur_pos, glm::vec3 target_pos, bool rotate=false);

		/*Input:       speed (speed of rotation)
		  Output:      void
		  Description: Rotate the object towards the axis calculated in faceTowards until the object faces the angle. 
		*/
		void RotateTowards(float speed);

		/*Input:       speed (speed of rotation), axis (axis of rotation)
		  Output:      void
		  Description: Initalize the angle of rotation and rotation speeds
		*/
		void RotateOverTimeInit(float speed, glm::vec3 axis);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: Rotate the object using the axis and speed calculated in RotateOverTimeInit above. 
		*/
		void RotateOverTime(float deltatime);
	private:
		glm::vec3 forward_; // Initial forward vector
		glm::vec3 side_; // Initial side vector

		glm::quat orientation_; // Orientation of Orientation
		glm::vec3 rotate_axis_; //axis of rotation
		float rotate_cur_angle_; //current rotation angle
		float rotate_max_angle_; //maximum rotation angle 
		float rotate_speed_; //speed of rotation

	}; // class Orientation

} // namespace game

#endif // CAMERA_H_
