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



	Projectile::Projectile(const std::string name, const std::string t, std::map<std::string, int> upgrades, std::map<std::string, float> weaponStats, bool boosted,
		const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal)
		: Entity(name, geometry, material, texture,normal) {
		//default all to zero
		type = t;
		upg = upgrades;
		stats = weaponStats;
		ttl    = 0;
		pierce = 0;
		dotDmg = 0;
		dotDuration = 0;
		dotStackMax = 0;
		target = NULL;
		health_ = 10000000000;
		speed = 450;
		this->boosted = boosted;
		dmg = []() {return 0;};
		move = [this](float deltaTime) { /*do nothing*/ };
		// this->SetScale(glm::vec3(0.6));
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
			speed -= 350;
			//travels 5 seconds
			ttl = glfwGetTime() + 10;
			pierce = 0;
			dmg = [this]() {
				return 5 ;
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
		}else if (type.compare("boss") == 0) {
			speed -= 400;
			//travels 5 seconds
			ttl = glfwGetTime() + 10;
			pierce = 0;
			dmg = [this]() {
				return 10;
			};
			move = [this](float deltaTime) { //minor player tracking
				orientation_->FaceTowards(position_, player_->GetPosition(), true);
				float rot_speed = deltaTime;
				if (player_->GetBoosted()) {
					rot_speed *= 2;
				}
				else {
					rot_speed *= 0.2;
				}
				orientation_->RotateTowards(rot_speed);
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};
		}
		else if (type.compare("laser_Battery") == 0) {
			//travels 5 seconds +10% per level
			ttl = glfwGetTime() + stats["laser_Battery_baseTTL"] * pow(1.1, upg["laser_Battery_Range_Level"]);
			//pierces 0 to 5 targets (+1 per upgrade)
			pierce = upg["laser_Battery_Pierce_Level"];
			//deals 10 damage + 10% per level
			dmg = [this]() {
				return stats["laser_Battery_baseDmg"] * pow(1.1, upg["laser_Battery_Damage_Level"]) * max(1, boosted * pow(1.25, 1+upg["evasive_Maneuvers_Level"]));
			};
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};

		}else if (type.compare("pursuer") == 0) {
			speed -= 100.0f;
			//travels 20 seconds
			ttl = glfwGetTime() + stats["pursuer_baseTTL"];
			dmg = [this]() {
				return stats["pursuer_baseDmg"] * max(1, boosted * pow(1.25, 1+upg["evasive_Maneuvers_Level"]));
			};
			move = [this](float deltaTime) {
				//will need access to enemy vector, determine nearest enemy and then go
				
				if (ttl - glfwGetTime() > stats["pursuer_baseTTL"]-0.2) {
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
					orientation_->RotateTowards(deltaTime*5);
					position_ += speed * orientation_->GetForward()*deltaTime;
				}			
			};

		}else if (type.compare("charge_Blast") == 0) {
			//travels 12 seconds
			int chargePierce = 50;
			pierce = chargePierce;
			ttl = glfwGetTime() + stats["charge_Blast_baseTTL"];
			//deals 100 damage + 10% per level
			dmg = [this, chargePierce]() {
				if (pierce == chargePierce-1) {
					this->Scale(glm::vec3(5,6,5)); //kaboom
					ttl = glfwGetTime() + 0.2;
				}
				return stats["charge_Blast_baseDmg"] * pow(1.1, upg["charge_Blast_Damage_Level"]) * max(1, boosted * pow(1.25, 1+upg["evasive_Maneuvers_Level"]));
			};

			this->Scale(glm::vec3(5));
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
				if(this->GetScale().x<70+10*upg["charge_Blast_Radius_Level"]){
					this->Scale(glm::vec3(max(1.025, pow(1.01, 1+upg["charge_Blast_Radius_Level"]))));
				}
			};
			


		}else if (type.compare("sniper_Shot") == 0) {
			speed *= 3.5;
			this->Scale(glm::vec3(3));
			//travels 15 seconds +10% per level
			ttl = glfwGetTime() + stats["sniper_Shot_baseTTL"] * pow(1.1, upg["sniper_Shot_Range_Level"]);
			//deals (1 damage + 60 per second travelled) + 10% per level
			int damageFactor = stats["sniper_Shot_baseDmg"];
			dmg = [this, damageFactor]() {
				return (1 - damageFactor *(ttl - glfwGetTime() - (damageFactor * pow(1.1, upg["sniper_Shot_Range_Level"])))) * pow(1.1, upg["sniper_Shot_Damage_Level"]) * max(1, boosted * pow(1.25, 1+upg["evasive_Maneuvers_Level"]));
			};
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};

					   
		}else if (type.compare("shotgun") == 0) { // multiple of these will be created on the player side
			//travels 0.7 seconds
			ttl = glfwGetTime() + stats["shotgun_baseTTL"];
			//deals 5 damage + 10% per level
			dmg = [this]() {
				return stats["shotgun_baseDmg"] * pow(1.1, upg["shotgun_Damage_Level"]) * max(1, boosted * pow(1.25, 1+upg["evasive_Maneuvers_Level"]));
			};
			//random spread
			this->Rotate(rand() % 14 - 7, glm::vec3(1, 0, 0));
			this->Rotate(rand() % 14 - 7, glm::vec3(0, 1, 0));
			this->Rotate(rand() % 14 - 7, glm::vec3(0, 0, 1));
			move = [this](float deltaTime) {
				position_ -= speed * glm::normalize(-orientation_->GetForward()) * deltaTime;
			};
		
					   
		}else if (type.compare("nanite_Torpedo") == 0) { // multiple of these will be created on the player side
			
			speed += 20;
			//travels 10 seconds
			ttl = glfwGetTime() + stats["nanite_Torpedo_baseTTL"];
			//deals 0 impact damage
			dmg = [this]() {
				return 0;
			};
			//5dps per stack up to 5 stacks, lasts for 5 seconds
			dotDmg = stats["nanite_Torpedo_baseDmg"] * pow(1.1, upg["nanite_Torpedo_Damage_Level"]) * max(1, boosted * pow(1.25, 1+upg["evasive_Maneuvers_Level"]));
			dotDuration = glfwGetTime() + 5 *pow(1.1, upg["nanite_Torpedo_Duration_Level"]);
			dotStackMax = 5 + upg["nanite_Torpedo_Stack_Level"];
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
