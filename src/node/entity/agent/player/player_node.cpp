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

	Player::Player(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : AgentNode(name, geometry, material, texture,normal) {

		max_shields_ = 100;
		shields_ = max_shields_;
		boosted_ = 0;
		movement_speed = 20;
		boost_speed_ = 4*movement_speed;
		first_person_ = true;
		boost_duration_ = 20;
		boost_duration_left_ = boost_duration_;

		shield_recharge_speed_ = 3;
		time_since_damage_ = 0;
		shield_recharge_delay_ = 10;
		nuclear_buildup_duration_ = 25;
		nuclear_buildup_left_ = nuclear_buildup_duration_;

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
		upgrades["shotgun_NumShots_Level"] = 5; // how many are fired, +6 per level

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

	float Player::GetShieldPercent(void) const {
		return shields_ / max_shields_;
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
			audio_->playAgain("missileShot");
			Projectile* missile;
			int numShots = 1;
			if (weapon.compare("shotgun") == 0) {
				numShots = 15 + 6 * upgrades["shotgun_NumShots_Level"];

			}
			for (int i = 0; i < numShots; i++) {
				missile = new Projectile("missile", weapon, upgrades, proj_rsc_->geom, proj_rsc_->mat, proj_rsc_->tex);
				missile->SetAsteroids(asteroids);
				missile->SetComets(comets);
				missile->SetEnemies(enemies);
				missile->SetColor(glm::vec3(0.19,0.78,1));
				missile->SetOrientation(GetOrientation());
				if (!first_person_) {
					glm::vec3 objPos = (float)300 * c_->GetForward() + c_->GetPosition();
					//missile->GetOrientationObj()->SetView(objPos, position_, glm::vec3(0, 1, 0));
					missile->GetOrientationObj()->FaceTowards(position_, objPos);

				}

				missile->setSpeed(this->getCurSpeed()*3);
				missile->init();
				missile->SetPosition(position_);
				missiles.push_back(missile);
			}
			lastShotTime = glfwGetTime();
		}	
	}

	void Player::Draw(Camera *camera) {
		
		AgentNode::Draw(camera);

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

	void Player::damage(float dmg,bool health) {
		if (shields_ > 0) {
			shields_ -= dmg;
		}
		else if (health_ > 0 && health) {
			health_ -= dmg;
		}
		time_since_damage_ = 0;
	}
	bool Player::Collision() {
		//check for collisions with player, set collide to true/false depending/ 
		bool collide = false;
		
		return collide;
	}

	std::string Player::GetCurrentWeapon(void) const {
		return projectileTypes[projType];
	}

	float Player::getBoostPercent(void) {
		return (boost_duration_- boost_duration_left_) / boost_duration_;
	}
	float Player::getNuclearOverloadPercent(void) {
		return (nuclear_buildup_duration_ - nuclear_buildup_left_) / nuclear_buildup_duration_;
	}
	bool Player::NuclearOverload(void) {
		return (boosted_ && boost_duration_left_ <= 0) || 
			(nuclear_buildup_left_<nuclear_buildup_duration_);
	}
	void Player::Update(float deltaTime) {

		if (boosted_) {
			boost_duration_left_ -= deltaTime;
			if (boost_duration_left_ <= 0) {
				boost_duration_left_ = 0;
				damage(deltaTime * 3, false);
				nuclear_buildup_left_ -= deltaTime;
			}
		}
		else {
			float time = glfwGetTime();
			if (boost_duration_left_ < boost_duration_) {
				boost_duration_left_ += deltaTime*2;
			}
			else {
				boost_duration_left_ = boost_duration_;
			}
			if (nuclear_buildup_left_ < nuclear_buildup_duration_) {
				nuclear_buildup_left_ += deltaTime * 10;
			}
			else {
				nuclear_buildup_left_ = nuclear_buildup_duration_;
			}
		}

		time_since_damage_ += deltaTime;
		if (time_since_damage_ > shield_recharge_delay_ && shields_ < max_shields_) {
			shields_ += deltaTime * shield_recharge_speed_*10;
		}
		Translate(c_->GetForward() * getCurSpeed() *deltaTime);
		c_->Translate(c_->GetForward() * getCurSpeed() *deltaTime);
		Collision();

		//update the missiles and check if they exist or not. 
		//std::cout << "mypos is " << position_.x << " "<< position_.y << " "<< position_.z << std::endl;
		
		AgentNode::Update(deltaTime);

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

	void Player::setAsteroids(std::vector<AsteroidNode*>* a) {
		asteroids = a;
	}void Player::setComets(std::vector<CometNode*>* a) {
		comets = a;
	}
	void Player::setEnemies(std::vector<Enemy*>* e) {
		enemies = e;
	}
	void Player::setDeathAnimations(std::vector<ParticleNode*>* p) {
		death_animations_ = p;
	}
}
