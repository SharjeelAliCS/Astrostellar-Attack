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

#include "hud_node.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	HUDNode::HUDNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture) {
		int a = 5;
		org_pos_ = position_;
	}



	HUDNode::~HUDNode() {
	}

	
	glm::mat4 HUDNode::CalculateFinalTransformation(Camera* camera) {
		float aspectRatio = camera->GetAspectRatio();
		glm::mat4 aspectRatioMat = glm::scale(glm::mat4(1.0f), glm::vec3(aspectRatio,1,1));
		return SceneNode::CalculateFinalTransformation(camera)*aspectRatioMat;
		
	}

	void HUDNode::Update(float deltaTime) {
		SceneNode::Update(deltaTime);
	}



}

