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

#include "particle_node.h"


namespace game {

	ParticleNode::ParticleNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *normal) : SceneNode(name, geometry, material, texture, normal) {
		duration_ = -1;
		max_duration_ = -1;
		start_time_ = -1;
	}

	ParticleNode::~ParticleNode() {
	}
	void ParticleNode::Update(float deltaTime) {
		if (duration_ > 0) {
			duration_ -= deltaTime;
		}
		else if(max_duration_!=-1){
			exists_ = false;
		}
	}
	void ParticleNode::Draw(Camera *camera) {
	
		if (!draw_ || !exists_)return;
		SetupBlending();
		
		// Select particle blending or not
		// Select proper material (shader program)
		glUseProgram(material_);

		glEnable(GL_BLEND);

		// Set geometry to draw
		glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

		// Set globals for camera
		camera->SetupShader(material_);

		// Set world matrix and other shader input variables
		SetupShader(material_, camera);

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
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void ParticleNode::SetupShader(GLuint program, Camera* camera) {

		int size = 12;
		// Set attributes for shaders
		GLint vertex_att = glGetAttribLocation(program, "vertex");
		glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(vertex_att);

		GLint normal_att = glGetAttribLocation(program, "normal");
		glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(normal_att);

		GLint color_att = glGetAttribLocation(program, "color");
		glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(color_att);

		GLint tex_att = glGetAttribLocation(program, "uv");
		glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(tex_att);

		GLint t_att = glGetAttribLocation(program, "t_d");
		glVertexAttribPointer(t_att, 1, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void *)(11 * sizeof(GLfloat)));
		glEnableVertexAttribArray(t_att);

		// World transformation
		glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
		
		glm::mat4 transf = CalculateFinalTransformation(camera) * scaling;

		GLint world_mat = glGetUniformLocation(program, "world_mat");
		glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

		// Normal matrix
		glm::mat4 normal_matrix = glm::transpose(glm::inverse(transf));
		GLint normal_mat = glGetUniformLocation(program, "normal_mat");
		glUniformMatrix4fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));

		//color
		GLint color = glGetUniformLocation(program, "color");
		glUniform3f(color, color_.x, color_.y, color_.z);

		// Texture
		if (texture_) {
			GLint tex = glGetUniformLocation(program, "texture_map");
			glUniform1i(tex, 0); // Assign the first texture to the map
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_); // First texture we bind
			// Define texture interpolation
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// Timer
		GLint timer_var = glGetUniformLocation(program, "timer");
		double current_time = glfwGetTime();
		if (start_time_ == -1)start_time_ = current_time;
		glUniform1f(timer_var, (float)current_time);


		GLint start_var = glGetUniformLocation(program, "start_time");
		glUniform1f(start_var, (float)start_time_);

		if (max_duration_ != -1) {
			GLint duration_var = glGetUniformLocation(program, "duration");
			glUniform1f(duration_var, (float)max_duration_);
		}
	}

}
