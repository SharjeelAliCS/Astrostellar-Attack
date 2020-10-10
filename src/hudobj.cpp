/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 2: A multi-component model
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates camera model with the tori.
 * For further info, please read the README file provided.
 *
 */

#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "hudobj.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	UIObj::UIObj(void) : GameObj() {

	}

	UIObj::UIObj(GLuint theshader, Transformation* m) : GameObj(theshader, m) {

	}



	UIObj::~UIObj() {
	}

	// Create the geometry for a square (with two triangles)
	// Return the number of array elements that form the square
	int UIObj::CreateSquare(void) {
		// The face of the square is defined by four vertices and two triangles

		// Number of attributes for vertices and faces
	//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
	//	const int face_att = 3; // Vertex indices (3)

		GLfloat vertex[] = {
			//  square (two triangles)
			   //  Position      Color             Texcoords
			-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
			0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
			0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
			-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
		};


		GLuint face[] = {
			0, 1, 2, //t1
			2, 3, 0  //t2
		};

		GLuint vbo, ebo;

		// Create buffer for vertices
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		// Create buffer for faces (index buffer)
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

		// Return number of elements in array buffer (6 in this case)
		return sizeof(face) / sizeof(GLuint);
	}


	void UIObj::setthisTexture(GLuint w, char *fname)
	{
		glBindTexture(GL_TEXTURE_2D, w);

		int width, height;
		unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		// Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void UIObj::LoadTexture(const char *fname)
	{
		GLuint w[1];
		glGenTextures(1, w);
		glBindTexture(GL_TEXTURE_2D, w[0]);

		float x = 0.2;
		float y = 0.4;


		float h = 0.1;
		glTexCoord2f(0, 0); glVertex2f(x, y);
		glTexCoord2f(0.5, 0); glVertex2f(x + 0.1, y);
		glTexCoord2f(0.5, 0.5); glVertex2f(x + 0.2, y + h);
		glTexCoord2f(0, 0.5); glVertex2f(x, y + h);


		int width, height;
		unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		// Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texture = w[0];

		glBindTexture(GL_TEXTURE_2D, texture);
		texApplied = true;

	}
	void UIObj::Draw(Camera *camera) {
		GameObj::Draw(camera);
	}

}

