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

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	RadarNode::RadarNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : ScreenNode(name, geometry, material, texture) {
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
	}
	void RadarNode::SetupShader(GLuint program, Camera* camera) {
		
		GLint sizeUniform = glGetUniformLocation(program, "arr_size");
		glUniform1i(sizeUniform, entity_pos_.size());


		if (entity_pos_.size() > 0) {

			//std::cout << entity_pos_[0].x << "," << entity_pos_[0].y << std::endl;
			GLint arr_dots = glGetUniformLocation(program, "dots");
			glUniform2fv(arr_dots, entity_pos_.size(), reinterpret_cast<GLfloat *>(entity_pos_.data()));
		} 
		SceneNode::SetupShader(program, camera);
	}
	void RadarNode::SetEntityPositions(std::vector<glm::vec2>& e) {
		entity_pos_ = e;
	}


}
