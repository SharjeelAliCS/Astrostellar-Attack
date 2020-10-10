/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef GAME_OBJ_H_
#define GAME_OBJ_H_

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
#include <geometry.h>

#include <map>
namespace game {

    // Class for a single game object 
    class GameObj {

        public:
            // Create game object
            GameObj(GLuint shader, Transformation* m);
			GameObj(GLuint shader, Transformation* m, glm::vec3 forward);
			GameObj();

			//Create any of the following shapes
			void CreateTorus(float loopsize, float rad, int loopsamples, int circlesamples);
			void CreateCylinder(float h, float rad, int heightsamples, int circlesamples);
			void CreateCube(float w, float h,float d);

            // Destructor
            ~GameObj();
            
            // Get node attributes
			glm::vec3 GetOrgPos(void) const;
            glm::vec3 GetPosition(void) const;
            glm::quat GetOrientation(void) const;
            glm::vec3 GetScale(void) const;
			std::string GetName(void) const;
			bool Exists(void) const;

			// Get relative attributes of the orientation
			glm::vec3 GetForward(void) const;
			glm::vec3 GetSide(void) const;
			glm::vec3 GetUp(void) const;

			// Perform relative transformations of orientation
			void Pitch(float angle);
			void Yaw(float angle);
			void Roll(float angle);

            // Set node attributes
			void SetOrgPos(glm::vec3 position);
            void SetPosition(glm::vec3 position);
            void SetOrientation(glm::quat orientation);
			void SetOrientation(float angle, glm::vec3 normal);
            void SetScale(glm::vec3 scale);

			void SetName(std::string name);
			void SetMovementSpeed(float s);

            // Perform transformations on node
            void Translate(glm::vec3 trans);
            void Rotate(glm::quat rot);
            void Scale(glm::vec3 scale);

			//Rotate over time simply rotates over the given norma axis per update iteration. 
			void RotateOverTime(float rotation_speed, glm::vec3 rotation_normal);

            // Draw the node according to scene parameters in 'camera'
            // variable
            void Draw(Camera *camera);

            // Update the node
            void Update(double deltaTime);

			//Check if the object has been hit using a pos and a range.
			bool Hit(glm::vec3 pos, float range);
         
        
        protected:

			//Use the geom structure, which stores the actual vbo/ebo of the object. This is mainly used for creatin additional pieces of geometry (such as the ringed planets or spaceship) efficently without making multiple game objects. 
			std::map<std::string, Geom*> geom;
			
			//movement speed over time
			float movement_speed;

            std::string name_; // Name of the scene node

			glm::vec3 org_pos_;
            glm::vec3 position_; // Position of node
            Rotation orientation_; // Orientation of node
            glm::vec3 scale_; // Scale of node

			float rotation_speed_;
			glm::vec3 rotation_normal_;

			//Does the object exist?
			bool exists_;

		//	GLuint size_; // number of primitives to draw
			GLuint program; // shader program to use for drawing

			//Taken from textbook, page 105

			//transformation model storing the stack. 
			Transformation* model;
			
    }; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
