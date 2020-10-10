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

#include "camera.h"

namespace game {

Camera::Camera(void){
}


Camera::~Camera(){
}


glm::vec3 Camera::GetPosition(void) const {

    return position_;
}


glm::quat Camera::GetOrientation(void) const {

	return orientation_.GetOrientation();
}


void Camera::SetPosition(glm::vec3 position){

    position_ = position;
}


void Camera::SetOrientation(glm::quat orientation){

	orientation_.SetOrientation(orientation);
}


void Camera::Translate(glm::vec3 trans){

    position_ += trans;
}


void Camera::Rotate(glm::quat rot){

	orientation_.Rotate(rot);
}


glm::vec3 Camera::GetForward(void) const {

	return orientation_.GetForward();
}


glm::vec3 Camera::GetSide(void) const {

	return orientation_.GetSide();
}


glm::vec3 Camera::GetUp(void) const {
	return orientation_.GetUp();
}


void Camera::Pitch(float angle){

	orientation_.Pitch(angle);
}

void Camera::Yaw(float angle){

	orientation_.Yaw(angle);
}

void Camera::Roll(float angle){

	orientation_.Roll(angle);
}



void Camera::SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up){
	orientation_.SetView(position, look_at, up);

}


void Camera::SetProjection(GLfloat fov, GLfloat near, GLfloat far, GLfloat w, GLfloat h){

    // Set projection based on field-of-view
    float top = tan((fov/2.0)*(glm::pi<float>()/180.0))*near;
    float right = top * w/h;
    projection_matrix_ = glm::frustum(-right, right, -top, top, near, far);
}


void Camera::SetupShader(GLuint program){

    // Update view matrix
    SetupViewMatrix();

     // Set view matrix in shader
    GLint view_mat = glGetUniformLocation(program, "view_mat");
    glUniformMatrix4fv(view_mat, 1, GL_FALSE, glm::value_ptr(view_matrix_));
    
    // Set projection matrix in shader
    GLint projection_mat = glGetUniformLocation(program, "projection_mat");
    glUniformMatrix4fv(projection_mat, 1, GL_FALSE, glm::value_ptr(projection_matrix_));
}


void Camera::SetupViewMatrix(void){

    // Get current vectors of coordinate system
    // [side, up, forward]
    // See slide in "Camera control" for details
    glm::vec3 current_forward = orientation_.GetOrientation() * orientation_.GetInitForward();
    glm::vec3 current_side = orientation_.GetOrientation() * orientation_.GetInitSide();
    glm::vec3 current_up = glm::cross(current_forward, current_side);
    current_up = glm::normalize(current_up);

	view_matrix_ = glm::lookAt(position_, position_ - current_forward, current_up);

	
}

} // namespace game
