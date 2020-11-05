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

#include "screen_node.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	ScreenNode::ScreenNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture) {
		int a = 5;
		org_pos_ = position_;
		progress_size_ = 1.0;
	}



	ScreenNode::~ScreenNode() {
	}

	
	void ScreenNode::SetProgress(float p) {
		progress_size_ = p;
	}
	glm::mat4 ScreenNode::CalculateFinalTransformation(Camera* camera) {
		float aspectRatio = camera->GetAspectRatio();
		glm::mat4 aspectRatioMat = glm::scale(glm::mat4(1.0f), glm::vec3(aspectRatio,1,1));
		return SceneNode::CalculateFinalTransformation(camera)*aspectRatioMat;
		
	}

	void ScreenNode::Update(float deltaTime) {
		SceneNode::Update(deltaTime);
	}

	void ScreenNode::Draw(Camera *camera) {
		if (!draw_)return;
		// Select proper material (shader program)
		glUseProgram(material_);

		glEnable(GL_BLEND);
		glDepthMask(false);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

		// Set geometry to draw
		glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

		// Set globals for camera
		camera->SetupShader(material_);

		// Set world matrix and other shader input variables
		SetupShader(material_, camera);

		// Draw geometry
		if (mode_ == GL_POINTS) {
			glDrawArrays(mode_, 0, size_);
		}
		else {
			glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
		}

		for (std::vector<SceneNode*>::iterator it = children_.begin(); it != children_.end(); ++it) {
			(*it)->Draw(camera);
		}
		glDisable(GL_BLEND);
		glDepthMask(true);
	}
	void ScreenNode::SetupShader(GLuint program, Camera* camera) {


		GLint progress_uniform = glGetUniformLocation(program, "progress_size");
		glUniform1f(progress_uniform, progress_size_);
		SceneNode::SetupShader(program, camera);
	}



}

