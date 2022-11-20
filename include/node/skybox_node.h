/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a skybox node class
 */

#ifndef SKYBOX_NODE_H_
#define SKYBOX_NODE_H_
#include "scene_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class.

	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class SkyBox : public SceneNode {

	public:
		// Create game object
		SkyBox(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~SkyBox();

		
		void Draw(Camera *camera);
		glm::mat4 CalculateFinalTransformation(Camera* camera);
		void SetupShader(GLuint program, Camera* camera);


	private:
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
