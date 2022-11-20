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


	ScreenNode::ScreenNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : SceneNode(name, geometry, material, texture,normal) {
		int a = 5;
		org_pos_ = position_;
		progress_size_ = glm::vec2(1);
		rotation_ = 0;
		SetOrientation(180, glm::vec3(1, 0, 0));
		glow_ = false;
		glow_amount_ = glm::vec3(0, 0, 0);
	}

	ScreenNode::~ScreenNode() {
	}

	void ScreenNode::SetGlow(bool g) {
		glow_ = g;
	}


	void ScreenNode::SetProgress(glm::vec2 p) {
		progress_size_ = p;
	}
	void ScreenNode::SetProgressX(float p) {
		progress_size_.x = p;
	}
	void ScreenNode::SetProgressY(float p) {
		progress_size_.y = p;
	}
	glm::mat4 ScreenNode::CalculateFinalTransformation(Camera* camera) {
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotation_*glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		float aspectRatio = camera->GetAspectRatio();
		glm::mat4 aspectRatioMat = glm::scale(glm::mat4(1.0f), glm::vec3(aspectRatio,1,1));
		return SceneNode::CalculateFinalTransformation(camera)*rotation*aspectRatioMat;
		
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

		//color
		GLint color = glGetUniformLocation(program, "glow");
		glUniform3f(color, glow_amount_.r*glow_, glow_amount_.g*glow_, glow_amount_.b*glow_);

		//for the screen hud and buttons, simply dont set any camera popsitions, etc. 
		GLint progress_uniform = glGetUniformLocation(program, "progress_size");
		glUniform2f(progress_uniform, progress_size_.x, progress_size_.y);
		SceneNode::SetupShader(program, camera);
	}

	void ScreenNode::Rotate(float angle) {
		rotation_ += angle;
	}



}

