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
	}

	Entity::~Entity() {
	}

	bool Entity::Exists(void) const {
		return exists_;
	}

	void Entity::SetMovementSpeed(float s) {
		movement_speed = s;
	}

	ParticleNode* Entity::GetParticle(void) {
		return particles_;
	}
	void Entity::SetHealth(float h) {
		health_ = h;
	}
	float Entity::GetHealth(void) const {
		return health_;
	}

	void Entity::RotateOverTime(float rotation_speed, glm::vec3 rotation_normal) {
		rotation_normal_ = rotation_normal;
		rotation_speed_ = rotation_speed;
	}

	void Entity::Update(float deltaTime) {
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
	void Entity::SetParticleSystem(ParticleNode* particles) {
		particles_ = particles;
	}

	void Entity::Draw(Camera *camera) {
		
		if (particles_ != NULL) {
			int a = 4;
			particles_->Draw(camera);
		}
		SceneNode::Draw(camera);
	}


}
