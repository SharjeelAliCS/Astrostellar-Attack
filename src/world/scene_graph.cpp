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
		HUDNode* hud = new HUDNode(node_name, geometry, material, texture);
		hud_.push_back(hud);
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

void SceneGraph::AddHUD(HUDNode *node) {
	node_.push_back(node);
}

HUDNode *SceneGraph::GetHUD(std::string node_name) const {
	// Find node with the specified name
	for (int i = 0; i < hud_.size(); i++) {
		if (hud_[i]->GetName() == node_name) {
			return hud_[i];
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
	for (int i = 0; i < hud_.size(); i++) {
		hud_[i]->Draw(camera);
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
	for (int i = 0; i < hud_.size(); i++) {
		hud_[i]->Update(deltaTime);
	}
}

} // namespace game
