/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a screen node class
 */

#ifndef SCREEN_NODE_H_
#define SCREEN_NODE_H_
#include "scene_node.h"

namespace game {

	// Class for a single game object 
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class ScreenNode : public SceneNode {

	public:
		// Create game object
		ScreenNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		
		// Destructor
		~ScreenNode();

		/*Input:       p (vec2 progress)
		  Output:      void
		  Description: Set the "progress" of a screen node HUD by filling it up using a specific percentage amount difined as a vec2 p (look in screen shader for more info). 
		*/
		void SetProgress(glm::vec2 p);

		/*Input:       p (x progress)
		  Output:      void
		  Description: Set the "horizontal progress" of a screen node HUD by filling it up using a specific percentage amount difined as a float p (look in screen shader for more info).
		*/
		void SetProgressX(float p);

		/*Input:       p (u progress)
		  Output:      void
		  Description: Set the "vertical progress" of a screen node HUD by filling it up using a specific percentage amount difined as a float p (look in screen shader for more info).
		*/
		void SetProgressY(float p);

		virtual void Draw(Camera *camera);
		virtual void Update(float deltaTime);
		virtual void SetupShader(GLuint program, Camera* camera);
		void Rotate(float angle);

		/*Input:       g (flag for glowing)
		  Output:      void
		  Description: Set if the screen object should "glow" (i.e. brighter) or not
		*/
		void SetGlow(bool g);

		glm::mat4 CalculateFinalTransformation(Camera* camera);

	protected:
		glm::vec3 glow_amount_; //glow amount (to brighten node with in shader)
		bool glow_ = false;
		glm::vec2 progress_size_; //the size of the progress. Defined as how much to remove when drawing in the shader using uv coordinates

		float rotation_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
