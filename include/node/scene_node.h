#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "camera.h"
#include "orientation.h"
#include "sound.h"
namespace game {

	// Class that manages one object in a scene 
	class SceneNode {

	public:
		// Create scene node from given resources
		SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		// Destructor
		~SceneNode();

		// Get node attributes
		glm::vec3 GetOrgPos(void) const;
		glm::vec3 GetPosition(void) const;
		glm::quat GetOrientation(void) const;
		Orientation*  GetOrientationObj(void) const;
		glm::vec3 GetScale(void) const;
		bool GetBlending(void) const;
		const std::string GetName(void) const;

		float getCurSpeed(void) { return movement_speed; }
		
		//Check if the object has been hit using a pos and a range.
		bool Hit(glm::vec3 pos, float range);
		bool HitTorus(glm::vec3 pos, float range);

		// Set node attributes
		void SetPosition(glm::vec3 position, bool newOrgPos = false);
		void SetOrgPos(glm::vec3 pos);
		void SetOrientation(glm::quat orientation);
		void SetOrientation(float angle, glm::vec3 normal);
		void SetScale(glm::vec3 scale);
		void SetJoint(glm::vec3 joint);
		void SetDraw(bool d);
		void SetTexture(Resource* texture);
		
		// Perform transformations on node
		void Translate(glm::vec3 trans);
		void Rotate(glm::quat rot);
		void Rotate(float angle, glm::vec3 normal);
		void Scale(glm::vec3 scale);
		void SetColor(glm::vec3 c);
		void SetAudio(Audio* audio);
		void SetBlending(bool blending);
		// Draw the node according to scene parameters in 'camera'
		// variable
		virtual void Draw(Camera *camera);
		
		// Update the node
		virtual void Update(float deltaTime);

		//Set the parent transformation for a child to use for drawing.
		void SetParentTransform(glm::mat4 trans);
		//Add a parent to the current node
		void AddParent(SceneNode* parent);
		//Add a child to the current list of children for the current node

		void AddChild(SceneNode* child);
		SceneNode* GetChild(std::string child);
		// OpenGL variables
		GLenum GetMode(void) const;
		GLuint GetArrayBuffer(void) const;
		GLuint GetElementArrayBuffer(void) const;
		GLsizei GetSize(void) const;
		GLuint GetMaterial(void) const;

	protected:
		Audio* audio_;
		//Parent of a node
		SceneNode* parent_;
		//List of children for a node
		std::vector<SceneNode*> children_;
		//transformation which contains the entire multipled matrix for transformation
		glm::mat4 parentTransform_;

		std::string name_; // Name of the scene node
		GLuint array_buffer_; // References to geometry: vertex and array buffers
		GLuint element_array_buffer_;
		GLenum mode_; // Type of geometry
		GLsizei size_; // Number of primitives in geometry
		GLuint material_; // Reference to shader program
		GLuint texture_; // Reference to texture resource
		GLuint normal_map_; // Reference to normal map resource

		glm::vec3 position_; // Position of node
		glm::vec3 org_pos_;
		glm::vec3 joint_;
		glm::vec3 color_;
		Orientation* orientation_; // Orientation of node
		glm::vec3 scale_; // Scale of node

		float movement_speed;//movement speed over time
		float rotation_speed_;
		glm::vec3 rotation_normal_;
		float Orientation_speed_;
		glm::vec3 Orientation_normal_;

		bool exists_;
		bool draw_;
		bool blending_; // Draw with blending or not
		// Set matrices that transform the node in a shader program
		virtual void SetupShader(GLuint program, Camera* camera);

		virtual void SetupBlending(void);
		virtual  glm::mat4 CalculateFinalTransformation(Camera* camera);

	}; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
