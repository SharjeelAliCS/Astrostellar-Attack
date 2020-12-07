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

#include "player_node.h"
#include "projectile_node.h"

namespace game {



	Projectile::Projectile(const std::string name, const std::string t, std::map<std::string, int> upgrades,
		const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal)
		: Entity(name, geometry, material, texture,normal) {
		//default all to zero
		type = t;
		upg = upgrades;
		ttl    = 0;
		pierce = 0;
		dotDmg = 0;
		dotDuration = 0;
		dotStackMax = 0;
		target = NULL;
		health_ = 10000000000;
		speed = 300;
		dmg = []() {return 0;};
		move = [this](float deltaTime) { /*do nothing*/ };
		this->SetScale(glm::vec3(0.6));
		//displayStats();
		geom_orientation_->Rotate(90, glm::vec3(1,0,0));
		
	}

	Projectile::~Projectile() {
		//delete all sub-projectiles then self
	}
	
	void Projectile::SetPlayer(Player* p) {
		player_ = p;
	}

	//TODO: make the movement speed a function of the player speed when fired
	void Projectile::init() {
		//damage upgrades are multiplictive 
		if (type.compare("enemy") == 0) {
			//travels 5 seconds
			ttl = glfwGetTime() + 5;
			pierce = 0;
			dmg = [this]() {
				return 5;
			};
			move = [this](float deltaTime) { //minor player tracking
				orientation_->FaceTowards(position_, player_->GetPosition(), true);
				float rot_speed = deltaTime;
				if (player_->GetBoosted()) {
					rot_speed*=0.5;
				}
				else {
					rot_speed*=0.05;
				}
				orientation_->RotateTowards(rot_speed);
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};
		}
		else if (type.compare("laserBattery") == 0) {
			//travels  seconds +10% per level
			ttl = glfwGetTime() + 5 * pow(1.1, upg["laserBatteryRangeLevel"]);
			//pierces 0 to 5 targets (+1 per upgrade)
			pierce = upg["laserBatteryPierceLevel"];
			//deals 10 damage + 10% per level
			dmg = [this]() {
				return 10 * pow(1.1, upg["laserBatteryDamageLevel"]);
			};
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};

		}else if (type.compare("pursuer") == 0) {
			//travels 20 seconds
			ttl = glfwGetTime() + 20;
			dmg = [this]() {
				return 5;
			};
			move = [this](float deltaTime) {
				//will need access to enemy vector, determine nearest enemy and then go
				
				if (ttl - glfwGetTime() > 19.8) {
					//go straight initially before locking on
					position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
				}
				else if (target == NULL){ // MAKE SURE ENEMY POINTERS ARE SET TO NULL WHEN DESTROYED
					float minD = 100000;
					for (SceneNode* a : *enemies) { // this might be a problem bc copy...
						float d = glm::distance(this->GetPosition(), a->GetPosition());
						if (minD > d) {
							minD = d;
							target = a;
						}
					}
				}
				else {

					//chase target
					orientation_->FaceTowards(this->GetPosition(), target->GetPosition(), true);
					orientation_->RotateTowards(deltaTime*3);
					position_ += speed * orientation_->GetForward()*deltaTime;
				}			
			};

		}else if (type.compare("chargeBlast") == 0) {
			speed += 75.0f;
			//travels 12 seconds
			ttl = glfwGetTime() + 12;
			//deals 150 damage + 10% per level
			dmg = [this]() {
				return 150 * pow(1.1, upg["chargeDamageLevel"]);
			};
			pierce = 5;
			this->Scale(glm::vec3(5));
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
				if(this->GetScale().x<70+10*upg["chargeRadiusLevel"]){
					this->Scale(glm::vec3(max(1.025, pow(1.01, 1+upg["chargeRadiusLevel"]))));
				}
			};
			


		}else if (type.compare("sniperShot") == 0) {
			speed += 100.0f;
			this->Scale(glm::vec3(2));
			//travels 15 seconds +10% per level
			ttl = glfwGetTime() + 15 * pow(1.1, upg["sniperRangeLevel"]);
			//deals (1 damage + 60 per second travelled) + 10% per level
			int damageFactor = 60;
			dmg = [this, damageFactor]() {
				
				return (1 - damageFactor *(ttl - glfwGetTime() - (damageFactor * pow(1.1, upg["sniperRangeLevel"])))) * pow(1.1, upg["sniperDamageLevel"]);
			};
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};

					   
		}else if (type.compare("shotgun") == 0) { // multiple of these will be created on the player side
			//travels 0.7 seconds
			ttl = glfwGetTime() + 0.7;
			//deals 5 damage + 10% per level
			dmg = [this]() {
				return 5 * pow(1.1, upg["shotgunDamageLevel"]);
			};
			//random spread
			this->Rotate(rand() % 14 - 7, glm::vec3(1, 0, 0));
			this->Rotate(rand() % 14 - 7, glm::vec3(0, 1, 0));
			this->Rotate(rand() % 14 - 7, glm::vec3(0, 0, 1));
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};
		
					   
		}else if (type.compare("naniteTorpedo") == 0) { // multiple of these will be created on the player side
			
			speed += 20;
			//travels 10 seconds
			ttl = glfwGetTime() + 10;
			//deals 0 impact damage
			dmg = [this]() {
				return 0;
			};
			//5dps per stack up to 5 stacks, lasts for 5 seconds
			dotDmg = 5 * pow(1.1, upg["naniteTorpedoDamageLevel"]);
			dotDuration = glfwGetTime() + 5 *pow(1.1, upg["naniteTorpedoDurationLevel"]);
			dotStackMax = 5 + upg["naniteTorpedoStackLevel"];
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};
		}
		//if not a valid type of projectile then flag for deletion
		else {
			exists_ = false;
		}
	}

	//for debuggin
	void Projectile::displayStats() {
		std::cout
			<< "ttl: " << ttl << std::endl
			<< "dmg: " << dmg() << std::endl
			<< "pierce: " << pierce << std::endl
			<< "dotDmg: " << dotDmg << std::endl
			<< "dotDuration: " << dotDuration << std::endl
			<< "dotStackMax: " << dotStackMax << std::endl
			<< "posbefore: " << position_.x << " " << position_.y << " " << position_.z << std::endl
			<< "orient: " <<orientation_->GetForward().x << " "<<orientation_->GetForward().y << " "<<orientation_->GetForward().z << " " << std::endl;
	}


	void Projectile::SetTTL(double t) {
		ttl = t;
	}

	void Projectile::Update(float deltaTime) {
		int thisTime = (int)glfwGetTime();
		if (thisTime - lastTime != 0) {
			//displayStats();
		}
		lastTime = thisTime;
		float a = (float)deltaTime;
		//Constantly move the projectile using the speed and delta time. 
		move(deltaTime);
		
		//If it exceeds range, set exists to false
		if (glfwGetTime()>=ttl) {
			exists_ = false;
		}
	}
	float Projectile::GetDamage(void) {
	
		if (pierce <= 0) {
			exists_ = false;
		}
		pierce--;

		return dmg();
	}
}
