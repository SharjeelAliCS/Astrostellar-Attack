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




 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {

	Player::Player(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : Entity(name, geometry, material, texture,normal) {

		geo = geometry;
		mat = material;
		tex = texture;

		max_shields_ = 100;
		shields_ = max_shields_;
		boosted_ = 0;
		movement_speed = 20;
		boost_speed_ = 4*movement_speed;
		//resman_ refuses to be global, likely because I'm being dumbass.

		//Projectile::Projectile(const std::string name, const std::string type, const std::map<std::string, int> upgrades, const Resource * geometry,
			//const Resource * material, const Resource * texture);
		/*
		 std::string object_name = "ship";
		 std::string material_name = "TextureShader";
		 std::string texture_name = "shipTexture";

		 Resource* geom = resman_.GetResource(object_name);
		 if (!geom) {
			// throw((std::string("Could not find resource \"") + object_name + std::string("\"")));
		 }

		 Resource* mat = resman_.GetResource(material_name);
		 if (!mat) {
			// throw((std::string("Could not find resource \"") + material_name + std::string("\"")));
		 }

		 Resource* tex = NULL;
		 if (texture_name != "") {
			 tex = resman_.GetResource(texture_name);
			 if (!tex) {
				// throw((std::string("Could not find resource \"") + material_name + std::string("\"")));
			 }
		 }
		 */

		for (int i = 0; i < 6; i++) {
			unlockedWeapons[i] = true; // usually false, true for testing
			rof[i] = 1.0f; //will play around with individiual rof later 
		}
		 
		unlockedWeapons[0] = true; //laser battery always unlocked

		upgrades["laserBatteryRangeLevel"] = 0;
		upgrades["laserBatteryPierceLevel"] = 0;
		upgrades["laserBatteryDamageLevel"] = 0;	 
		
		upgrades["pursuerROFLevel"] = 5;
		 
		upgrades["chargeRadiusLevel"] = 0;  //size of shot
		upgrades["chargeDamageLevel"] = 0;  
		upgrades["chargeDurationLevel"] = 0; //how long to charge up the shot
		 
		upgrades["sniperDamageLevel"] = 0;
		upgrades["sniperRangeLevel"] = 0;

		upgrades["shotgunDamageLevel"] = 0;
		upgrades["shotgunNumLevel"] = 5; // how many are fired, +6 per level

		upgrades["naniteTorpedoDamageLevel"] = 0;   //dot dmg
		upgrades["naniteTorpedoDurationLevel"] = 0; //dot duration
		upgrades["naniteTorpedoStackLevel"] = 0;    //max stacks of dot

		projType = 0;


		 //Projectile* missile = new Projectile("missile", "laserBattery", upgrades, geom, mat, tex);
		 //Projectile* missile = new Projectile("missile", "laserBattery", upgrades, geometry, material, texture);
		 //Projectile* missile = new Projectile("missile", "sniperShot", upgrades, geometry, material, texture);
		 //missiles.push_back(missile);

		 //*/
	}



	Player::~Player() {
	}

	void Player::SetBoosted(int i) {
		boosted_ = i;
	}
	int Player::GetBoosted(void) const {
		return boosted_;
	}
	float Player::GetBoostSpeed(void) const {
		return boost_speed_;
	}
	void Player::SetShields(float h) {
		shields_ = h;
	}
	float Player::GetShields(void) const {
		return shields_;
	}


	//tab to change weapon
	void Player::nextWeapon() {
		std::cout << "weapon-change";
		projType = (projType + 1) % numWeapons;
		if (!unlockedWeapons[projType]) {
			nextWeapon();
		}
		else {
			std::cout << "weapon is: " << projectileTypes[projType];
		}
	}


	void Player::Fire() {
		std::string weapon = projectileTypes[projType];

		double shotTime = lastShotTime + rof[projType] * ((projectileTypes[projType].compare("pursuer") == 0) ? (2 - pow(1.1, upgrades["pursuerROFLevel"])) : 1);
		if (glfwGetTime() > shotTime) {
			Projectile* missile;
			int numShots = 1;
			if (weapon.compare("shotgun") == 0) {
				numShots = 15 + 6 * upgrades["shotgunNumLevel"];

			}
			for (int i = 0; i < numShots; i++) {
				missile = new Projectile("missile", weapon, upgrades, this, asteroids, enemies, geo, mat, tex);
				missile->SetPosition(position_);
				missiles.push_back(missile);
			}
			lastShotTime = glfwGetTime();
		}	
	}

	void Player::Draw(Camera *camera) {

		for (std::vector<Projectile*>::iterator it = missiles.begin(); it != missiles.end(); ++it) {
			(*it)->Draw(camera);
		}
		if (!draw_)return;
		Entity::Draw(camera);

	}
	glm::quat Player::RotLagBehind(float pitch, float yaw) {
		glm::quat rotation = glm::angleAxis(yaw, orientation_->GetUp());
		glm::quat ori = rotation * orientation_->GetOrientation();
		ori = glm::normalize(ori);

		rotation = glm::angleAxis(pitch, orientation_->GetSide());
		ori = rotation * ori;
		ori = glm::normalize(ori);

		return ori;

	}

	bool Player::Collision(std::vector<Enemy*>* enemies) {
		//check for collisions with player, set collide to true/false depending/ 
		bool collide = false;
		for (auto en = enemies->begin(); en != enemies->end(); ) {
			if ((*en)->Hit(position_, glm::length((*en)->GetScale())*1.2)) {
				en = enemies->erase(en);
				collide = true;
			}
			else {
				++en;
			}
		}

		//Check for collisions with missiles and remove both fromo their vectors. 
		for (auto it = missiles.begin(); it != missiles.end(); ) {
			bool removed = false;

			for (auto en = enemies->begin(); en != enemies->end(); ) {
				if ((*en)->Hit((*it)->GetPosition(), glm::length((*en)->GetScale())*1.2)) {
					it = missiles.erase(it);
					enemies->erase(en);
					removed = true;
					break;
					
				}
				else {
					++en;
				}
			}
			if (!removed) {
				++it;
			}
			
			
		}
		return collide;
	}

	void Player::Update(float deltaTime) {
		Translate(c_->GetForward() * getCurSpeed() *deltaTime);
		c_->Translate(c_->GetForward() * getCurSpeed() *deltaTime);

		//update the missiles and check if they exist or not. 
		//std::cout << "mypos is " << position_.x << " "<< position_.y << " "<< position_.z << std::endl;
		for (std::vector<Projectile*>::iterator it = missiles.begin(); it != missiles.end();) {
			(*it)->Update(deltaTime);
			if ((*it)->Exists()) {
				++it;
				
			}
			else {
				int a;
				it = missiles.erase(it);
			}

		}
	
		Entity::Update(deltaTime);
		if (particles_ != NULL) {
			//particles_->Rotate(-90, glm::vec3(0, 1, 0));
		}

	}


	float Player::getCurSpeed() const {
		if (boosted_) {
			return boost_speed_+movement_speed;
		}
		else {
			return movement_speed;
		}
	}

	void Player::setCam(Camera* c) {
		c_ = c;
	}

	void Player::setAsteroids(std::vector<SceneNode*>* a) {
		asteroids = a;
	}
	void Player::setEnemies(std::vector<Enemy*>* e) {
		enemies = e;
	}
}
