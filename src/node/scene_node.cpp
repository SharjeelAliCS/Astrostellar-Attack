#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "scene_node.h"

namespace game {

SceneNode::SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal){

    // Set name of scene node
    name_ = name;

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

    // Set texture
    if (texture){
        texture_ = texture->GetResource();
    } else {
        texture_ = 0;
    }
	if (normal) {
		normal_map_ = normal->GetResource();
	}
	else {
		normal_map_ = 0;
	}
    // Other attributes
    scale_ = glm::vec3(1.0, 1.0, 1.0);
	orientation_ = new Orientation();
	orientation_->SetView(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	joint_ = glm::vec3(0);
	parentTransform_ = glm::mat4(1);
	blending_ = false;
	color_ = glm::vec3(1);
}


SceneNode::~SceneNode(){
}


const std::string SceneNode::GetName(void) const {
    return name_;
}


bool SceneNode::GetBlending(void) const {
	return blending_;
}
glm::vec3 SceneNode::GetPosition(void) const {
    return position_;
}

glm::quat SceneNode::GetOrientation(void) const {
	return orientation_->GetOrientation();
}
Orientation*  SceneNode::GetOrientationObj(void) const {
	return orientation_;
}

glm::vec3 SceneNode::GetScale(void) const {

    return scale_;
}

glm::vec3 SceneNode::GetOrgPos(void) const {
	return org_pos_;
}

void SceneNode::SetPosition(glm::vec3 position,bool newOrgPos){
    position_ = position;
	org_pos_ = position;
}

void SceneNode::SetColor(glm::vec3 c) {
	color_ = c;
}
SceneNode* SceneNode::GetChild(std::string child) {
	for (std::vector<SceneNode*>::iterator it = children_.begin(); it != children_.end(); ++it) {
		return (*it);
	}
	return NULL;
}


void SceneNode::SetAudio(Audio* audio) {
	audio_ = audio;
}
void SceneNode::SetTexture(Resource* texture) {
	texture_ = texture->GetResource();
}
void SceneNode::SetOrientation(float angle, glm::vec3 normal) {
	
	orientation_->SetOrientation(glm::angleAxis(angle*glm::pi<float>() / 180.0f, normal));
}
void SceneNode::SetBlending(bool blending) {
	blending_ = blending;
}

void SceneNode::SetOrientation(glm::quat orientation) {

	orientation_->SetOrientation(orientation);
}
void SceneNode::SetOrgPos(glm::vec3 pos) {
	org_pos_ = pos;
}

void SceneNode::SetJoint(glm::vec3 joint) {
	joint_ = joint;
}

void SceneNode::SetScale(glm::vec3 scale){
    scale_ = scale;
}
void SceneNode::SetDraw(bool d) {
	draw_ = d;
}
void SceneNode::Translate(glm::vec3 trans){
    position_ += trans;
}


bool SceneNode::Hit(glm::vec3 pos, float range) {
	//std::cout << "distance is: " << glm::distance(pos, this->GetPosition()) << std::endl;
	//std::cout << "range is: " <<  range + glm::length(this->GetScale()) << std::endl<< std::endl;

	return glm::distance(pos, this->GetPosition()) <= range + glm::length(this->GetScale());
}


bool SceneNode::HitTorus(glm::vec3 pos, float range) {

	float R = 0.15;
	float r = 0.6;
	float t = 0.2;
	bool hit = true;
	glm::vec3 n = orientation_->GetForward();
	glm::vec3 pc = pos - position_;
	float dist_y = glm::dot(pc, n);

	glm::vec3 dis_plane_v = dist_y * n;
	glm::vec3 p_plane = pos - dis_plane_v;

	glm::vec3 pnc = p_plane - position_;
	float dist_x = glm::length(pnc) - r;
	/*
	glm::vec3 co = r * glm::normalize(pnc);

	glm::vec3 dis_co_p_v = pos - co;
	float dist = glm::length(dis_co_p_v);
	*/

	float dist = glm::length(glm::vec2(dist_x, dist_y));

	dist -= t;
	dist -= R;
	std::cout << "plane: " << dist_y << ", dist " << dist_x << std::endl;
	if (dist < 0) {
		std::cout << "HIT " << std::endl;
	}
	/*
	float dis = glm::distance(pos, position_);
	//std::cout << dis << std::endl;
	if (dis <= range) {

		exists_ = false;
	}
	*/

	return hit;
}


void SceneNode::Rotate(glm::quat rot){
	orientation_->Rotate(rot);
}
void SceneNode::Rotate(float angle, glm::vec3 normal) {
	orientation_->Rotate(glm::angleAxis(angle*glm::pi<float>() / 180.0f, normal));
}

void SceneNode::Scale(glm::vec3 scale){
    scale_ *= scale;
}



void SceneNode::SetParentTransform(glm::mat4 trans) {
	parentTransform_ = trans;
}

void SceneNode::AddParent(SceneNode* parent) {
	parent_ = parent;
}


void SceneNode::AddChild(SceneNode* child) {
	children_.push_back(child); //push a child into the vector. 
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

void SceneNode::SetupBlending(void) {
	if (blending_) {
		// Disable z-buffer
		glDisable(GL_DEPTH_TEST);

		// Enable blending
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Simpler form
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
	}
	else {
		// Enable z-buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
}
void SceneNode::Draw(Camera *camera){
	if (!draw_)return;
	SetupBlending();
	/*
	//https://stackoverflow.com/questions/6301085/how-to-check-if-an-object-lies-outside-the-clipping-volume-in-opengl
	glm::mat4 view_mat = camera->GetView();
	glm::vec4 Pclip = view_mat * glm::vec4(position_, 1.);
	if(not( abs(Pclip.x) < Pclip.w &&
		abs(Pclip.y) < Pclip.w &&
		abs(Pclip.z) < Pclip.w)){
		return;
		}
	else {
	}
	*/
	
	glm::vec3 view_plane = camera->GetForward();
	if (glm::dot(view_plane, (position_-camera->GetPosition())) < 0 &&
		glm::distance(camera->GetPosition(), position_)< camera->GetFarDistance()) {
		return;
	}
    // Select proper material (shader program)
    glUseProgram(material_);

	//glEnable(GL_BLEND);

    // Set geometry to draw
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

    // Set globals for camera
    camera->SetupShader(material_);

    // Set world matrix and other shader input variables
    SetupShader(material_,camera);

    // Draw geometry
    if (mode_ == GL_POINTS){
        glDrawArrays(mode_, 0, size_);
    } else {
        glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
    }

	for (std::vector<SceneNode*>::iterator it = children_.begin(); it != children_.end(); ++it) {
		(*it)->Draw(camera);
	}
	glDisable(GL_BLEND);
}


void SceneNode::Update(float deltaTime){

    // Do nothing for this generic type of scene node
}

glm::mat4 SceneNode::CalculateFinalTransformation(Camera* camera) {
	// World transformation
	glm::mat4 Orientation = glm::mat4_cast(orientation_->GetOrientation());
	glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);
	glm::mat4 trans_joint = glm::translate(glm::mat4(1.0), joint_);
	glm::mat4 trans_joint_inv = glm::translate(glm::mat4(1.0), -joint_);

	glm::mat4 orbit = trans_joint_inv * Orientation * trans_joint;
	glm::mat4 transf = parentTransform_* translation * orbit;
	
	for (std::vector<SceneNode*>::iterator it = children_.begin(); it != children_.end(); ++it) {
		(*it)->SetParentTransform(transf);
	}

	return transf;
}
void SceneNode::SetupShader(GLuint program, Camera* camera){

	
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
    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
	 glm::mat4 transf = CalculateFinalTransformation(camera) * scaling;

    GLint world_mat = glGetUniformLocation(program, "world_mat");
    glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

    // Normal matrix
    glm::mat4 normal_matrix = glm::transpose(glm::inverse(transf));
    GLint normal_mat = glGetUniformLocation(program, "normal_mat");
    glUniformMatrix4fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    // Texture
    if (texture_){
        GLint tex = glGetUniformLocation(program, "texture_map");
        glUniform1i(tex, 0); // Assign the first texture to the map
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture_); // First texture we bind
        // Define texture interpolation
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_REPEAT);
    }

	//normal
	if (normal_map_) {
		glDisable(GL_BLEND);
		GLint tex = glGetUniformLocation(program, "normal_map");
		glUniform1i(tex, 1); // Assign the first texture to the map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_map_); // normal texture we bind
	}


	//color
	GLint color = glGetUniformLocation(program, "color");
	glUniform3f(color, color_.x, color_.y, color_.z);

    // Timer
    GLint timer_var = glGetUniformLocation(program, "timer");
    double current_time = glfwGetTime();
    glUniform1f(timer_var, (float) current_time);
}

} // namespace game;
