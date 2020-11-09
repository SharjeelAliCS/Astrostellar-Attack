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

#include "projectile_node.h"

namespace game {



	Projectile::Projectile(const std::string name, const std::string type, const std::map<std::string, int> upgrades, glm::quat orient, const Resource *geometry, const Resource *material, const Resource *texture)
		: Entity(name, geometry, material, texture) {
		//default all to zero
		upg = upgrades;
		ttl    = 0;
		pierce = 0;
		dotDmg = 0;
		dotDuration = 0;
		dotStackMax = 0;
		target = NULL;
		dmg = []() {return 0;};
		move = [this](float deltaTime) { /*do nothing*/ };
		this->SetOrientation(orient);
		this->SetScale(glm::vec3(0.02));
		init(type, upgrades);

		//displayStats();
		
	}

	Projectile::~Projectile() {
		//delete all sub-projectiles then self
	}
	

	//TODO: make the movement speed a function of the player speed when fired
	void Projectile::init(const std::string type, std::map<std::string, int> upgrades) {
		//damage upgrades are multiplictive 
		if (type.compare("laserBattery") == 0) {
			//travels 5 seconds +10% per level
			ttl = glfwGetTime() + 5 * pow(1.1, upgrades["laserBatteryRangeLevel"]);
			//pierces 0 to 5 targets (+1 per upgrade)
			pierce = upgrades["laserBatteryPierceLevel"];
			//deals 10 damage + 10% per level
			dmg = [this]() {
				return 10 * pow(1.1, upg["laserBatteryDamageLevel"]);
			};
			//moves forward at a speed of 10.0
			move = [this](float deltaTime) {
				position_ -= 10.0f * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};

		}else if (type.compare("pursuer") == 0) {
			//travels 20 seconds
			ttl = glfwGetTime() + 20;
			//deals 3 damage + 10% per level
			dmg = [this]() {
				return 5;
			};
			//moves forward at a speed of 20
			move = [this](float deltaTime) {
				//will need access to enemy vector, determine nearest enemy and then go
				if (glfwGetTime() - ttl < 1.5) {
					//go straight initially before locking on
					position_ -= 20.0f * glm::normalize(-orientation_->GetForward()) * deltaTime;
				}
				else if (target == NULL){
					//target died before reaching
					//find nearest target and set it as target
				}
				else {
					//chase target
				}
			};


		}else if (type.compare("chargeBlast") == 0) {
			//travels 12 seconds
			ttl = glfwGetTime() + 12;
			//deals 30 damage + 10% per level
			dmg = [this]() {
				return 30 * pow(1.1, upg["chargeDamageLevel"]);
			};
			//moves forward at a speed of 15.0
			move = [this](float deltaTime) {
				position_ -= 15.0f * glm::normalize(-orientation_->GetForward()) * deltaTime;	
			};
			this->Scale(glm::vec3(1.5 * pow(1.1, upgrades["chargeRadiusLevel"])));


		}else if (type.compare("sniperShot") == 0) {
			//travels 15 seconds +10% per level
			ttl = glfwGetTime() + 15 * pow(1.1, upgrades["sniperRangeLevel"]);
			//deals (1 damage + 15 per second travelled) + 10% per level
			dmg = [this]() {
				return (1 - 15*(ttl - glfwGetTime() - (15 * pow(1.1, upg["sniperRangeLevel"])))) * pow(1.1, upg["sniperDamageLevel"]);
			};
			//moves forward at a speed of 50.0
			move = [this](float deltaTime) {
				position_ -= 50.0f * glm::normalize(-orientation_->GetForward()) * deltaTime;	
			};

					   
		}else if (type.compare("shotgun") == 0) { // multiple of these will be created on the player side
			//travels 0.8 seconds
			ttl = glfwGetTime() + 0.8;
			//deals 5 damage + 10% per level
			dmg = [this]() {
				return 5 * pow(1.1, upg["shotgunDamageLevel"]);
			};
			//moves forward at a speed of 40.0
			this->Rotate(rand() % 30 - 15, glm::vec3(1, 0, 0));
			this->Rotate(rand() % 30 - 15, glm::vec3(0, 1, 0));
			this->Rotate(rand() % 30 - 15, glm::vec3(0, 0, 1));
			move = [this](float deltaTime) {
				position_ -= 40.0f * glm::normalize(-orientation_->GetForward()) * deltaTime;	
			};
		
					   
		}else if (type.compare("naniteTorpedo") == 0) { // multiple of these will be created on the player side
			//travels 10 seconds
			ttl = glfwGetTime() + 10;
			//deals 0 impact damage
			dmg = [this]() {
				return 0;
			};
			//5dps per stack up to 5 stacks, lasts for 5 seconds
			dotDmg = 5 * pow(1.1, upgrades["naniteTorpedoDamageLevel"]);
			dotDuration = glfwGetTime() + 5 *pow(1.1, upgrades["naniteTorpedoDurationLevel"]);
			dotStackMax = 5 + upgrades["naniteTorpedoStackLevel"];
			//moves forward at a speed of 20.0
			move = [this](float deltaTime) {
				position_ -= 20.0f * glm::normalize(-orientation_->GetForward()) * deltaTime;	
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
			<< "posbefore: " << position_.x << " " << position_.y << " " << position_.z << std::endl;
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

}
