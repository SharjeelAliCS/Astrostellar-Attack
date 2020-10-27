/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 2: A multi-component model
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates camera model with the tori.
 * For further info, please read the README file provided.
 *
 */

#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "hud_node.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	HUDNode::HUDNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture) {
		int a = 5;
		org_pos_ = position_;
	}



	HUDNode::~HUDNode() {
	}


	void HUDNode::Draw(Camera *camera) {
		// Select proper material (shader program)
		glUseProgram(material_);
		glDisable(GL_DEPTH_TEST);  
		// Set geometry to draw
		glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

		// Set globals for camera
		camera->SetupShader(material_);

		// Set world matrix and other shader input variables
		SetupShader(material_,camera);

		// Draw geometry
		if (mode_ == GL_POINTS) {
			glDrawArrays(mode_, 0, size_);
		}
		else {
			glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
		}

		for (std::vector<SceneNode*>::iterator it = children_.begin(); it != children_.end(); ++it) {
			(*it)->Draw(camera);
		}
		glEnable(GL_DEPTH_TEST);
	}
	
	glm::mat4 HUDNode::CalculateFinalTransformation(Camera* camera) {

		float zoom = camera->GetZoom();
		// World transformation
		glm::mat4 cameraOrientation = glm::mat4_cast(camera->GetOrientation());
		glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.0), camera->GetPosition());
		glm::mat4 cameraZoom = glm::translate(glm::mat4(1.0), camera->GetZoomPos());
		glm::mat4 cameraScale = glm::scale(glm::mat4(1.0f), glm::vec3(zoom));
		glm::mat4 cameraTransf = cameraTranslation * cameraOrientation*cameraZoom*cameraScale;

		glm::mat4 Orientation = glm::mat4_cast(orientation_->GetOrientation());
		glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);
		glm::mat4 trans_joint = glm::translate(glm::mat4(1.0), joint_);
		glm::mat4 trans_joint_inv = glm::translate(glm::mat4(1.0), -joint_);

		glm::mat4 orbit = trans_joint_inv * Orientation * trans_joint;
		glm::mat4 transf = cameraTransf* translation*orbit;

		for (std::vector<SceneNode*>::iterator it = children_.begin(); it != children_.end(); ++it) {
			(*it)->SetParentTransform(transf);
		}

		return transf;
	}

	void HUDNode::Update(float deltaTime) {
		SceneNode::Update(deltaTime);
	}



}

