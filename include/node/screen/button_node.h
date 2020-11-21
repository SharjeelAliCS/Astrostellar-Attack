/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 2: A multi-component model
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates camera model with the tori.
 * For further info, please read the README file provided.
 *
 */

#ifndef BUTTON_NODE_H_
#define BUTTON_NODE_H_
#include "screen_node.h"

namespace game {

	// Class for a single game object 
	class ButtonNode : public ScreenNode {

	public:
		// Create game object
		ButtonNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);

		// Destructor
		~ButtonNode();
		void Draw(Camera *camera);
		void Update(float deltaTime);
		void SetupShader(GLuint program, Camera* camera);
		bool Clicked(float x, float y);
		void UpdateScreenSize(float x, float y);
		glm::mat4 CalculateFinalTransformation(Camera* camera);
		

	private:
		float screen_height_;
		float screen_width_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
