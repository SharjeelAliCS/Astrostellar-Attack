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

#include "boss_node.h"
#include "player_node.h"

namespace game {

	Boss::Boss(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : Enemy(name, geometry, material, texture, normal) {
		movement_speed = 50;
		InitState();
		rate_of_fire_ = 1;
		follow_duration_ = 50000;
		move_away_duration_ = 5;
		time_since_last_move_ = 0;
		min_distance_ = 250;
		detect_distance_ = 50000;
		time_since_fire_ = 0;
		boost_speed_ = 20;
		phase_ = 0;
		this->Scale(glm::vec3(6));
		upgrades = new std::map<std::string, int>();
		weaponStats = new std::map<std::string, float>();
	}

	Boss::~Boss() {
	}

	void Boss::InitState(void) {

		FindNewDirection(0);
		active_state_ = &Enemy::FindPlayer;
	}


	void Boss::Draw(Camera* camera) {
		for (std::vector<Enemy*>::iterator it = orbs_.begin(); it != orbs_.end(); ++it) {
			(*it)->Draw(camera);
		}
		Enemy::Draw(camera);
	}
	void Boss::Update(float deltaTime) {
		for (std::vector<Enemy*>::iterator it = orbs_.begin(); it != orbs_.end(); ++it) {
			(*it)->Update(deltaTime);
		}
		Enemy::Update(deltaTime);

	}

	glm::mat4 Boss::CalculateFinalTransformation(Camera* camera) {
		// World transformation
		glm::mat4 transf = SceneNode::CalculateFinalTransformation(camera);

		for (std::vector<Enemy*>::iterator it = orbs_.begin(); it != orbs_.end(); ++it) {
			(*it)->SetParentTransform(transf);
		}

		return transf;
	}


}
