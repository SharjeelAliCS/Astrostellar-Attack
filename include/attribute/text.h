/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
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


//This class and text was taken from the following link. The vast, vast majority (about 90%) of
//code found in text.h and text.cpp is NOT MINE and belongs to the author of the link. 
//this means we intend to NOT get any marks for adding in text as this code is not ours
//for comp3501 and has a different author than us. 

//Tutorial found here: https://learnopengl.com/In-Practice/Text-Rendering
//Source code found here: https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/2.text_rendering/text_rendering.cpp
namespace game {

	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	struct Text {
		std::string text;
		glm::vec2 pos;
		float scale;
		glm::vec3 color;
		glm::vec3 glow;
		Text();
		Text(std::string t, glm::vec2 p, float s, glm::vec3 c);
	};
	class TextRenderer {

	public:
		TextRenderer(void);
		TextRenderer(std::string font, const Resource *material);
		~TextRenderer();

		void RenderText(Text* text);

	private:
		unsigned int VAO, VBO;
		std::string font_;

		int InitFont();
			
		std::map<char, Character> Characters;
		GLuint material_; // Reference to shader program
	}; // class Orientation


} // namespace game

#endif // CAMERA_H_
