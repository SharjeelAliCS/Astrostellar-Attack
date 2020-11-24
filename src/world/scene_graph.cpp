#include <stdexcept>
#include <iostream>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene_graph.h"

namespace game {

SceneGraph::SceneGraph(void){

    background_color_ = glm::vec3(0.0, 0.0, 0.0);
	player_ = NULL;
	skybox_ = NULL;
	node_ = new std::vector<SceneNode*>;
	enemy_ = new std::vector<Enemy*>;

	for (int i = HUD_MENU; i != NONE+1; i++){
		std::vector<ScreenNode*> screen;
		std::vector<ButtonNode*> button;
		screen_.insert({static_cast<ScreenType>(i), screen });
		button_.insert({ static_cast<ScreenType>(i), button });
	}
	active_menu_ = HUD_MENU;

	radar_distance_ = 1000;

}


SceneGraph::~SceneGraph(){
}


void SceneGraph::SetBackgroundColor(glm::vec3 color){

    background_color_ = color;
}


glm::vec3 SceneGraph::GetBackgroundColor(void) const {

    return background_color_;
}
 
SceneNode *SceneGraph::CreateNode(std::string node_name, Resource *geometry, Resource *material,NodeType type, Resource *texture, Resource *normal){

	SceneNode *scn;
	switch (type) {
	case PLAYER: {
		player_ = new Player(node_name, geometry, material, texture, normal);
		scn = player_;
		break; }
	case SKYBOX: {
		skybox_ = new SkyBox(node_name, geometry, material, texture, normal);
		scn = skybox_;
		break; }
	case ENEMY: {
		Enemy* en = new Enemy(node_name, geometry, material, texture, normal);
		enemy_->push_back(en);
		scn = en;
		break; }
	case HUD: {
		ScreenNode* hud = new ScreenNode(node_name, geometry, material, texture, normal);
		screen_.at(NONE).push_back(hud);
		scn = hud;
		break; }
	case NODE: {
		SceneNode* node = new SceneNode(node_name, geometry, material, texture, normal);
		node_->push_back(node);
		scn = node;
		break; }
	case ASTEROID: {
		AsteroidNode* node = new AsteroidNode(node_name, geometry, material, texture, normal);
		node_->push_back(node);
		scn = node;
		break; }

	}
	return scn;
}


void SceneGraph::AddNode(SceneNode *node){
    node_->push_back(node);
}

SceneNode *SceneGraph::GetNode(std::string node_name) const {
    // Find node with the specified name
    for (int i = 0; i < node_->size(); i++){
        if ((*node_)[i]->GetName() == node_name){
            return (*node_)[i];
        }
    }
    return NULL;

}

void SceneGraph::AddSkyBox(SkyBox *node) {
	skybox_ = node;
}

SkyBox *SceneGraph::GetSkyBox() const {
	return skybox_;

}
void SceneGraph::AddPlayer(Player *node) {
	player_ = node;
}
void SceneGraph::AddRadar(RadarNode *node) {
	radar_ = node;
}

Player *SceneGraph::GetPlayer() const {
	return player_;
}

void SceneGraph::AddEnemy(Enemy *node) {
	enemy_->push_back(node);
}

Enemy *SceneGraph::GetEnemy(std::string node_name) const {
	// Find node with the specified name
	for (int i = 0; i < enemy_.size(); i++) {
		if (enemy_[i]->GetName() == node_name) {
			return enemy_[i];
		}
	}
	return NULL;
}

void SceneGraph::AddAsteroid(AsteroidNode *node) {
	asteroid_.push_back(node);
}

AsteroidNode *SceneGraph::GetAsteroid(std::string node_name) const {
	// Find node with the specified name
	for (int i = 0; i < enemy_.size(); i++) {
		if (asteroid_[i]->GetName() == node_name) {
			return asteroid_[i];
		}
	}
	return NULL;
}

void SceneGraph::AddComet(CometNode *node) {
	comet_.push_back(node);
}

CometNode *SceneGraph::GetComet(std::string node_name) const {
	// Find node with the specified name
	for (int i = 0; i < enemy_.size(); i++) {
		if (comet_[i]->GetName() == node_name) {
			return comet_[i];
		}
	}
	return NULL;
}

void SceneGraph::AddScreen(ScreenNode *node, ScreenType type) {
	screen_.at(type).push_back(node);
}

ScreenNode *SceneGraph::GetScreen(std::string node_name) const {
	// Find node with the specified name
	for (auto it = screen_.begin(); it != screen_.end(); it++){
		for (int i = 0; i < it->second.size(); i++) {
			if (it->second[i]->GetName() == node_name) {
				return it->second[i];
			}
		}
	}

	return NULL;

}

void SceneGraph::AddButton(ButtonNode *node, ScreenType type) {
	button_.at(type).push_back(node);
}

ButtonNode *SceneGraph::GetButton(std::string node_name) const {
	// Find node with the specified name
	for (auto it = button_.begin(); it != button_.end(); it++) {
		for (int i = 0; i < it->second.size(); i++) {
			if (it->second[i]->GetName() == node_name) {
				return it->second[i];
			}
		}
	}

	return NULL;

}

void SceneGraph::Draw(Camera *camera){

    // Clear background
    glClearColor(background_color_[0], 
                 background_color_[1],
                 background_color_[2], 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw all scene nodes

	if (skybox_ != NULL)skybox_->Draw(camera);

    for (int i = 0; i < node_->size(); i++){
		(*node_)[i]->Draw(camera);
    }
	for (int i = 0; i < enemy_->size(); i++) {
		(*enemy_)[i]->Draw(camera);
	}
	for (int i = 0; i < asteroid_.size(); i++) {
		asteroid_[i]->Draw(camera);
	}
	for (int i = 0; i < comet_.size(); i++) {
		comet_[i]->Draw(camera);
	}
	if(player_!=NULL)player_->Draw(camera);


	if (active_menu_ == HUD_MENU || active_menu_ == PAUSE_MENU) {
		for (int i = 0; i < screen_.at(NONE).size(); i++) {
			screen_.at(NONE)[i]->Draw(camera);
		}
		radar_->Draw(camera);
	}
	if (active_menu_ == PAUSE_MENU) {
		for (int i = 0; i < screen_.at(HUD_MENU).size(); i++) {
			screen_.at(HUD_MENU)[i]->Draw(camera);
		}
	}

	for (int i = 0; i < screen_.at(active_menu_).size(); i++) {
		screen_.at(active_menu_)[i]->Draw(camera);
	}

	for (int i = 0; i < button_.at(active_menu_).size(); i++) {
		button_.at(active_menu_)[i]->Draw(camera);
	}

	
}


void SceneGraph::Update(float deltaTime){

	if (player_ != NULL)player_->Update(deltaTime);
	if (skybox_ != NULL)skybox_->Update(deltaTime);

    for (int i = 0; i < node_->size(); i++){
		(*node_)[i]->Update(deltaTime);
    }
	for (int i = 0; i < enemy_->size(); i++) {
		(*enemy_)[i]->Update(deltaTime);
	}
	for (int i = 0; i < asteroid_.size(); i++) {
		asteroid_[i]->Update(deltaTime);
	}
	for (int i = 0; i < comet_.size(); i++) {
		comet_[i]->Update(deltaTime);
	}
	for (int i = 0; i < screen_.at(NONE).size(); i++) {
		screen_.at(NONE)[i]->Update(deltaTime);
	}

	for (int i = 0; i < screen_.at(active_menu_).size(); i++) {
		screen_.at(active_menu_)[i]->Update(deltaTime);
	}
	if (active_menu_ == PAUSE_MENU) {
		for (int i = 0; i < screen_.at(HUD_MENU).size(); i++) {
			screen_.at(HUD_MENU)[i]->Update(deltaTime);
		}
	}
	if (active_menu_ == HUD_MENU || active_menu_ == PAUSE_MENU) {
		radar_->Update(deltaTime);
		for (int i = 0; i < screen_.at(NONE).size(); i++) {
			screen_.at(NONE)[i]->Update(deltaTime);
		}
	}
	UpdateRadar();
}

void SceneGraph::UpdateRadar() {
	glm::vec3 direction = player_->GetOrientationObj()->GetForward();
	for (int i = 0; i < asteroid_.size(); i++) {
		UpdateRadarNode(direction, asteroid_[i]->GetPosition(),glm::vec3(1,1,0));
	}
	for (int i = 0; i < comet_.size(); i++) {
		UpdateRadarNode(direction, comet_[i]->GetPosition(), glm::vec3(1, 1, 0));
	}
	for (int i = 0; i < enemy_->size(); i++) {
		UpdateRadarNode(direction, (*enemy_)[i]->GetPosition(), glm::vec3(1, 0, 0));
	}
	UpdateRadarNode(direction, glm::vec3(0) , glm::vec3(1, 1, 1));

	glm::vec3 pos_3d = player_->GetPosition();
	glm::vec2 pos_player(pos_3d.x, pos_3d.z);
}


std::string SceneGraph::ButtonEvents(float x, float y) {

	std::string btn = "";
	for (int i = 0; i < button_.at(active_menu_).size(); i++) {
		bool within_range = button_.at(active_menu_)[i]->WithinButton(x, y);
		button_.at(active_menu_)[i]->SetGlow(within_range);
		if (within_range) {
			btn = button_.at(active_menu_)[i]->GetName();
		}
	}
	return btn;
}

void SceneGraph::UpdateScreenSizeNodes(float x, float y) {
	for (auto it = button_.begin(); it != button_.end(); it++) {
		for (int i = 0; i < it->second.size(); i++) {
			it->second[i]->UpdateScreenSize(x, y);
		}
	}
}
void SceneGraph::UpdateRadarNode(glm::vec3 direction, glm::vec3 target_pos,glm::vec3 color){
	glm::vec3 pos_entity = CalculateDistanceFromPlayer(target_pos);
	glm::vec2 pos_2d(pos_entity.x, pos_entity.y);
	glm::vec3 pos_3d = player_->GetPosition();
	glm::vec2 pos_player(pos_3d.x, pos_3d.z);

	if (glm::length(pos_2d) <= radar_distance_ && pos_entity.z<=radar_distance_/10) {
		glm::vec2 radar_pos = pos_2d;
		radar_pos = radar_pos/radar_distance_;

		radar_pos /= 2;
		radar_pos.y *= -1;
		//radar_pos.x *= -1;

		radar_pos.x += 0.5;
		radar_pos.y += 0.5;

		radar_->AddDotPos(radar_pos);
		radar_->AddDotColor(color);
	}
	else  if (glm::length(target_pos) == 0) {
		glm::vec2 radar_pos = pos_2d;
		radar_pos = glm::normalize(radar_pos);

		radar_pos /= 2;
		radar_pos.y *= -1;
		radar_pos.x *= -1;

		radar_pos.x += 0.5;
		radar_pos.y += 0.5;

		radar_->AddDotPos(radar_pos);
		radar_->AddDotColor(color);
	}
}

glm::vec3 SceneGraph::CalculateDistanceFromPlayer(glm::vec3 pos) {
	//https://stackoverflow.com/questions/23472048/projecting-3d-points-to-2d-plane
	glm::vec3 player_pos = player_->GetPosition();
	glm::vec3 plane_up = player_->GetOrientationObj()->GetUp();
	glm::vec3 plane_x = player_->GetOrientationObj()->GetForward();
	glm::vec3 plane_y = player_->GetOrientationObj()->GetSide();

	float dis_plane = glm::dot((pos - player_pos), plane_up);
	glm::vec3 pos_plane = pos - dis_plane * plane_up;

	float y = glm::dot(plane_x, (pos-player_pos));
	float x = glm::dot(plane_y, (pos - player_pos));
	//float x = glm::distance(player_pos, plane_x, pos_plane, plane_x);
	//float y = glm::distance(player_pos, plane_y, pos_plane, plane_y);

	glm::vec3 pos_2d(x,y, abs(dis_plane));

	return pos_2d;
}
void SceneGraph::SetCurrentScreen(ScreenType t) {
	active_menu_ = t;
}

} // namespace game
