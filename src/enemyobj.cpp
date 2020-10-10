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

#include "enemyobj.h"


namespace game {


	EnemyObj::EnemyObj(void) : GameObj() {

	}

	EnemyObj::EnemyObj(GLuint theshader, Transformation* m) : GameObj(theshader, m) {
		
	}

	EnemyObj::EnemyObj(GLuint theshader, Transformation* m, glm::vec3 forward) : GameObj(theshader, m, forward) {
	}

	EnemyObj::~EnemyObj() {

	}

	void EnemyObj::CreatePlanet(void) {

		//Create the basic body sphere
		Geom* body = new Geom(program, model);
		body->CreateSphere(0.5);

		//Create the basic ring sphere. 
		Geom* ring = new Geom(program, model);
		ring->CreateTorus(1, 0.2, 20, 20);
		ring->SetOrientation(90, glm::vec3(1, 0, 0));
		ring->SetScale(glm::vec3(1, 1, 0.2));
		ring->RotateOverTime(90, glm::vec3(0, 1, 0));
		geom.insert({ "body", body });
		geom.insert({ "ring", ring });
	}

	

}
