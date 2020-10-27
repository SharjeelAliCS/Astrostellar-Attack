/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "skybox_node.h"


namespace game {

	SkyBox::SkyBox(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture) {
	}

	SkyBox::~SkyBox() {
	}

	void SkyBox::Draw(Camera *camera) {
		// Select proper material (shader program)

		SetPosition(camera->GetPosition());
		
		glUseProgram(material_);

		// Set geometry to draw
		glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

		// Set globals for camera
		camera->SetupShader(material_);

		// Set world matrix and other shader input variables
		SetupShader(material_,camera);

		// Draw geometry
		glDisable(GL_DEPTH_TEST);
		if (mode_ == GL_POINTS) {
			glDrawArrays(mode_, 0, size_);
		}
		else {
			glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
		}
		glEnable(GL_DEPTH_TEST);
	}

}
