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

#include "button_node.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	ButtonNode::ButtonNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : ScreenNode(name, geometry, material, texture, normal) {
	}



	ButtonNode::~ButtonNode() {
	}

	bool ButtonNode::Clicked(float x, float y) {
		float btn_height = scale_.y*screen_height_;
		float btn_width = btn_height * (scale_.x / scale_.y);

		float screen_pos_x = (position_.x/2+0.5)*screen_width_;
		float screen_pos_y = (-position_.y/2+0.5)*screen_height_;

		if (x > screen_pos_x - btn_width / 2 &&
			x < screen_pos_x + btn_width / 2 &&
			y > screen_pos_y - btn_height / 2 &&
			y < screen_pos_y + btn_height / 2) {
			return true;
		}

		return false;
	}

	void ButtonNode::UpdateScreenSize(float x, float y) {
		screen_width_ = x;
		screen_height_ = y;
	}
	glm::mat4 ButtonNode::CalculateFinalTransformation(Camera* camera) {
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotation_*glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		float aspectRatio = camera->GetAspectRatio();
		glm::mat4 aspectRatioMat = glm::scale(glm::mat4(1.0f), glm::vec3(aspectRatio, 1, 1));
		return rotation * SceneNode::CalculateFinalTransformation(camera)*aspectRatioMat;

	}

	void ButtonNode::Update(float deltaTime) {
		SceneNode::Update(deltaTime);
	}

	void ButtonNode::Draw(Camera *camera) {
		ScreenNode::Draw(camera);
	}
	void ButtonNode::SetupShader(GLuint program, Camera* camera) {

		SceneNode::SetupShader(program, camera);
	}


}

