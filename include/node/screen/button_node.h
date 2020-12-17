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
#include "text.h"

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
		void SetTextObj(TextRenderer* t);
		void SetText(Text* t);
		void SetupShader(GLuint program, Camera* camera);
		bool WithinButton(float x, float y);
		void UpdateScreenSize(float x, float y);
		glm::mat4 CalculateFinalTransformation(Camera* camera);
		
		Text* GetText(int i = 0);
		void SetType(std::string t) { type_ = t; }
		std::string GetType(void) { return type_; }

	private:
		TextRenderer* TextObj_;
		std::vector<Text*> text_;
		float screen_height_;
		float screen_width_;
		std::string type_;

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
