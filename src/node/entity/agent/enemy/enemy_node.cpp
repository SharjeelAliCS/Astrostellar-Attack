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
#include "player_node.h"

namespace game {

	Enemy::Enemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : AgentNode(name, geometry, material, texture,normal) {
		movement_speed = 50;
		InitState();
		rate_of_fire_ = 5;
		follow_duration_ = 5;
		move_away_duration_ = 3;
		time_since_last_move_ = 0;

		detect_distance_ = 500;
	}

	Enemy::~Enemy() {
	}

	void Enemy::InitState(void) {
		active_state_ = &Enemy::FindPlayer;
	}
	void Enemy::FollowPlayer(float deltaTime) {
		//std::map<std::string, int>
		if (glm::distance(position_, player_->GetPosition()) > detect_distance_) {
			active_state_ = &Enemy::FindPlayer;
			return;
		}

		orientation_->RotateTowards(position_, player_->GetPosition());

		//position_ += orientation_->GetForward()*movement_speed*deltaTime;

		if (time_since_last_move_ > follow_duration_) {
			time_since_last_move_ = 0;
			active_state_ = &Enemy::AttackPlayer;
		}

	}
	void Enemy::AttackPlayer(float deltaTime) {
		//std::cout << name_ << "fired at pos " << position_.x << "," << position_.y << "," << position_.z << std::endl;

		Projectile* missile = new Projectile("missile", "enemy", upgrades, geo, mat, tex);
		missile->SetPlayer(player_);

		missile->SetOrientation(GetOrientation());

		missile->setSpeed(this->getCurSpeed()*3);
		missile->init();
		missile->SetPosition(position_);
		
		//calculate position of player after missile ttl seconds:

		glm::vec3 pos = player_->GetPosition();
		glm::vec3 fo = player_->GetOrientationObj()->GetForward();

		pos += fo*player_->getCurSpeed()*(float)missile->GetTTL();
		missile->GetOrientationObj()->RotateTowards(position_, pos);

		missiles.push_back(missile);
		if (missiles.size() > 0) {
			std::cout << "Enemy fired a missile!" << std::endl;
		}
		orientation_->RotateTowards(position_, position_+ orientation_->GetSide());


		active_state_ = &Enemy::MoveAwayFromPlayer;
	}
	void Enemy::FindPlayer(float deltaTime) {
		if (glm::distance(position_, player_->GetPosition() )< detect_distance_) {
			active_state_ = &Enemy::FollowPlayer;
		}
	}
	void Enemy::MoveAwayFromPlayer(float deltaTime) {
		if (time_since_last_move_ > move_away_duration_) {
			time_since_last_move_ = 0;
			active_state_ = &Enemy::FollowPlayer;
		}
		//position_ += orientation_->GetForward()*movement_speed*deltaTime;
	}

	void Enemy::Update(float deltaTime) {
		
		(this->*active_state_)(deltaTime);
		time_since_last_move_ += deltaTime;

		AgentNode::Update(deltaTime);
	}


	void Enemy::Draw(Camera* camera) {
		AgentNode::Draw(camera);
	}
	
	

}
