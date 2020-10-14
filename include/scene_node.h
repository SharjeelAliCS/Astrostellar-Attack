#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "camera.h"
#include "rotation.h"
#include "transformation.h"
namespace game {

    // Class that manages one object in a scene 
    class SceneNode {

        public:
            // Create scene node from given resources
            SceneNode(const std::string name, const Resource *geometry, const Resource *material, Transformation* model);
			SceneNode(void);
            // Destructor
            ~SceneNode();
            
			// Get node attributes
			glm::vec3 GetOrgPos(void) const;
			glm::vec3 GetPosition(void) const;
			glm::quat GetOrientation(void) const;
			glm::vec3 GetScale(void) const;
			const std::string GetName(void) const;
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

            // OpenGL variables
            GLenum GetMode(void) const;
            GLuint GetArrayBuffer(void) const;
            GLuint GetElementArrayBuffer(void) const;
            GLsizei GetSize(void) const;
            GLuint GetMaterial(void) const;

        protected:
            std::string name_; // Name of the scene node
            GLuint array_buffer_; // References to geometry: vertex and array buffers
            GLuint element_array_buffer_;
            GLenum mode_; // Type of geometry
            GLsizei size_; // Number of primitives in geometry
            GLuint material_; // Reference to shader program
            glm::vec3 position_; // Position of node
			glm::vec3 org_pos_;
			Rotation orientation_; // Orientation of node
            glm::vec3 scale_; // Scale of node
			//movement speed over time
			float movement_speed;

			float rotation_speed_;
			glm::vec3 rotation_normal_;
			bool exists_;

            // Set matrices that transform the node in a shader program
            void SetupShader(GLuint program);
			void ApplyTransformation(GLuint program);

			//transformation model storing the stack. 
			Transformation* model_;

    }; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
