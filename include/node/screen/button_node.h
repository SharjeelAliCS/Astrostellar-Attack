/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a button node class
 */

#ifndef BUTTON_NODE_H_
#define BUTTON_NODE_H_
#include "screen_node.h"
#include "text.h"

namespace game {

	// Class for a single game object 
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class ButtonNode : public ScreenNode {

	public:
		// Create game object
		ButtonNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);

		// Destructor
		~ButtonNode();
		void Draw(Camera *camera);
		void Update(float deltaTime);

		/*Input:       t (text renderer object)
		  Output:      void
		  Description: set the text rendering object for a button
		*/
		void SetTextObj(TextRenderer* t);

		/*Input:       t (text to add)
		  Output:      void
		  Description: Add a text struct to the list to render with the button
		*/
		void SetText(Text* t);
		void SetupShader(GLuint program, Camera* camera);

		/*Input:       x (x mouse pos), y (y mouse pos)
		  Output:      bool (flag for if within button or not)
		  Description: Check if player clicked within the button
		*/

		bool WithinButton(float x, float y);

		/*Input:       x (screen width), y (screen height)
		  Output:      void
		  Description: Update the screen size
		*/
		void UpdateScreenSize(float x, float y);
		glm::mat4 CalculateFinalTransformation(Camera* camera);
		
		/*Input:       i (index of text)
		  Output:      Text* (Get the text from index)
		  Description: Get the text using the index
		*/

		Text* GetText(int i = 0);

		/*Input:       t (type of button)
		  Output:      void
		  Description: Set the button type
		*/
		void SetType(std::string t) { type_ = t; }

		/*Input:       void
		  Output:      string (type of button)
		  Description: Get the button type
		*/
		std::string GetType(void) { return type_; }

	private:
		TextRenderer* TextObj_; //text rendering object
		std::vector<Text*> text_; //text list
		float screen_height_; //height of screen
		float screen_width_;//width of screen
		std::string type_;//type of button

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
