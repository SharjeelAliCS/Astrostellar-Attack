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


	Player::Player(void) : SceneNode() {

	}

	Player::Player(const std::string name, const Resource *geometry, const Resource *material, Transformation* model) : SceneNode(name, geometry, material, model) {
	}



	Player::~Player() {
	}
	void Player::Fire() {
		/*
		Projectile* missile = new Projectile("missile", model);
		missile->CreateCube(0.5,0.5,0.5);
		missile->SetPosition(position_);
		missile->SetOrgPos(position_);
		missile->SetOrientation(orientation_.GetOrientation());
		missile->SetMovementSpeed(500);
		missile->SetRange(500);

		missiles.push_back(missile);
		*/
		
	}

	void Player::Draw(Camera *camera) {

		for (std::vector<Projectile*>::iterator it = missiles.begin(); it != missiles.end(); ++it) {
			(*it)->Draw(camera);
		}
		
		SceneNode::Draw(camera);

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

	void Player::Update(double deltaTime) {
		//update the missiles and check if they exist or not. 
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



	}
}
