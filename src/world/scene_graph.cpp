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

	for (int i = HUD_MENU; i != NONE+1; i++){
		std::vector<ScreenNode*> temp;
		screen_.insert({static_cast<ScreenType>(i), temp });
	}
	active_menu_ = HUD_MENU;

	radar_distance_ = 100;

}


SceneGraph::~SceneGraph(){
}


void SceneGraph::SetBackgroundColor(glm::vec3 color){

    background_color_ = color;
}


glm::vec3 SceneGraph::GetBackgroundColor(void) const {

    return background_color_;
}
 

SceneNode *SceneGraph::CreateNode(std::string node_name, Resource *geometry, Resource *material,NodeType type, Resource *texture){

	SceneNode *scn;
	switch (type) {
	case PLAYER: {
		player_ = new Player(node_name, geometry, material, texture);
		scn = player_;
		break; }
	case SKYBOX: {
		skybox_ = new SkyBox(node_name, geometry, material, texture);
		scn = skybox_;
		break; }
	case ENEMY: {
		Enemy* en = new Enemy(node_name, geometry, material, texture);
		enemy_.push_back(en);
		scn = en;
		break; }
	case HUD: {
		ScreenNode* hud = new ScreenNode(node_name, geometry, material, texture);
		screen_.at(NONE).push_back(hud);
		scn = hud;
		break; }
	case NODE: {
		SceneNode* node = new SceneNode(node_name, geometry, material, texture);
		node_.push_back(node);
		scn = node;
		break; }
	}
	return scn;
}


void SceneGraph::AddNode(SceneNode *node){
    node_.push_back(node);
}

SceneNode *SceneGraph::GetNode(std::string node_name) const {
    // Find node with the specified name
    for (int i = 0; i < node_.size(); i++){
        if (node_[i]->GetName() == node_name){
            return node_[i];
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
	enemy_.push_back(node);
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

void SceneGraph::Draw(Camera *camera){

    // Clear background
    glClearColor(background_color_[0], 
                 background_color_[1],
                 background_color_[2], 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw all scene nodes

	if (skybox_ != NULL)skybox_->Draw(camera);
	
	if(player_!=NULL)player_->Draw(camera);

    for (int i = 0; i < node_.size(); i++){
        node_[i]->Draw(camera);
    }
	for (int i = 0; i < enemy_.size(); i++) {
		enemy_[i]->Draw(camera);
	}

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

	
}


void SceneGraph::Update(float deltaTime){

	if (player_ != NULL)player_->Update(deltaTime);
	if (skybox_ != NULL)skybox_->Update(deltaTime);

    for (int i = 0; i < node_.size(); i++){
        node_[i]->Update(deltaTime);
    }
	for (int i = 0; i < enemy_.size(); i++) {
		enemy_[i]->Update(deltaTime);
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
	glm::vec3 pos_3d = player_->GetPosition();
	glm::vec2 pos_player(pos_3d.x, pos_3d.z);

	glm::vec3 direction = player_->GetOrientationObj()->GetForward();
	std::vector<glm::vec2> entities;
	for (int i = 0; i < node_.size(); i++) {
		UpdateRadarNode(direction,pos_player, node_[i], entities);
	}
	for (int i = 0; i < enemy_.size(); i++) {
		UpdateRadarNode(direction,pos_player, enemy_[i], entities);
	}
	radar_->SetEntityPositions(entities);

}

void SceneGraph::UpdateRadarNode(glm::vec3 direction, glm::vec2 pos, SceneNode* node, std::vector<glm::vec2>& e){
	glm::vec3 pos_3d = node->GetPosition();
	glm::vec2 pos_2d(pos_3d.x, pos_3d.z);

	
	if (glm::distance(pos_2d, pos) <= radar_distance_) {
		glm::vec2 direction_2d(direction.x, direction.z);
		direction_2d = glm::normalize(direction_2d);
		float angle = atan2(direction_2d.x, direction_2d.y);

		glm::vec2 radar_pos = pos_2d-pos;
		radar_pos = radar_pos/radar_distance_;

		radar_pos /= 2;

		radar_pos.x = radar_pos.x* cos(angle) - radar_pos.y * sin(angle);
		radar_pos.y = radar_pos.x *sin(angle) + radar_pos.y * cos(angle);

		radar_pos.y *= -1;
		radar_pos.x *= -1;
		radar_pos.x += 0.5;
		radar_pos.y += 0.5;

		e.push_back(radar_pos);
	}

}

void SceneGraph::SetCurrentScreen(ScreenType t) {
	active_menu_ = t;
}

} // namespace game
