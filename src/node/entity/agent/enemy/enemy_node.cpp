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
		rate_of_fire_ = 1;
		follow_duration_ = 5;
		move_away_duration_ = 5;
		time_since_last_move_ = 0;
		min_distance_ = 50;
		detect_distance_ = 750;
		time_since_fire_ = 0;
		boost_speed_ = 50;
	}

	Enemy::~Enemy() {
	}

	void Enemy::InitState(void) {

		FindNewDirection(0);
		active_state_ = &Enemy::FindPlayer;
	}

	void Enemy::Fire(float deltaTime) {

		time_since_fire_ += deltaTime;
		if (time_since_fire_< rate_of_fire_) {
			return;
		}
		time_since_fire_ = 0;

		Projectile* missile = new Projectile("missile", "enemy", upgrades, proj_rsc_->geom, proj_rsc_->mat, proj_rsc_->tex);
		missile->SetPlayer(player_);

		missile->SetOrientation(GetOrientation());

		missile->setSpeed(this->getCurSpeed() * 3);
		missile->init();
		missile->SetPosition(position_);
		missile->SetColor(glm::vec3(0, 1, 0.18));
		glm::vec3 pos = CalculateAimPosition(missile->GetSpeed());
		float speed = player_->getCurSpeed()*0.1;

		missile->GetOrientationObj()->FaceTowards(position_, pos);

		missiles.push_back(missile);
	}

	void Enemy::FindNewDirection(float deltaTime) {
		rotation_axis_ = glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX));
		rotation_axis_ = glm::normalize(rotation_axis_);
	}
	void Enemy::FollowPlayer(float deltaTime) {

		orientation_->FaceTowards(position_, player_->GetPosition(), true);
		orientation_->RotateTowards(deltaTime * 3);
		if (glm::distance(position_, player_->GetPosition()) > detect_distance_) {
			FindNewDirection(deltaTime);
			active_state_ = &Enemy::FindPlayer;
			return;
		}
		if (glm::distance(position_, player_->GetPosition()) < min_distance_) {
			time_since_last_move_ = 0;
			active_state_ = &Enemy::MoveToRandomDirection;
			return;
		}
		position_ += orientation_->GetForward()*movement_speed*deltaTime;

		Fire(deltaTime);

		if (time_since_last_move_ > follow_duration_) {
			time_since_last_move_ = 0;
			active_state_ = &Enemy::MoveToRandomDirection;
		}

	}
	void Enemy::AttackPlayer(float deltaTime) {
		
		Fire(deltaTime);
		active_state_ = &Enemy::FollowPlayer;
	}
	void Enemy::FindPlayer(float deltaTime) {
		if (glm::distance(position_, player_->GetPosition() )< detect_distance_) {
			
			glm::vec3 pos = CalculateAimPosition(getCurSpeed());
			//orientation_->FaceTowards(position_, pos,true);

			active_state_ = &Enemy::FollowPlayer;
		}
		else {
			Rotate(deltaTime*30, rotation_axis_);
			position_ += orientation_->GetForward()*(movement_speed+boost_speed_)*deltaTime;
		}
	}
	void Enemy::MoveToRandomDirection(float deltaTime) {
		float radius = 10;

		glm::vec3 pos = position_+glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX));

		orientation_->FaceTowards(position_, pos,true);
		active_state_ = &Enemy::MoveAwayFromPlayer;
	}

	void Enemy::MoveAwayFromPlayer(float deltaTime) {
		if (time_since_last_move_ > move_away_duration_) {
			time_since_last_move_ = 0;
			FindNewDirection(deltaTime);
			active_state_ = &Enemy::FindPlayer;
		}
		orientation_->RotateTowards(deltaTime*3);
		position_ += orientation_->GetForward()*movement_speed*deltaTime;
	}

	//https://gamedev.stackexchange.com/questions/35859/algorithm-to-shoot-at-a-target-in-a-3d-game
	glm::vec3 Enemy::CalculateAimPosition(float speed) {

		glm::vec3 player_velocity;
		
		if (player_->IsFirstPerson()) {
			player_velocity = player_->getCurSpeed() * player_->GetOrientationObj()->GetForward();
		}
		else {
			player_velocity = player_->getCurSpeed() * player_->GetCam()->GetForward();
		} 

		glm::vec3 player_pos = player_->GetPosition();

		glm::vec3 delta = player_pos - position_;

		float a = glm::dot(player_velocity, player_velocity) - pow(speed, 2);
		float b = 2 * glm::dot(player_velocity, delta);
		float c = glm::dot(delta, delta);

		float p = -b / (2 * a);
		float q = (float)sqrt((b * b) - 4 * a * c) / (2 * a);

		float t1 = p - q;
		float t2 = p + q;
		float t;

		if (t1 > t2 && t2 > 0){
			t = t2;
		}
		else{
			t = t1;
		}

		glm::vec3 aim_spot = player_pos + player_velocity * t;
		return aim_spot;

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
