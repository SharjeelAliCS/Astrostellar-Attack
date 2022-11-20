/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913,LearnOpenGL 
CONTENTS:              Contains a text rendering class, and text structure

 */

#ifndef TEXT_H_
#define TEXT_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include <map>
#include "resource.h"
#include "path_config.h"
#include FT_FREETYPE_H  
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/*The classes listed here were not created by us, but instead by the website LearnOpenGL. About 90% of the code here was taken from their site, with the rest being created by us. 

Sources:
	https://learnopengl.com/In-Practice/Text-Rendering
	https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/2.text_rendering/text_rendering.cpp
*/

namespace game {

	//Taken from the LearnOpenGL source above
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	//This text struct was created by us
	struct Text {
		std::string text;
		glm::vec2 pos;
		float scale;
		glm::vec3 color;
		glm::vec3 glow;
		Text();
		Text(std::string t, glm::vec2 p, float s, glm::vec3 c);
	};

	//Main code taken from LearnOpenGL linked above
	class TextRenderer {

	public:

		TextRenderer(void);
		TextRenderer(std::string font, const Resource *material);
		~TextRenderer();

		/*Input:       text (text struct to render)
		  Output:      void
		  Description: Renders a text struct
		*/
		void RenderText(Text* text);

	private:
		unsigned int VAO, VBO;//VAO, VBO to render
		std::string font_; //font to use

		/*Input:       void
		  Output:      int (successful init or not)
		  Description: Initalizes the font
		*/
		int InitFont();
			
		std::map<char, Character> Characters; //map of common english language characters

		GLuint material_; // Reference to shader program
	}; // class Orientation


} // namespace game

#endif // CAMERA_H_
