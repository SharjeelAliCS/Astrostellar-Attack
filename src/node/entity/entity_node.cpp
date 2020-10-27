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

#include "entity_node.h"


namespace game {

	Entity::Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture) {
	}

	Entity::~Entity() {
	}

	bool Entity::Exists(void) const {
		return exists_;
	}

	void Entity::SetMovementSpeed(float s) {
		movement_speed = s;
	}

	bool Entity::Hit(glm::vec3 pos, float range) {
		float dis = glm::distance(pos, position_);
		//std::cout << dis << std::endl;
		if (dis <= range) {

			exists_ = false;
		}
		return !exists_;
	}
	void Entity::RotateOverTime(float rotation_speed, glm::vec3 rotation_normal) {
		rotation_normal_ = rotation_normal;
		rotation_speed_ = rotation_speed;
	}



}
