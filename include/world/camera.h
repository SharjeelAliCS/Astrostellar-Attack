/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a camera class
 */
#ifndef CAMERA_H_
#define CAMERA_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "orientation.h"

namespace game {

    // Abstraction of a camera
    class Camera {

        public:
            Camera(void);
            ~Camera();
 
			/*Input:       void
			  Output:      vec3 (position of camera)
			  Description: Returns the camera position
			 */
            glm::vec3 GetPosition(void) const;

			/*Input:       void
			  Output:      vec3 (zoom position)
			  Description: Returns the zoom position as a vector
			 */
			glm::vec3 GetZoomPos(void) const;
			
			/*Input:       None
			  Output:      vec3 (foward angle)
			  Description: Returns the foward angle
			*/
			glm::vec3 GetForward(void) const;

			/*Input:       None
			  Output:      vec3 (up angle)
			  Description: Returns the up angle
			*/
			glm::vec3 GetSide(void) const;

			/*Input:       None
			  Output:      vec3 (inital foward angle)
			  Description: Returns the inital foward angle
			*/
			glm::vec3 GetUp(void) const;

			/*Input:       None
			  Output:      mat4 (view matrix)
			  Description: Returns the view matrix
			*/
			glm::mat4 GetView(void) const;

			/*Input:       None
			  Output:      mat4 (projection matrix)
			  Description: Returns the projection matrix
			*/
			glm::mat4 GetProjection(void) const;

			/*Input:       None
			  Output:      quat (orientation)
			  Description: Returns the orienation
			  */
			glm::quat GetOrientation(void) const;

			/*Input:       None
			  Output:      float (far distance)
			  Description: Returns the far looking distance
			*/
			float GetFarDistance(void) const;

			/*Input:       None
			  Output:      float (aspect ratio)
			  Description: Returns the aspect ratio of the screen
			*/
			float GetAspectRatio(void) const;

			/*Input:       None
			  Output:      float (zoom)
			  Description: Returns the zoom amount of the camera
			*/
			float GetZoom(void) const;
			
			/*Input:       vec3 (position)
			  Output:      void
			  Description: Set the camera position
			*/
            void SetPosition(glm::vec3 position);

			/*Input:       quat (orientation)
			  Output:      void
			  Description: Set the camera orienation
			*/
            void SetOrientation(glm::quat orientation);

			/*Input:       position (vec3 position), look_at (look at axis), up (cross product facing up from look at angle)
			  Output:      void
			  Description: Set the view matrix for the camera
			*/
			void SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up);
		
			/*Input:       fov (fov of camera), near (near clipping plane), far (far clipping plane), w (width of screen), h (height of screen)
			  Output:      void
			  Description: Set the projection matrix of the camera
			*/
			void SetProjection(GLfloat fov, GLfloat near, GLfloat far, GLfloat w, GLfloat h);
			
			/*Input:       program (shader program)
			  Output:      void
			  Description: Set the camera shader
			*/
			void SetupShader(GLuint program);

			/*Input:       z (camera zoom)
			  Output:      void
			  Description: Set the camera zoom
			*/
			void SetZoom(float z);

			/*Input:       trans (vec3 translation amount)
			  Output:      void
			  Description: Translate the camera
			*/
            void Translate(glm::vec3 trans);

			/*Input:       rot (vec3 rotation amount)
			  Output:      void
			  Description: rotation the camera
			*/
            void Rotate(glm::quat rot);
            
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

			/*Input:       z (camera zoom amount)
			  Output:      void
			  Description: Zoom the camera by the zoom amount
			 */
			void Zoom(float z);

        private:
			float aspect_ratio_; //aspect ratio of screen
            glm::vec3 position_; // Position of camera
			glm::vec3 zoom_pos_; //zoom position (look at)
			float zoom_; //zoom amount

            Orientation* orientation_; // Orientation of camera
            glm::vec3 forward_; // Initial forward vector
            glm::vec3 side_; // Initial side vector
            glm::mat4 view_matrix_; // View matrix
            glm::mat4 projection_matrix_; // Projection matrix

			float far_distance_; //far clipping plane distance

			/*Input:       void
			  Output:      void
			  Description: Create view matrix from current camera parameters
			 */
            void SetupViewMatrix(void);

    }; // class Camera

} // namespace game

#endif // CAMERA_H_
