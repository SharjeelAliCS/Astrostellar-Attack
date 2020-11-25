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

#include "Orientation.h"

namespace game {

	Orientation::Orientation(void) {
	}


	Orientation::~Orientation() {
	}


	glm::quat Orientation::GetOrientation(void) const {

		return orientation_;
	}

	void Orientation::SetOrientation(glm::quat orientation) {

		orientation_ = orientation;
	}

	void Orientation::Rotate(glm::quat rot) {

		orientation_ = rot * orientation_;
		orientation_ = glm::normalize(orientation_);
	}


	glm::vec3 Orientation::GetForward(void) const {

		glm::vec3 current_forward = orientation_ * forward_;
		//std::cout << forward_.x << "," << forward_.y << "," << forward_.z  << std::endl;
		return -current_forward; // forward in space is opposite to camera's coordinate axis
	}


	glm::vec3 Orientation::GetInitForward(void) const {
		return forward_;
	}
	glm::vec3 Orientation::GetInitSide(void) const {
		return side_;
	}
	glm::vec3 Orientation::GetSide(void) const {

		glm::vec3 current_side = orientation_ * side_;
		return current_side;
	}


	glm::vec3 Orientation::GetUp(void) const {

		glm::vec3 current_forward = orientation_ * forward_;
		glm::vec3 current_side = orientation_ * side_;
		glm::vec3 current_up = glm::cross(current_forward, current_side);
		current_up = glm::normalize(current_up);
		return current_up;
	}


	void Orientation::Pitch(float angle) {

		glm::quat Orientation = glm::angleAxis(angle, GetSide());
		orientation_ = Orientation * orientation_;
		orientation_ = glm::normalize(orientation_);
	}


	void Orientation::Yaw(float angle) {

		glm::quat Orientation = glm::angleAxis(angle, GetUp());
		orientation_ = Orientation * orientation_;
		orientation_ = glm::normalize(orientation_);
	}

	void Orientation::Roll(float angle) {

		glm::quat Orientation = glm::angleAxis(angle, GetForward());
		orientation_ = Orientation * orientation_;
		orientation_ = glm::normalize(orientation_);
	}

	//taken from https://gamedev.net/forums/topic/493428-rotate-an-object-to-face-a-target-in-3d-space/4218536/
	void Orientation::RotateTowards(glm::vec3 cur_pos, glm::vec3 target_pos) {
		glm::vec3 forw = GetForward();
		glm::vec3 distance = target_pos - cur_pos;
		glm::vec3 dis = glm::normalize(distance);
		glm::vec3 axis = glm::cross(forw, dis);
		axis = glm::normalize(axis);

		float cosa = glm::dot(forw, dis);
		float angle = acos(cosa);
		glm::quat qu = glm::angleAxis(angle, axis);


		Rotate(qu);
	}
	void Orientation::SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up) {
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
