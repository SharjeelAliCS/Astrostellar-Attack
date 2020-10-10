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

#include "playerobj.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	PlayerObj::PlayerObj(void) : GameObj() {

	}

	PlayerObj::PlayerObj(GLuint theshader, Transformation* m) : GameObj(theshader, m) {
		CreateGeom();
	}



	PlayerObj::~PlayerObj() {
	}

	void PlayerObj::CreateGeom(void) {
		//Creates the basic spaceship shape. Add one cube, 2 torus on both sides. 
		Geom* body = new Geom(program, model);
		body->CreateCube(1.0, 0.5, 2.0);
		
		Geom* ringLeft = new Geom(program, model);
		ringLeft->CreateTorus(1, 0.1, 20, 20);
		ringLeft->SetPosition(glm::vec3(-1,0,0));
		ringLeft->SetOrientation(90, glm::vec3(1, 0, 0));

		Geom* ringRight = new Geom(program, model);
		ringRight->CreateTorus(1, 0.1, 20, 20);
		ringRight->SetPosition(glm::vec3(1, 0, 0));
		ringRight->SetOrientation(90, glm::vec3(1, 0, 0));

		geom.insert({ "body", body });
		geom.insert({ "left", ringLeft });
		geom.insert({ "right", ringRight });
	}
	glm::quat PlayerObj::RotLagBehind(float pitch, float yaw) {
		glm::quat rotation = glm::angleAxis(yaw, orientation_.GetUp());
		glm::quat ori = rotation * orientation_.GetOrientation();
		ori = glm::normalize(ori);

		rotation = glm::angleAxis(pitch, orientation_.GetSide());
		ori = rotation * ori;
		ori = glm::normalize(ori);

		return ori;

	}

	void PlayerObj::Fire() {
		
		ProjectileObj* missile = new ProjectileObj(program, model);
		missile->CreateCube(0.5,0.5,0.5);
		missile->SetPosition(position_);
		missile->SetOrgPos(position_);
		missile->SetOrientation(orientation_.GetOrientation());
		missile->SetMovementSpeed(500);
		missile->SetRange(500);

		missiles.push_back(missile);
		
	}

	void PlayerObj::Draw(Camera *camera) {

		for (std::vector<ProjectileObj*>::iterator it = missiles.begin(); it != missiles.end(); ++it) {
			(*it)->Draw(camera);
		}
		
		GameObj::Draw(camera);

	}
	bool PlayerObj::Collision(std::vector<EnemyObj*>* enemies) {
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

	void PlayerObj::Update(double deltaTime) {
		//update the missiles and check if they exist or not. 
		for (std::vector<ProjectileObj*>::iterator it = missiles.begin(); it != missiles.end();) {
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
