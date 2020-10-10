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

#include "projectileobj.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	ProjectileObj::ProjectileObj(void) : GameObj() {
		
	}

	ProjectileObj::ProjectileObj(GLuint theshader, Transformation* m) : GameObj(theshader, m) {

	}



	ProjectileObj::~ProjectileObj() {
	}

	void ProjectileObj::SetRange(float range) {
		range_ = range;
	}

	void ProjectileObj::Update(double deltaTime) {
		float a = (float)deltaTime;
		//Constantly move the projectile using the speed and delta time. 
		position_ -= movement_speed * glm::normalize(-orientation_.GetForward())*(float)deltaTime;
		
		//If it exceeds range, set exists to false
		if (glm::distance(position_, org_pos_) >= range_) {
			exists_ = false;
		}
	}

}
