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
		attackDisabledFor_ = 0;
		InitState();
		rate_of_fire_ = 1;
		follow_duration_ = 5;
		move_away_duration_ = 5;
		time_since_last_move_ = 0;
		min_distance_ = 20;
		detect_distance_ = 600;
		max_stationary_distance_ = 800;
		time_since_fire_ = 0;
		boost_speed_ = 50;
		phase_ = 0;
		this->Scale(glm::vec3(6));
		upgrades = new std::map<std::string, int>();
		weaponStats = new std::map<std::string, float>();
		proj_color_ = glm::vec3(0, 1, 0.18);
	}

	Enemy::~Enemy() {
	}

	void Enemy::InitState(void) {

		FindNewDirection(0);
		if (name_.compare("bossOrb") == 0) {
			active_state_ = &Enemy::Fire;
		}
		else {
			active_state_ = &Enemy::FindPlayer;
		}
	}

	std::map<std::string, int> Enemy::GetDrops(void) {
		//randomize some bonus loot
		if (rand() % 100 < 10) {
			drops["stellaranite_Fragments"]++;
		}
		if (rand() % 100 < 2) {
			drops["stellaranite_Slabs"]++;
		}
		if (rand() % 100 < 20) {
			int x = rand() % 100;
			if (x < 25) {
				drops["charge_Blast_Ammo"]+= rand()%5+5;
			}else if (x < 50) {
				drops["pursuer_Ammo"]+= rand()%20+5;
			}else if (x < 75) {
				drops["shotgun_Ammo"]+= rand()%10+5;
			}else {
				drops["sniper_Ammo"]+= rand()%5+5;
			}
		}
		drops["credits"] += rand() % 3;
		
		return drops; 
	}

	void Enemy::DisableAttackFor(double sec) {
		attackDisabledFor_ += sec;
	}

	void Enemy::Fire(float deltaTime) {

		time_since_fire_ += deltaTime;

		//make sure time since fire exceeds rate of fire to attack
		if (time_since_fire_ < rate_of_fire_ || attackDisabledFor_ > 0) {
			return;
		}
		time_since_fire_ = 0;

		std::string proj_type;
		if (name_.compare("bossOrb") == 0) {
			proj_type = "boss";
		}
		else {
			proj_type = "enemy";
		}

		//create the projectile  and assign various attributes
		Projectile* missile = new Projectile("missile", proj_type, *upgrades, *weaponStats, false, proj_rsc_->geom, proj_rsc_->mat, proj_rsc_->tex);
		missile->SetPlayer(player_);

		missile->SetOrientation(GetOrientation());

		glm::vec3 proj_pos = CalculateParentChildPos();
		missile->SetSpeed(missile->GetSpeed() +this->getCurSpeed());
		missile->init();
		missile->SetPosition(proj_pos);
		missile->SetColor(proj_color_);
		glm::vec3 pos = CalculateAimPosition(missile->GetSpeed());
		float speed = player_->getCurSpeed()*0.1;

		missile->GetOrientationObj()->FaceTowards(proj_pos, pos);

		missiles.push_back(missile);

	}

	void Enemy::FindNewDirection(float deltaTime) {
		rotation_axis_ = glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX));
		rotation_axis_ = glm::normalize(rotation_axis_);
	}
	void Enemy::FollowPlayer(float deltaTime) {

		orientation_->FaceTowards(position_, player_->GetPosition(), true);
		orientation_->RotateTowards(deltaTime * 3);

		//find the player if theyre within detect distance
		if (glm::distance(position_, player_->GetPosition()) > detect_distance_) {
			FindNewDirection(deltaTime);
			active_state_ = &Enemy::FindPlayer;
			return;
		}

		//move to a random direction if player is too close
		if (glm::distance(position_, player_->GetPosition()) < min_distance_*glm::length(scale_)) {
			time_since_last_move_ = 0;
			active_state_ = &Enemy::MoveToRandomDirection;
			return;
		}
		position_ += orientation_->GetForward()*movement_speed*deltaTime;

		Fire(deltaTime);

		//if following for too long, switch to random direction
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

		//if the player is within dectection, follow or ram it
		if (glm::distance(position_, player_->GetPosition() )< detect_distance_) {
			
			glm::vec3 pos = CalculateAimPosition(getCurSpeed());
			if (enemy_type_ == "Ram") {
				active_state_ = &Enemy::RamPlayer;
			}
			else {
				active_state_ = &Enemy::FollowPlayer;
			}
		}

		//if the player is way too far, get a little closer to it
		else if (glm::distance(position_, player_->GetPosition()) > max_stationary_distance_) {
			orientation_->FaceTowards(position_, player_->GetPosition(), true);
			orientation_->RotateTowards(deltaTime * 3);
			position_ += orientation_->GetForward()*movement_speed*deltaTime;

		}
		//if player is far, then simply move in a circle
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

	void Enemy::RamPlayer(float deltaTime) {
		//simply move straight towards the player, and increase speed if player is close to ram
		orientation_->FaceTowards(position_, player_->GetPosition(), true);
		orientation_->RotateTowards(deltaTime * 3);
		if (glm::distance(position_, player_->GetPosition()) > detect_distance_) {
			FindNewDirection(deltaTime);
			active_state_ = &Enemy::FindPlayer;
			return;
		}
		float speed = movement_speed;
		if (glm::distance(position_, player_->GetPosition()) < min_distance_*glm::length(scale_)) {
			speed += boost_speed_;
		}
		position_ += orientation_->GetForward()*speed*deltaTime;

		if (time_since_last_move_ > follow_duration_) {
			time_since_last_move_ = 0;
			active_state_ = &Enemy::MoveToRandomDirection;
		}
	}

	//source for the equations (NOT MY OWN WORK): https://gamedev.stackexchange.com/questions/35859/algorithm-to-shoot-at-a-target-in-a-3d-game
	glm::vec3 Enemy::CalculateAimPosition(float speed) {

		glm::vec3 player_velocity;
		
		if (player_->IsFirstPerson()) {
			player_velocity = player_->getCurSpeed() * player_->GetOrientationObj()->GetForward();
		}
		else {
			player_velocity = player_->getCurSpeed() * player_->GetCam()->GetForward();
		} 

		glm::vec3 player_pos = player_->GetPosition();

		glm::vec3 delta = player_pos - CalculateParentChildPos();

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
	void Enemy::DoNothing(float deltaTime) {

	}
	void Enemy::PoisonAura(float deltaTime) {
		if (glm::distance(position_, player_->GetPosition()) < min_distance_) {
			player_->damage(deltaTime * 20);
		}
	}

	void Enemy::Update(float deltaTime) {
		if (attackDisabledFor_ > 0) {
			attackDisabledFor_ -= deltaTime;
		}
		if(name_.compare("Boss")==0)PoisonAura(deltaTime);
		(this->*active_state_)(deltaTime);
		time_since_last_move_ += deltaTime;

		AgentNode::Update(deltaTime);
	}

	void Enemy::Draw(Camera* camera) {
		AgentNode::Draw(camera);
	}
	
	

}
