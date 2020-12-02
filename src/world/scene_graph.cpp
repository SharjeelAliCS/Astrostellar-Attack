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
	comet_ = new std::vector<CometNode*>;
	asteroid_ = new std::vector<AsteroidNode*>;
	death_animations_ = new std::vector<ParticleNode*>;

	for (int i = HUD_MENU; i != NONE+1; i++){
		std::vector<ScreenNode*> screen;
		std::vector<ButtonNode*> button;
		screen_.insert({static_cast<ScreenType>(i), screen });
		button_.insert({ static_cast<ScreenType>(i), button });
	}
	active_menu_ = HUD_MENU;

	radar_distance_ = 1000;
	enemy_healthbar_distance_ = 300;
	audio_ = NULL;

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
	for (int i = 0; i < enemy_->size(); i++) {
		if (enemy_->at(i)->GetName() == node_name) {
			return enemy_->at(i);
		}
	}
	return NULL;
}

void SceneGraph::AddAsteroid(AsteroidNode *node) {
	asteroid_->push_back(node);
}

AsteroidNode *SceneGraph::GetAsteroid(std::string node_name) const {
	// Find node with the specified name
	for (int i = 0; i < asteroid_->size(); i++) {
		if (asteroid_->at(i)->GetName() == node_name) {
			return asteroid_->at(i);
		}
	}
	return NULL;
}

void SceneGraph::AddComet(CometNode *node) {
	comet_->push_back(node);
}

CometNode *SceneGraph::GetComet(std::string node_name) const {
	// Find node with the specified name
	for (int i = 0; i < comet_->size(); i++) {
		if (comet_->at(i)->GetName() == node_name) {
			return comet_->at(i);
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


void SceneGraph::SetupDrawToTexture(float frame_width, float frame_height) {

	// Set up frame buffer
	glGenFramebuffers(1, &frame_buffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

	// Set up target texture for rendering
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);

	// Set up an image for the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set up a depth buffer for rendering
	glGenRenderbuffers(1, &depth_buffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frame_width, frame_height);

	// Configure frame buffer (attach rendering buffers)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	// Check if frame buffer was setup successfully 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw(std::ios_base::failure(std::string("Error setting up frame buffer")));
	}

	// Reset frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Set up quad for drawing to the screen
	static const GLfloat quad_vertex_data[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	};

	// Create buffer for quad
	glGenBuffers(1, &quad_array_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, quad_array_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_data), quad_vertex_data, GL_STATIC_DRAW);
}


void SceneGraph::Draw(Camera *camera, bool to_texture,float frame_width, float frame_height){
	GLint viewport[4];
	if (to_texture) {
		// Save current viewport	
		glGetIntegerv(GL_VIEWPORT, viewport);

		// Enable frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
		glViewport(0, 0, frame_width, frame_height);

	}
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

	float radius = 0.05;
	for (int i = 0; i < enemy_->size(); i++) {
		(*enemy_)[i]->Draw(camera);

		if (glm::distance((*enemy_)[i]->GetPosition(), player_->GetPosition()) < enemy_healthbar_distance_) {
			glm::vec3 screen_pos = (*enemy_)[i]->GetScreenSpacePos(true, camera);
			
			if (abs(screen_pos.z) <= 1) {
				float per = (*enemy_)[i]->getHealthPercent();
				GetScreen("enemyHealthBar")->SetProgressX(per);
				DrawEnemyHealth(camera, glm::vec2(screen_pos.x, screen_pos.y));
			}
		}
		/*
		glm::vec2 screen_pos = (*enemy_)[i]->GetScreenSpacePos(false, camera);
		bool within_length = glm::length(screen_pos) < radius;

		if ((*enemy_)[i]->SeeHealth(within_length)) {
			screen_pos = (*enemy_)[i]->GetScreenSpacePos(true, camera);
			float per = (*enemy_)[i]->getHealthPercent();
			GetScreen("enemyHealthBar")->SetProgressX(per);
			DrawEnemyHealth(camera, screen_pos);
		}
		*/
	}
	for (int i = 0; i < asteroid_->size(); i++) {
		asteroid_->at(i)->Draw(camera);
	}
	for (int i = 0; i < comet_->size(); i++) {
		comet_->at(i)->Draw(camera);
	}
	for (int i = 0; i < death_animations_->size(); i++) {
		death_animations_->at(i)->Draw(camera);
	}
	if(player_!=NULL)player_->Draw(camera);


	if (active_menu_ == HUD_MENU || active_menu_ == PAUSE_MENU) {

		for (int i = 0; i < screen_.at(NONE).size(); i++) {
			screen_.at(NONE)[i]->Draw(camera);
		}
		radar_->Draw(camera);

		//DrawEnemyHealth(camera);
	}
	if (active_menu_ == PAUSE_MENU) {
		for (int i = 0; i < screen_.at(HUD_MENU).size(); i++) {
			screen_.at(HUD_MENU)[i]->Draw(camera);
		}

		//DrawEnemyHealth(camera);
	}

	for (int i = 0; i < screen_.at(active_menu_).size(); i++) {
		screen_.at(active_menu_)[i]->Draw(camera);
	}

	for (int i = 0; i < button_.at(active_menu_).size(); i++) {
		button_.at(active_menu_)[i]->Draw(camera);
	}
	if (to_texture) {
		// Reset frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Restore viewport
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}

}

void SceneGraph::DrawEnemyHealth(Camera* camera, glm::vec2 pos) {

		for (int i = 0; i < screen_.at(ENEMY_HEALTH).size(); i++) {
			screen_.at(ENEMY_HEALTH)[i]->SetPosition(glm::vec3(pos, 0));
			screen_.at(ENEMY_HEALTH)[i]->Draw(camera);
		}
		return;
	
}

void SceneGraph::DisplayScreenSpace(GLuint program, std::string name,bool to_texture, float frame_width, float frame_height) {
	GLint viewport[4];
	if (to_texture) {
		// Save current viewport	
		glGetIntegerv(GL_VIEWPORT, viewport);

		// Enable frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
		glViewport(0, 0, frame_width, frame_height);

	}
	// Configure output to the screen
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	// Set up quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, quad_array_buffer_);

	// Select proper material (shader program)
	glUseProgram(program);

	// Setup attributes of screen-space shader
	GLint pos_att = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(pos_att);
	glVertexAttribPointer(pos_att, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint tex_att = glGetAttribLocation(program, "uv");
	glEnableVertexAttribArray(tex_att);
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

	float current_time = glfwGetTime();
	//get the time intensity (0 to 1) for the screen effect
	float time_intensity = player_->getNuclearOverloadPercent();
	
	// Timer
	GLint timer_var = glGetUniformLocation(program, "timer");
	glUniform1f(timer_var, current_time);

	GLint time_diff_var = glGetUniformLocation(program, "time_diff");
	glUniform1f(time_diff_var, time_intensity);

	//std::cout << "reduction start time is " << time_intensity << std::endl;

	

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);

	// Draw geometry
	glDrawArrays(GL_TRIANGLES, 0, 6); // Quad: 6 coordinates

	// Reset current geometry
	glEnable(GL_DEPTH_TEST);

	if (to_texture) {
		// Reset frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Restore viewport
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}
}


void SceneGraph::CreateDeathAnimation(SceneNode* node) {
	std::string name = node->GetName() + "_death";
	ParticleNode* pn = new ParticleNode(name,death_animation_rsc.geom, death_animation_rsc.mat, death_animation_rsc.tex);
	glm::vec3 scale = node->GetScale()*(float)0.1;
	pn->SetScale(scale);
	pn->SetPosition(node->GetPosition());
	pn->SetBlending(true);
	pn->SetColor(glm::vec3(0.58,0.29,0));
	pn->SetDuration(0.5);
	death_animations_->push_back(pn);
}
bool SceneGraph::ProjectileCollision(AgentNode* node, bool player) {
	std::vector<Projectile*>* missiles = node->GetMissiles();

	for (auto it = missiles->begin(); it != missiles->end(); ) {
		
		Collision((*it), player);
		if ((*it)->Exists()) {
			++it;
		}
		else{
			it = missiles->erase(it);
		}
	}
	return true;


}
bool SceneGraph::Collision(Entity* node, bool player) {
	bool collided = false;
	if (player) {
		for (auto ast = asteroid_->begin(); ast != asteroid_->end(); ) {
			if ((*ast)->Hit(node->GetPosition(), (*ast)->GetScale().x * 0.9)) {
				CreateDeathAnimation((*ast));
				ast = asteroid_->erase(ast);
				node->damage(node->GetDamage());
				collided = true;
				if (player)audio_->playAgain("asteroidExplosion");
			}
			else {
				++ast;
			}
		}

		for (auto ast = enemy_->begin(); ast != enemy_->end(); ) {
			if ((*ast)->Hit(node->GetPosition(), (*ast)->GetScale().x * 0.2)) {
				node->damage((*ast)->GetDamage());
				(*ast)->damage(node->GetDamage());
				collided = true;
				if (player)audio_->playAgain("enemyHit");

				float per = (*ast)->getHealthPercent();
				std::cout << "damage is " << node->GetDamage() << std::endl;
			}
			if((*ast)->Exists()){
				++ast;
			}
			else {
				CreateDeathAnimation((*ast));
				ast = enemy_->erase(ast);
				if (player)audio_->playAgain("asteroidExplosion");
			}
		}

		for (auto ast = comet_->begin(); ast != comet_->end(); ) {
			if ((*ast)->Hit(node->GetPosition(), (*ast)->GetScale().x * 0.9)) {
				CreateDeathAnimation((*ast));
				ast = comet_->erase(ast);
				node->damage(node->GetDamage());
				collided = true;
				if (player)audio_->playAgain("asteroidExplosion");
			}
			else {
				++ast;
			}
		}
	}
	else {
		if ((player_->Hit(node->GetPosition(), glm::length(player_->GetScale()) * 0.9))) {
			player_->damage(node->GetDamage());
			node->damage(player_->GetDamage());
			collided = true;
			audio_->playAgain("playerHit");

		}
	}
	return collided;
}

void SceneGraph::SetDeathAnimation(NodeResources dm) {
	death_animation_rsc = dm;
}
void SceneGraph::Update(float deltaTime){
	Collision(player_, true);
	ProjectileCollision(player_, true);

	if (player_ != NULL)player_->Update(deltaTime);
	if (skybox_ != NULL)skybox_->Update(deltaTime);

    for (int i = 0; i < node_->size(); i++){
		(*node_)[i]->Update(deltaTime);
    }
	for (int i = 0; i < enemy_->size(); i++) {
		(*enemy_)[i]->Update(deltaTime);
		ProjectileCollision((*enemy_)[i], false);
	}
	for (int i = 0; i < asteroid_->size(); i++) {
		asteroid_->at(i)->Update(deltaTime);
	}
	for (int i = 0; i < comet_->size(); i++) {
		comet_->at(i)->Update(deltaTime);
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
	for (auto ast = death_animations_->begin(); ast != death_animations_->end(); ) {
		(*ast)->Update(deltaTime);
		if (!(*ast)->GetExists()) {
			ast = death_animations_->erase(ast);
		}
		else {
			++ast;
		}
	}
	UpdateRadar();
}

void SceneGraph::UpdateRadar() {
	glm::vec3 direction = player_->GetOrientationObj()->GetForward();
	for (int i = 0; i < asteroid_->size(); i++) {
		UpdateRadarNode(direction, asteroid_->at(i)->GetPosition(),glm::vec3(1,1,0));
	}
	for (int i = 0; i < comet_->size(); i++) {
		UpdateRadarNode(direction, comet_->at(i)->GetPosition(), glm::vec3(1, 1, 0));
	}
	for (int i = 0; i < enemy_->size(); i++) {
		UpdateRadarNode(direction, (*enemy_)[i]->GetPosition(), glm::vec3(1, 0, 0),true);
	}
	UpdateRadarNode(direction, glm::vec3(0) , glm::vec3(1, 1, 1),true);

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
void SceneGraph::UpdateRadarNode(glm::vec3 direction, glm::vec3 target_pos,glm::vec3 color,bool edge){
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
	else  if (edge) {
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
