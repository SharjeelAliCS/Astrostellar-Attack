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

#include "entity_node.h"

namespace game {

	Entity::Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : SceneNode(name, geometry, material, texture,normal) {
		max_health_ = 100;
		health_ = max_health_;
		particles_ = NULL;
		see_health_duration_ = 2;
		see_health_left_ = 0;
	}

	Entity::~Entity() {
	}

	bool Entity::Exists(void) const {
		return exists_;
	}

	bool Entity::SeeHealth(bool see) {
		if (see) {
			see_health_left_ = see_health_duration_;
		}
		return see_health_left_ > 0;
	}
	void Entity::SetMovementSpeed(float s) {
		movement_speed = s;
	}
	bool Entity::damage(float dmg, bool health) {
		health_ -= dmg;
		if (health_ < 0) {
			health_ = 0;
			exists_ = false;
			return true;
		}
		return false;
	}
	ParticleNode* Entity::GetParticle(void) {
		return particles_;
	}
	void Entity::SetHealth(float h) {
		health_ = h;
	}
	void Entity::SetMaxHealth(float h) {
		health_ = h;
		max_health_ = health_;
	}
	float Entity::GetHealth(void) const {
		return health_;
	}

	float Entity::getHealthPercent(void) const {
		return health_ / max_health_;
	}
	void Entity::RotateOverTime(float rotation_speed, glm::vec3 rotation_normal) {
		rotation_normal_ = rotation_normal;
		rotation_speed_ = rotation_speed;
	}

	void Entity::Update(float deltaTime) {
		see_health_left_ -= deltaTime;
		if (particles_ != NULL) {
			//particles_->Rotate(deltaTime * 30, glm::vec3(0,0,1));
		}
	}

	glm::mat4 Entity::CalculateFinalTransformation(Camera* camera) {
		// World transformation
		glm::mat4 transf = SceneNode::CalculateFinalTransformation(camera);
		if (particles_ != NULL) {
			particles_->SetParentTransform(transf);
		}
		return transf;

	}
	float Entity::GetDamage(void)  {
		return 20;
	}
	void Entity::SetParticleSystem(ParticleNode* particles) {
		particles_ = particles;
	}


}
