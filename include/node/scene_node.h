/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a scene node class
 */

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

		/*Input:       void
		  Output:      vec3 (starting position of object)
		  Description: Get the starting positon of an object
		*/
		glm::vec3 GetOrgPos(void) const;

		/*Input:       void
		  Output:      vec3 ( position of object)
		  Description: Get the positon of an object
		*/
		glm::vec3 GetPosition(void) const;

		/*Input:       void
		  Output:      quat (orientation of object)
		  Description: Get the orientation of an object
		*/
		glm::quat GetOrientation(void) const;

		/*Input:       void
		  Output:      orientation (orientation object)
		  Description: Get the orientation object for a node
		*/
		Orientation*  GetOrientationObj(void) const;

		/*Input:       void
		  Output:      vec3 (scale)
		  Description: Get the scale for a node
		*/
		glm::vec3 GetScale(void) const;

		/*Input:       void
		  Output:      bool (blending type)
		  Description: Get the blending type
		*/
		bool GetBlending(void) const;

		/*Input:       void
		  Output:      string (name of node)
		  Description: Get the name of a node
		*/
		const std::string GetName(void) const;

		/*Input:       void
		  Output:      float (speed of node)
		  Description: Get the current movement speed of a node
		*/
		float getCurSpeed(void) { return movement_speed; }
		
		/*Input:       void
		  Output:      bool (check if collision detected between obj and pos)
		  Description: Get the collision of an object with another position
		*/
		virtual bool Hit(glm::vec3 pos, float range);
		bool HitTorus(glm::vec3 pos, float range);

		/*Input:       position (postion of node), newOrgPos (flag to reset original position)
		  Output:      void
		  Description: Set the position of a node
		*/
		void SetPosition(glm::vec3 position, bool newOrgPos = false);

		/*Input:       position (original postion of node)
		  Output:      void
		  Description: Set the original position of a node
		*/
		void SetOrgPos(glm::vec3 pos);

		/*Input:       orientation (orientation)
		  Output:      void
		  Description: Set the orientation of a node
		*/
		void SetOrientation(glm::quat orientation);

		/*Input:       angle (angle of rotation), normal (axis of rotation)
		  Output:      void
		  Description: Set the orientaiton using an angle and normal
		*/
		void SetOrientation(float angle, glm::vec3 normal);

		/*Input:       scale (vec3 scale of node)
		  Output:      void
		  Description: Set the scale of a node
		*/
		void SetScale(glm::vec3 scale);

		/*Input:       joint (vec3 joint of node)
		  Output:      void
		  Description: Set the joint of a node for use in hierchial transformations
		*/
		void SetJoint(glm::vec3 joint);

		/*Input:       d (flag for drawing node or not)
		  Output:      void
		  Description: Set if the node should be drawn or not
		*/
		void SetDraw(bool d);

		/*Input:       texture (texture of node)
		  Output:      void
		  Description: Set the texture of a node
		*/
		void SetTexture(Resource* texture);
		
		/*Input:       trans (vec3 translation amount)
		  Output:      void
		  Description: translate a node
		*/
		void Translate(glm::vec3 trans);

		/*Input:       rot (quat rotation amount)
		  Output:      void
		  Description: rotate a node
		*/
		void Rotate(glm::quat rot);

		/*Input:       angle (angle of rotation), normal (axis of rotation)
		  Output:      void
		  Description: rotate a node by an angle and normal instead
		*/
		void Rotate(float angle, glm::vec3 normal);

		/*Input:       scale (scale amount)
		  Output:      void
		  Description: scale a node
		*/
		void Scale(glm::vec3 scale);

		/*Input:       scale (color)
		  Output:      void
		  Description: set the node's shader color
		*/
		void SetColor(glm::vec3 c);

		/*Input:       audio (audio object)
		  Output:      void
		  Description: sets the node's audio object
		*/
		void SetAudio(Audio* audio);

		/*Input:       blending (flag for blending)
		  Output:      void
		  Description: Set if a node should be blended or not)
		*/
		void SetBlending(bool blending);
		
		/*Input:       camera (camera)
		  Output:      void
		  Description: Draws a node
		*/
		virtual void Draw(Camera *camera);
		
		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: updates a node
		*/
		virtual void Update(float deltaTime);

		/*Input:       trans (transformation of parent)
		  Output:      void
		  Description: Set the parent transformation for a child to use for drawing.
		*/
		void SetParentTransform(glm::mat4 trans);
	
		/*Input:       parent (scene node)
		  Output:      void
		  Description: sets a parent to the scene node
		*/
		void AddParent(SceneNode* parent);
	
		/*Input:       child (child node to add)
		  Output:      void
		  Description: Add a child to the current list of children for the current node
		*/
		void AddChild(SceneNode* child);

		/*Input:       child (name of child node)
		  Output:      SceneNode (child node)
		  Description: Gets the child node by name
		*/
		SceneNode* GetChild(std::string child);

		/*Input:       e (flag for if object exists)
		  Output:      void
		  Description: sets the node's exist flag. if false, object will be deleted in next frame
		*/
		void SetExists(bool e) {exists_ = e;}

		/*Input:       void
		  Output:      bool (exists flag)
		  Description: get the node's exist flag to delete it or not
		*/
		bool GetExists(void) const { return exists_; }

		/*Input:       void
		  Output:      Orientation (original orientation object)
		  Description: Return the original orientation object of a given node
		*/
		Orientation* GetGeomOrientation() { return geom_orientation_; }

		/*Input:       abovePlayer (flag to get position above player), camera (camera)
		  Output:      vec3 (screen space positions)
		  Description: Convert the 3D world space into screen space for use in HUD
		*/
		glm::vec3 GetScreenSpacePos(bool abovePlayer,Camera* camera);

		/*Input:       void
		  Output:      NodeResources (current node resource)
		  Description: Return the node resource of the given node
		*/
		NodeResources* GetNodeResources(void) { return node_resources_; }

		/*Input:       n (noderesources to set)
		  Output:      void
		  Description: set the node's resources
		*/
		void SetNodeResources(NodeResources* n) { node_resources_ = n; }

		/*Input:       speed (speed of rotation), axis (axis of rotation)
		  Output:      void
		  Description: Initalize the angle of rotation and rotation speeds
		*/
		void RotateOrientationInit(float speed, glm::vec3 axis);

		/*Input:       void
		  Output:      vec3 (position of object)
		  Description: Caclulate the position of a node in world space, if it  has a parent
		*/
		glm::vec3 CalculateParentChildPos(void);


		// OpenGL variables
		GLenum GetMode(void) const;
		GLuint GetArrayBuffer(void) const;
		GLuint GetElementArrayBuffer(void) const;
		GLsizei GetSize(void) const;
		GLuint GetMaterial(void) const;

	protected:

		NodeResources* node_resources_;
		Audio* audio_;
		//Parent of a node
		SceneNode* parent_;
		//List of children for a node
		std::vector<SceneNode*> children_;
		//transformation which contains the entire multipled matrix for transformation
		glm::mat4 parentTransform_;
		glm::mat4 current_transform_;
		std::string name_; // Name of the scene node
		GLuint array_buffer_; // References to geometry: vertex and array buffers
		GLuint element_array_buffer_;
		GLenum mode_; // Type of geometry
		GLsizei size_; // Number of primitives in geometry
		GLuint material_; // Reference to shader program
		GLuint texture_; // Reference to texture resource
		GLuint normal_map_; // Reference to normal map resource

		glm::vec3 position_; // Position of node
		glm::vec3 org_pos_; //original starting position of node
		glm::vec3 joint_; //the joint to be used for rotating in hierchial transformations

		glm::vec3 color_;//color to be applied in shader program
		Orientation* orientation_; // Orientation of node
		Orientation* geom_orientation_; // Orientation of node
		glm::vec3 scale_; // Scale of node

		float movement_speed;//movement speed over time
		float rotation_speed_; //rotation speed
		glm::vec3 rotation_normal_; //axis of rotation
		float Orientation_speed_; 
		glm::vec3 Orientation_normal_;

		bool exists_; //if the node exists
		bool draw_; //if drawing the node
		bool blending_; // Draw with blending or not
		// Set matrices that transform the node in a shader program

		/*Input:       program (shader program), camera (camera to draw)
		  Output:      vec3 (position of object)
		  Description: Set up the shaders
		*/
		virtual void SetupShader(GLuint program, Camera* camera);

		/*Input:       void
		  Output:      void
		  Description: Setup blending for a node
		*/
		virtual void SetupBlending(void);

		/*Input:       camera (camera)
		  Output:      mat4 (final transformation position of an object)
		  Description: Transform an object 
		*/
		virtual  glm::mat4 CalculateFinalTransformation(Camera* camera);
		


	}; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
