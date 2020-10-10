/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */
#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "rotation.h"

namespace game {

	Rotation::Rotation(void) {
	}


	Rotation::~Rotation() {
	}


	glm::quat Rotation::GetOrientation(void) const {

		return orientation_;
	}

	void Rotation::SetOrientation(glm::quat orientation) {

		orientation_ = orientation;
	}

	void Rotation::Rotate(glm::quat rot) {

		orientation_ = rot * orientation_;
		orientation_ = glm::normalize(orientation_);
	}


	glm::vec3 Rotation::GetForward(void) const {

		glm::vec3 current_forward = orientation_ * forward_;
		//std::cout << forward_.x << "," << forward_.y << "," << forward_.z  << std::endl;
		return -current_forward; // forward in space is opposite to camera's coordinate axis
	}


	glm::vec3 Rotation::GetInitForward(void) const {
		return forward_;
	}
	glm::vec3 Rotation::GetInitSide(void) const {
		return side_;
	}
	glm::vec3 Rotation::GetSide(void) const {

		glm::vec3 current_side = orientation_ * side_;
		return current_side;
	}


	glm::vec3 Rotation::GetUp(void) const {

		glm::vec3 current_forward = orientation_ * forward_;
		glm::vec3 current_side = orientation_ * side_;
		glm::vec3 current_up = glm::cross(current_forward, current_side);
		current_up = glm::normalize(current_up);
		return current_up;
	}


	void Rotation::Pitch(float angle) {

		glm::quat rotation = glm::angleAxis(angle, GetSide());
		orientation_ = rotation * orientation_;
		orientation_ = glm::normalize(orientation_);
	}


	void Rotation::Yaw(float angle) {

		glm::quat rotation = glm::angleAxis(angle, GetUp());
		orientation_ = rotation * orientation_;
		orientation_ = glm::normalize(orientation_);
	}

	void Rotation::Roll(float angle) {

		glm::quat rotation = glm::angleAxis(angle, GetForward());
		orientation_ = rotation * orientation_;
		orientation_ = glm::normalize(orientation_);
	}

	void Rotation::SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up) {
		// Store initial forward and side vectors
		// See slide in "Camera control" for details
		forward_ = look_at - position;
		forward_ = glm::normalize(forward_);
		side_ = glm::cross(up, forward_);
		side_ = glm::normalize(side_);

		// Reset orientation and position of camera
		orientation_ = glm::quat();
	}


} // namespace game
