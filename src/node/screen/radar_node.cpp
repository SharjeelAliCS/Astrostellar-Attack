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
#include "radar_node.h"

namespace game {


	RadarNode::RadarNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : ScreenNode(name, geometry, material, texture,normal) {
	}

	RadarNode::~RadarNode() {
	}

	glm::mat4 RadarNode::CalculateFinalTransformation(Camera* camera) {
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotation_*glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		float aspectRatio = camera->GetAspectRatio();
		glm::mat4 aspectRatioMat = glm::scale(glm::mat4(1.0f), glm::vec3(aspectRatio, 1, 1));
		return rotation*SceneNode::CalculateFinalTransformation(camera)*aspectRatioMat;

	}

	void RadarNode::Update(float deltaTime) {
		SceneNode::Update(deltaTime);
	}

	void RadarNode::Draw(Camera *camera) {

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

		dot_color_.clear();
		dot_pos_.clear();
	}
	void RadarNode::SetupShader(GLuint program, Camera* camera) {
		
		GLint sizeUniform = glGetUniformLocation(program, "arr_size");
		glUniform1i(sizeUniform, dot_pos_.size());

		//if the radar has dots on it, then set the uniform arrays for dot position and dot colors. 
		if (dot_pos_.size() > 0) {

			GLint arr_dots = glGetUniformLocation(program, "dots_pos");
			glUniform2fv(arr_dots, dot_pos_.size(), reinterpret_cast<GLfloat *>(dot_pos_.data()));
			GLint arr_colors = glGetUniformLocation(program, "dot_colors");
			glUniform3fv(arr_colors, dot_color_.size(), reinterpret_cast<GLfloat *>(dot_color_.data()));
		} 
		SceneNode::SetupShader(program, camera);
	}

	void RadarNode::AddDotPos(glm::vec2 dot) {
		dot_pos_.push_back(dot);
	}
	void RadarNode::AddDotColor(glm::vec3 color) {
		dot_color_.push_back(color);
	}


}

