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

#include "agent_node.h"


namespace game {

	AgentNode::AgentNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : Entity(name, geometry, material, texture, normal) {

		geo = geometry;
		mat = material;
		tex = texture;

	}

	AgentNode::~AgentNode() {
	}


	void AgentNode::Update(float deltaTime) {

		for (std::vector<Projectile*>::iterator it = missiles.begin(); it != missiles.end();) {
			(*it)->Update(deltaTime);
			if ((*it)->Exists()) {
				++it;

			}
			else {
				int a;
				*it = NULL;
				std::cout << "missile died" << std::endl;
				it = missiles.erase(it);
			}

		}

	}


	void AgentNode::Draw(Camera* camera) {
		for (std::vector<Projectile*>::iterator it = missiles.begin(); it != missiles.end(); ++it) {
			(*it)->Draw(camera);
		}
		Entity::Draw(camera);
	}

}
