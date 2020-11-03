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

#include "enemy_node.h"


namespace game {

	Enemy::Enemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : Entity(name, geometry, material, texture) {
	}

	Enemy::~Enemy() {
	}

	

}