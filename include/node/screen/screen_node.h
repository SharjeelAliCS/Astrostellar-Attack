/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 2: A multi-component model
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates camera model with the tori.
 * For further info, please read the README file provided.
 *
 */

#ifndef SCREEN_NODE_H_
#define SCREEN_NODE_H_
#include "scene_node.h"

namespace game {

	// Class for a single game object 
	class ScreenNode : public SceneNode {

	public:
		// Create game object
		ScreenNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);
		
		// Destructor
		~ScreenNode();
		void SetProgress(float p);

		void Draw(Camera *camera);
		void Update(float deltaTime);
		void SetupShader(GLuint program, Camera* camera);

		glm::mat4 CalculateFinalTransformation(Camera* camera);

	private:
		float progress_size_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
