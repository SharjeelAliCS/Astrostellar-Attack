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

#include "asteroid_node.h"


namespace game {

	AsteroidNode::AsteroidNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : ObstacleNode(name, geometry, material, texture,normal) {
	}

	AsteroidNode::~AsteroidNode() {
	}

	std::map<std::string, int> AsteroidNode::GetDrops(void) {
		//randomize some bonus loot
		if (rand() % 100 < 5) {
			drops["stellaranite_Fragments"]++;
		}
		if (rand() % 100 < 1) {
			drops["stellaranite_Slabs"]++;
		}
		drops["credits"] += rand() % 5;

		return drops;
	}

}
