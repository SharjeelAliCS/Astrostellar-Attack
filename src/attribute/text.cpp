#include <iostream>
#include <time.h>
#include <sstream>

#include "text.h"

namespace game {

	Text::Text() : text(""), pos(glm::vec2(0)), scale(0), color(glm::vec3(0)), glow(glm::vec3(0)) {}
	Text::Text(std::string t, glm::vec2 p, float s, glm::vec3 c) : text(t), pos(p), scale(s), color(c), glow(glm::vec3(0)) {}

	TextRenderer::TextRenderer(void) {
	}
	TextRenderer::TextRenderer(std::string font, const Resource *material) {
		// Set material (shader program)
		if (material->GetType() != Material) {
			throw(std::invalid_argument(std::string("Invalid type of material")));
		}

		material_ = material->GetResource();

		font_ = font;
		InitFont();
		//GenerateTextures();

		// Don't do work in the constructor, leave it for the Init() function
	}
	TextRenderer::~TextRenderer() {

	}
	
	int TextRenderer::InitFont() {
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return -1;
		}

		// find path to font
		//std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");
		if (font_.empty())
		{
			std::cout << "ERROR::FREETYPE: Failed to load font from " <<font_ << std::endl;
			return -1;
		}

		// load font as face
		FT_Face face;
		if (FT_New_Face(ft, font_.c_str(), 0, &face)) {
			std::cout << "ERROR::FREETYPE: Failed to load font from " << font_ <<std::endl;
			return -1;
		}
		else {
			// set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, 48);

			// disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// load first 128 characters of ASCII set
			for (unsigned char c = 0; c < 128; c++)
			{
				// Load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);


		// configure VAO/VBO for texture quads
		// -----------------------------------
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	void TextRenderer::RenderText(Text* text) {
		
		glm::vec2 pos = text->pos;
		std::string text_string = text->text;
		float scale = text->scale;
		glm::vec3 color = text->color;
		pos.x = (pos.x/2 + 0.5)*800;
		pos.y = (pos.y / 2 + 0.5)*450;

		glUseProgram(material_);
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(450));
		glUniformMatrix4fv(glGetUniformLocation(material_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUniform3f(glGetUniformLocation(material_, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);


		GLint glow = glGetUniformLocation(material_, "glow");
		glUniform3f(glow,text->glow.r, text->glow.g, text->glow.b);

			// iterate through all characters
			std::string::const_iterator c;
			for (c = text_string.begin(); c != text_string.end(); c++)
			{
				Character ch = Characters[*c];

				float xpos = pos.x + ch.Bearing.x * scale;
				float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;
				// update VBO for each character
				float vertices[6][4] = {
					{ xpos,     ypos + h,   0.0f, 0.0f },
					{ xpos,     ypos,       0.0f, 1.0f },
					{ xpos + w, ypos,       1.0f, 1.0f },

					{ xpos,     ypos + h,   0.0f, 0.0f },
					{ xpos + w, ypos,       1.0f, 1.0f },
					{ xpos + w, ypos + h,   1.0f, 0.0f }
				};
				// render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				pos.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_BLEND);
	}
}