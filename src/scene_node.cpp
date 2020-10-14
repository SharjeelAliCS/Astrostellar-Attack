#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "scene_node.h"

namespace game {

SceneNode::SceneNode(void) {

}	

SceneNode::SceneNode(const std::string name, const Resource *geometry, const Resource *material, Transformation* model){

    // Set name of scene node
    name_ = name;
	model_ = model;
    // Set geometry
    if (geometry->GetType() == PointSet){
        mode_ = GL_POINTS;
    } else if (geometry->GetType() == Mesh){
        mode_ = GL_TRIANGLES;
    } else {
        throw(std::invalid_argument(std::string("Invalid type of geometry")));
    }

    array_buffer_ = geometry->GetArrayBuffer();
    element_array_buffer_ = geometry->GetElementArrayBuffer();
    size_ = geometry->GetSize();

    // Set material (shader program)
    if (material->GetType() != Material){
        throw(std::invalid_argument(std::string("Invalid type of material")));
    }

    material_ = material->GetResource();

    // Other attributes
    scale_ = glm::vec3(1.0, 1.0, 1.0);
}


SceneNode::~SceneNode(){
}


const std::string SceneNode::GetName(void) const {

    return name_;
}


glm::vec3 SceneNode::GetForward(void) const {

	return orientation_.GetForward();
}


glm::vec3 SceneNode::GetSide(void) const {

	return orientation_.GetSide();
}


glm::vec3 SceneNode::GetUp(void) const {
	return orientation_.GetUp();
}


void SceneNode::Pitch(float angle) {

	orientation_.Pitch(angle);
}

void SceneNode::Yaw(float angle) {

	orientation_.Yaw(angle);
}

void SceneNode::Roll(float angle) {

	orientation_.Roll(angle);
}

glm::vec3 SceneNode::GetOrgPos(void) const {

	return org_pos_;
}

glm::vec3 SceneNode::GetPosition(void) const {

	return position_;
}


glm::quat SceneNode::GetOrientation(void) const {

	return orientation_.GetOrientation();
}


glm::vec3 SceneNode::GetScale(void) const {

	return scale_;
}

bool SceneNode::Exists(void) const {
	return exists_;
}


void SceneNode::SetOrgPos(glm::vec3 position) {

	org_pos_ = position;
}


void SceneNode::SetPosition(glm::vec3 position) {

	position_ = position;
}

void SceneNode::SetOrientation(float angle, glm::vec3 normal) {
	orientation_.SetOrientation(glm::angleAxis(angle*glm::pi<float>() / 180.0f, normal));
}

void SceneNode::SetOrientation(glm::quat orientation) {

	orientation_.SetOrientation(orientation);
}


void SceneNode::SetScale(glm::vec3 scale) {

	scale_ = scale;
}

void SceneNode::SetMovementSpeed(float s) {
	movement_speed = s;
}

void SceneNode::SetName(std::string name) {
	name_ = name;
}
void SceneNode::Translate(glm::vec3 trans) {

	position_ += trans;
}


void SceneNode::Rotate(glm::quat rot) {

	orientation_.Rotate(rot);
}


void SceneNode::Scale(glm::vec3 scale) {

	scale_ *= scale;
}

void SceneNode::RotateOverTime(float rotation_speed, glm::vec3 rotation_normal) {

	rotation_normal_ = rotation_normal;
	rotation_speed_ = rotation_speed;
}


GLenum SceneNode::GetMode(void) const {

    return mode_;
}


GLuint SceneNode::GetArrayBuffer(void) const {

    return array_buffer_;
}


GLuint SceneNode::GetElementArrayBuffer(void) const {

    return element_array_buffer_;
}


GLsizei SceneNode::GetSize(void) const {

    return size_;
}


GLuint SceneNode::GetMaterial(void) const {

    return material_;
}

bool SceneNode::Hit(glm::vec3 pos, float range) {
	float dis = glm::distance(pos, position_);
	//std::cout << dis << std::endl;
	if (dis <= range) {

		exists_ = false;
	}
	return !exists_;
}

void SceneNode::Draw(Camera *camera){

    // Select proper material (shader program)
    glUseProgram(material_);

    // Set geometry to draw
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

    // Set globals for camera
    camera->SetupShader(material_);

    // Set world matrix and other shader input variables
    SetupShader(material_);

    // Draw geometry
    if (mode_ == GL_POINTS){
        glDrawArrays(mode_, 0, size_);
    } else {
        glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
    }
}


void SceneNode::Update(double deltaTime){

    // Do nothing for this generic type of scene node
}


void SceneNode::SetupShader(GLuint program){

    // Set attributes for shaders
    GLint vertex_att = glGetAttribLocation(program, "vertex");
    glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vertex_att);

    GLint normal_att = glGetAttribLocation(program, "normal");
    glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(normal_att);

    GLint color_att = glGetAttribLocation(program, "color");
    glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (6*sizeof(GLfloat)));
    glEnableVertexAttribArray(color_att);

    GLint tex_att = glGetAttribLocation(program, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (9*sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_att);

    // World transformation
	ApplyTransformation(program);

    // Timer
    GLint timer_var = glGetUniformLocation(program, "timer");
    double current_time = glfwGetTime();
    glUniform1f(timer_var, (float) current_time);
}

void SceneNode::ApplyTransformation(GLuint program) {
	model_->Translate(position_, true);
	model_->Rotate(orientation_.GetOrientation());
	model_->Scale(scale_);
	GLint world_mat = glGetUniformLocation(program, "world_mat");
	glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(model_->Apply(true)));
}

} // namespace game;
