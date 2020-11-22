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



	Projectile::Projectile(const std::string name, const std::string type, std::map<std::string, int> upgrades, SceneNode* p, 
		std::vector<SceneNode*>* ast, std::vector<Enemy*>* enemy,
		const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal)
		: Entity(name, geometry, material, texture,normal) {
		//default all to zero
		upg = upgrades;
		player = p;
		asteroids = ast;
		enemies = enemy;
		ttl    = 0;
		pierce = 0;
		dotDmg = 0;
		dotDuration = 0;
		dotStackMax = 0;
		target = NULL;

		dmg = []() {return 0;};
		move = [this](float deltaTime) { /*do nothing*/ };
		this->SetOrientation(p->GetOrientation());
		this->SetScale(glm::vec3(0.6));
		speed = p->getCurSpeed();
		init(type, upg);


		//displayStats();
		
	}

	Projectile::~Projectile() {
		//delete all sub-projectiles then self
	}
	

	//TODO: make the movement speed a function of the player speed when fired
	void Projectile::init(const std::string type, std::map<std::string, int> upgrades) {
		//damage upgrades are multiplictive 
		if (type.compare("laserBattery") == 0) {
			speed += 10.0f;
			//travels  seconds +10% per level
			ttl = glfwGetTime() + 5 * pow(1.1, upgrades["laserBatteryRangeLevel"]);
			//pierces 0 to 5 targets (+1 per upgrade)
			pierce = upgrades["laserBatteryPierceLevel"];
			//deals 10 damage + 10% per level
			dmg = [this]() {
				return 10 * pow(1.1, upg["laserBatteryDamageLevel"]);
			};
			//moves forward at a speed of 10.0
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};

		}else if (type.compare("pursuer") == 0) {
			speed += 20.0f;
			//travels 20 seconds
			ttl = glfwGetTime() + 20;
			//deals 3 damage + 10% per level
			dmg = [this]() {
				return 5;
			};
			//moves forward at a speed of 20
			move = [this](float deltaTime) {
				//will need access to enemy vector, determine nearest enemy and then go
				std::cout << "target is" <<target <<std::endl;
				std::cout << "time is" << glfwGetTime() - ttl << std::endl;

				if (ttl - glfwGetTime() > 19.6) {
					//go straight initially before locking on
					position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
				}
				else if (target == NULL){ // MAKE SURE ENEMY POINTERS ARE SET TO NULL WHEN DESTROYED
					//target died before reaching
					//find nearest target and set it as target
					//testing with asteroids
					float minD = 100000;
					glm::vec3 view_plane = player->GetOrientationObj()->GetSide();
					for (SceneNode* a : *asteroids) {
						//only want things in front of the player to be chased, this doesn't do that...
						//if (glm::dot(view_plane, (player->GetPosition() - a->GetPosition())) < 0) {
							float d = glm::distance(this->GetPosition(), a->GetPosition());
							if (minD > d) {
								minD = d;
								target = a;
							}
						//}
					}
					//TODO update orientation
					position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
				}
				else {
					//chase target
					//TODO would like to change this to be a more gentle curve
					glm::vec3 direction = glm::normalize(this->GetPosition() - target->GetPosition());
					position_ -= speed * glm::normalize(direction) * deltaTime;
				}
				
			};


		}else if (type.compare("chargeBlast") == 0) {
			speed += 15.0f;
			//travels 12 seconds
			ttl = glfwGetTime() + 12;
			//deals 30 damage + 10% per level
			dmg = [this]() {
				return 30 * pow(1.1, upg["chargeDamageLevel"]);
			};
			//moves forward at a speed of 15.0
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};
			this->Scale(glm::vec3(1.5 * pow(1.1, upgrades["chargeRadiusLevel"])));


		}else if (type.compare("sniperShot") == 0) {
			speed += 50.0f;
			//travels 15 seconds +10% per level
			ttl = glfwGetTime() + 15 * pow(1.1, upgrades["sniperRangeLevel"]);
			//deals (1 damage + 15 per second travelled) + 10% per level
			dmg = [this]() {
				return (1 - 15*(ttl - glfwGetTime() - (15 * pow(1.1, upg["sniperRangeLevel"])))) * pow(1.1, upg["sniperDamageLevel"]);
			};
			//moves forward at a speed of 50.0
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};

					   
		}else if (type.compare("shotgun") == 0) { // multiple of these will be created on the player side
			speed += 40.0f;
			//travels 0.8 seconds
			ttl = glfwGetTime() + 100/speed;
			//deals 5 damage + 10% per level
			dmg = [this]() {
				return 5 * pow(1.1, upg["shotgunDamageLevel"]);
			};
			//moves forward at a speed of 40.0
			this->Rotate(rand() % 20 - 10, glm::vec3(1, 0, 0));
			this->Rotate(rand() % 20 - 10, glm::vec3(0, 1, 0));
			this->Rotate(rand() % 20 - 10, glm::vec3(0, 0, 1));
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
			dotDmg = 5 * pow(1.1, upgrades["naniteTorpedoDamageLevel"]);
			dotDuration = glfwGetTime() + 5 *pow(1.1, upgrades["naniteTorpedoDurationLevel"]);
			dotStackMax = 5 + upgrades["naniteTorpedoStackLevel"];
			//moves forward at a speed of 20.0
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

	//shamelessly stolen from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#i-need-an-equivalent-of-glulookat-how-do-i-orient-an-object-towards-a-point-
	glm::quat Projectile::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
		start = glm::normalize(start);
		dest = glm::normalize(dest);

		float cosTheta = glm::dot(start, dest);
		glm::vec3 rotationAxis;

		if (cosTheta < -1 + 0.001f) {
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
			if (glm::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

			rotationAxis = glm::normalize(rotationAxis);
			return glm::angleAxis(glm::radians(180.0f), rotationAxis);
		}

		rotationAxis = glm::cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return glm::quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);

	}
}
