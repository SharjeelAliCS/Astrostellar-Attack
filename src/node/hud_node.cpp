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
	}



	HUDNode::~HUDNode() {
	}

	void HUDNode::Draw(Camera *camera) {
		SceneNode::Draw(camera);
	}

	void HUDNode::Update(float deltaTime) {
		SceneNode::Update(deltaTime);
	}

}

