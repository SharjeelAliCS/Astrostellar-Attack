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

#include "gameobj.h"

/* TODO: 

	- remove unneeded camera functionality from this demo
	
*/

namespace game {


	GameObj::GameObj(void) {

		program = 0; // cannot draw anything yet

		// Other attributes
		scale_ = glm::vec3(1.0, 1.0, 1.0);
	}

	GameObj::GameObj(GLuint theshader, Transformation* m) {

		program = theshader; // shader program used to render

							 // Note, by default contains no geometry -- call CreateTorus or whatever to make a drawable object.
		
		// Other attributes
		scale_ = glm::vec3(1.0, 1.0, 1.0);
		model = m;

		rotation_speed_ = 0;
		rotation_normal_ = glm::vec3(0);
		movement_speed = 0;
		orientation_.SetView(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		exists_ = true;
	}

	GameObj::GameObj(GLuint theshader, Transformation* m, glm::vec3 forward) {

		program = theshader; // shader program used to render

							 // Note, by default contains no geometry -- call CreateTorus or whatever to make a drawable object.

		// Other attributes
		scale_ = glm::vec3(1.0, 1.0, 1.0);
		model = m;

		rotation_speed_ = 0;
		rotation_normal_ = glm::vec3(0);
		movement_speed = 0;
		orientation_.SetView(glm::vec3(0), forward, glm::vec3(0, 1, 0));
		exists_ = true;
	}

GameObj::~GameObj(){
	for (auto const& x : geom) {
		delete x.second;
	}
	
}


glm::vec3 GameObj::GetForward(void) const {

	return orientation_.GetForward();
}


glm::vec3 GameObj::GetSide(void) const {

	return orientation_.GetSide();
}


glm::vec3 GameObj::GetUp(void) const {
	return orientation_.GetUp();
}


void GameObj::Pitch(float angle) {

	orientation_.Pitch(angle);
}

void GameObj::Yaw(float angle) {

	orientation_.Yaw(angle);
}

void GameObj::Roll(float angle) {

	orientation_.Roll(angle);
}

glm::vec3 GameObj::GetOrgPos(void) const {

	return org_pos_;
}

glm::vec3 GameObj::GetPosition(void) const {

    return position_;
}


glm::quat GameObj::GetOrientation(void) const {

    return orientation_.GetOrientation();
}


glm::vec3 GameObj::GetScale(void) const {

    return scale_;
}

bool GameObj::Exists(void) const {
	return exists_;
}


void GameObj::SetOrgPos(glm::vec3 position) {

	org_pos_ = position;
}


void GameObj::SetPosition(glm::vec3 position){

    position_ = position;
}

void GameObj::SetOrientation(float angle, glm::vec3 normal) {
	orientation_.SetOrientation(glm::angleAxis(angle*glm::pi<float>() / 180.0f, normal));
}

void GameObj::SetOrientation(glm::quat orientation){

    orientation_ .SetOrientation(orientation);
}


void GameObj::SetScale(glm::vec3 scale){

    scale_ = scale;
}

void GameObj::SetMovementSpeed(float s) {
	movement_speed = s;
}

void GameObj::SetName(std::string name) {
	name_ = name;
}
void GameObj::Translate(glm::vec3 trans){

    position_ += trans;
}


void GameObj::Rotate(glm::quat rot){

    orientation_.Rotate(rot);
}


void GameObj::Scale(glm::vec3 scale){

    scale_ *= scale;
}

void GameObj::RotateOverTime(float rotation_speed, glm::vec3 rotation_normal) {

	rotation_normal_ = rotation_normal;
	rotation_speed_ = rotation_speed;
}


std::string GameObj::GetName(void) const {
	return name_;
}

bool GameObj::Hit(glm::vec3 pos, float range) {
	float dis = glm::distance(pos, position_);
	//std::cout << dis << std::endl;
	if (dis <= range) {

		exists_ = false;
	}
	return !exists_;
}

void GameObj::Draw(Camera *camera){
	if (exists_) {
		// Set globals for camera
		camera->SetupShader(program);

		model->Translate(position_, true);
		model->Rotate(orientation_.GetOrientation());
		model->Scale(scale_);

		// Draw geometry
		for (auto const& x : geom) {
			x.second->Draw(camera);
		}

		model->PopMatrix();
	}
    
}


void GameObj::Update(double deltaTime){
	orientation_.Rotate(glm::angleAxis(rotation_speed_*(float)deltaTime*glm::pi<float>() / 180.0f, rotation_normal_));
    //Instead, simply iterate through each geom and update them. 
	for (auto const& x : geom) {
		x.second->Update(deltaTime);
	}


}


//For the below functions, nothing really has been changed. Just delagted the creation to geom. 
void GameObj::CreateCube(float w, float h, float d) {
	if (geom.count("0")) {
		geom.at("0") = new Geom(program, model);
	}
	else {
		geom.insert({ "0", new Geom(program, model) });
	}
	geom.at("0")->CreateCube(w,h,d);
}
void GameObj::CreateTorus(float loop_radius, float circle_radius, int num_loop_samples, int num_circle_samples) {
	if (geom.count("0")) {
		geom.at("0") = new Geom(program, model);
	}
	else {
		geom.insert({ "0", new Geom(program, model) });
	}
	geom.at("0")->CreateTorus(loop_radius, circle_radius, num_loop_samples, num_circle_samples);
}

void GameObj::CreateCylinder(float height = 0.6, float circle_radius = 0.2, int num_height_samples = 20, int num_circle_samples = 30) {
	if (geom.count("0")) {
		geom.at("0") = new Geom(program, model);
	}
	else {
		geom.insert({ "0", new Geom(program, model )});
	}
	geom.at("0")->CreateCylinder(height, circle_radius, num_circle_samples, num_circle_samples);
}




} // namespace game;
