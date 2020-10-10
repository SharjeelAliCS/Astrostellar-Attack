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

#include "geometry.h"

 /* TODO:

	 - remove unneeded camera functionality from this demo

 */

namespace game {


	Geom::Geom(void) {

		program = 0; // cannot draw anything yet

		 // Note, by default contains no geometry -- call CreateTorus or whatever to make a drawable object.
		size_ = 0;

	}

	Geom::Geom(GLuint theshader, Transformation* m) {

		program = theshader; // shader program used to render

							 // Note, by default contains no geometry -- call CreateTorus or whatever to make a drawable object.
		size_ = 0;

		// Other attributes
		model = m;
		movement_speed = 0;
		orientation_.SetView(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		scale_ = glm::vec3(1.0);
		rotation_speed_ = 0;
		rotation_normal_ = glm::vec3(0);
	}

	Geom::~Geom() {
	}



	glm::vec3 Geom::GetPosition(void) const {

		return position_;
	}


	glm::quat Geom::GetOrientation(void) const {

		return orientation_.GetOrientation();
	}

	glm::vec3 Geom::GetScale(void) const {

		return scale_;
	}

	void Geom::SetPosition(glm::vec3 position) {

		position_ = position;
	}

	void Geom::SetOrientation(float angle, glm::vec3 normal) {
		orientation_.SetOrientation(glm::angleAxis(angle*glm::pi<float>() / 180.0f, normal));
	}

	void Geom::SetOrientation(glm::quat orientation) {

		orientation_.SetOrientation(orientation);
	}


	void Geom::SetScale(glm::vec3 scale) {

		scale_ = scale;
	}

	void Geom::RotateOverTime(float rotation_speed, glm::vec3 rotation_normal) {

		rotation_normal_ = rotation_normal;
		rotation_speed_ = rotation_speed;
	}

	GLuint Geom::GetArrayBuffer(void) const {

		return array_buffer_;
	}


	GLuint Geom::GetElementArrayBuffer(void) const {

		return element_array_buffer_;
	}


	GLsizei Geom::GetSize(void) const {

		return size_;
	}

	void Geom::Update(double deltaTime) {
		orientation_.Rotate(glm::angleAxis(rotation_speed_*(float)deltaTime*glm::pi<float>() / 180.0f, rotation_normal_));
	}
	void Geom::Draw(Camera *camera) {
		glUseProgram(program);
		// Set geometry to draw
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		//	SetShaderAttributes(program);
		//	BindVertexAttributes(program);

			// Set globals for camera
		camera->SetupShader(program);

		// Set world matrix and other shader input variables
		SetShaderAttributes(program);

		// Draw geometry

		glDrawElements(GL_TRIANGLES, size_, GL_UNSIGNED_INT, 0);

	}


	void Geom::SetShaderAttributes(GLuint program) {
		glEnable(GL_TEXTURE_2D);
		// Set attributes for shaders
		GLint vertex_att = glGetAttribLocation(program, "vertex");
		glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(vertex_att);

		GLint normal_att = glGetAttribLocation(program, "normal");
		glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(normal_att);

		GLint color_att = glGetAttribLocation(program, "color");
		glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(color_att);

		GLint tex_att = glGetAttribLocation(program, "uv");
		glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(tex_att);

		// World transformation by adding to the transformation model instead, to keep things simpler. 

		model->Translate(position_, true);
		model->Rotate(orientation_.GetOrientation());
		model->Scale(scale_);

		GLint world_mat = glGetUniformLocation(program, "world_mat");
		GLint tex = glGetUniformLocation(program, "tex");
		//Applies the model's top stack to the unfiformmatrix. 
		glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(model->Apply(true)));

		// Timer
		GLint timer_var = glGetUniformLocation(program, "timer");
		double current_time = glfwGetTime();
		glUniform1f(timer_var, (float)current_time);
		glDisable(GL_TEXTURE_2D);
	}


	void Geom::CreateCube(float w, float h, float d) {

		// Create a cube

		// Number of vertices and faces to be created
		const GLuint vertex_num = 8;
		const GLuint face_num = 6 * 2;

		// Number of attributes for vertices and faces
		const int vertex_att = 11;  // 11 attributes per vertex: 3D position (3), 3D normal (3), RGB color (3), 2D texture coordinates (2)
		const int face_att = 3; // Vertex indices (3)

								// Data buffers for the torus
		GLfloat *vertex = NULL;
		GLuint *face = NULL;

		// Allocate memory for buffers
		try {
			vertex = new GLfloat[vertex_num * vertex_att];
			face = new GLuint[face_num * face_att];
		}
		catch (std::exception &e) {
			throw e;
		}

		// Create vertices 

		float s, t; // parameters zero to one
		glm::vec3 loop_center;
		glm::vec3 vertex_position;
		glm::vec3 vertex_normal;
		glm::vec3 vertex_color;
		glm::vec2 vertex_coord;

		int itr = 0;
		for (int i = 0; i < 2; i++) { // along the side

			for (int j = 0; j < 2; j++) { // small circle
				for (int k = 0; k < 2; k++) {

					vertex_color = glm::vec3((float)i / 2, (float)j / 2, (float)k / 2);



					vertex_normal = glm::vec3(1, 0, 0);
					vertex_position = glm::vec3(
						w*(float)(i * 2 - 1) / 2,
						h*(float)(j * 2 - 1) / 2,
						d*(float)(k * 2 - 1) / 2);

					vertex_coord = glm::vec2((((float)i + k) / 2, j));
					//vertex_coord = glm::vec2((fmin((float)i+k,1), j));
					//std::cout << vertex_coord.r << ", " << vertex_coord.g << std::endl;
					for (int l = 0; l < 3; l++) {
						int index = (itr)*vertex_att + l;
						//std::cout << index << std::endl;
						vertex[index] = vertex_position[l];
						vertex[index + 3] = vertex_normal[l];
						vertex[index + 6] = vertex_color[l];
					}
					vertex[(itr)*vertex_att + 9] = vertex_coord[0];
					vertex[(itr)*vertex_att + 10] = vertex_coord[1];

					itr += 1;

				}

			}
		}

		std::string a = "";

		glm::vec3 t1, t2;

		std::vector<glm::vec3> faces;
		faces.push_back(glm::vec3(0, 1, 2));//left
		faces.push_back(glm::vec3(1, 3, 2));//left
		faces.push_back(glm::vec3(0, 4, 5));//bottom
		faces.push_back(glm::vec3(0, 1, 5));//bottom
		faces.push_back(glm::vec3(4, 5, 7));//right
		faces.push_back(glm::vec3(4, 6, 7));//right
		faces.push_back(glm::vec3(2, 3, 7));//top
		faces.push_back(glm::vec3(2, 6, 7));//top
		faces.push_back(glm::vec3(1, 3, 7));//front
		faces.push_back(glm::vec3(1, 5, 7));//front
		faces.push_back(glm::vec3(0, 2, 6));//back
		faces.push_back(glm::vec3(0, 4, 6));//back

		for (int i = 0; i < faces.size(); i++) {
			for (int k = 0; k < 3; k++) {
				int index = (i)*face_att + k;
				//std::cout << index << std::endl;
				face[index] = (GLuint)faces[i][k];
			}
		}

		size_ = face_num * face_att;

		// Create OpenGL buffer for vertices
		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, vertex_num * vertex_att * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
		// Create OpenGL buffer for faces
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att * sizeof(GLuint), face, GL_STATIC_DRAW);

		// Free data buffers
		delete[] vertex;
		delete[] face;

		glActiveTexture(GL_TEXTURE0);

	}
	void Geom::CreateTorus(float loop_radius, float circle_radius, int num_loop_samples, int num_circle_samples) {

		// Create a torus
		// The torus is built from a large loop with small circles around the loop

		// Number of vertices and faces to be created
		// Check the construction algorithm below to understand the numbers
		// specified below
		const GLuint vertex_num = num_loop_samples * num_circle_samples;
		const GLuint face_num = num_loop_samples * num_circle_samples * 2;

		// Number of attributes for vertices and faces
		const int vertex_att = 11;
		const int face_att = 3;

		// Data buffers for the torus
		GLfloat *vertex = NULL;
		GLuint *face = NULL;

		// Allocate memory for buffers
		try {
			vertex = new GLfloat[vertex_num * vertex_att]; // 11 attributes per vertex: 3D position (3), 3D normal (3), RGB color (3), 2D texture coordinates (2)
			face = new GLuint[face_num * face_att]; // 3 indices per face
		}
		catch (std::exception &e) {
			throw e;
		}

		// Create vertices 
		float theta, phi; // Angles for circles
		glm::vec3 loop_center;
		glm::vec3 vertex_position;
		glm::vec3 vertex_normal;
		glm::vec3 vertex_color;
		glm::vec2 vertex_coord;

		for (int i = 0; i < num_loop_samples; i++) { // large loop

			theta = 2.0*glm::pi<GLfloat>()*i / num_loop_samples; // loop sample (angle theta)
			loop_center = glm::vec3(loop_radius*cos(theta), loop_radius*sin(theta), 0); // centre of a small circle

			for (int j = 0; j < num_circle_samples; j++) { // small circle

				phi = 2.0*glm::pi<GLfloat>()*j / num_circle_samples; // circle sample (angle phi)

																	 // Define position, normal and color of vertex
				vertex_normal = glm::vec3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
				vertex_position = loop_center + vertex_normal * circle_radius;
				vertex_color = glm::vec3(1.0 - ((float)i / (float)num_loop_samples),
					(float)i / (float)num_loop_samples,
					(float)j / (float)num_circle_samples);
				vertex_coord = glm::vec2(theta / (2.0*glm::pi<GLfloat>()),
					phi / (2.0*glm::pi<GLfloat>()));

				// Add vectors to the data buffer
				for (int k = 0; k < 3; k++) {
					vertex[(i*num_circle_samples + j)*vertex_att + k] = vertex_position[k];
					vertex[(i*num_circle_samples + j)*vertex_att + k + 3] = vertex_normal[k];
					vertex[(i*num_circle_samples + j)*vertex_att + k + 6] = vertex_color[k];
				}
				vertex[(i*num_circle_samples + j)*vertex_att + 9] = vertex_coord[0];
				vertex[(i*num_circle_samples + j)*vertex_att + 10] = vertex_coord[1];
			}
		}

		// Create triangles
		for (int i = 0; i < num_loop_samples; i++) {
			for (int j = 0; j < num_circle_samples; j++) {
				// Two triangles per quad
				glm::vec3 t1(((i + 1) % num_loop_samples)*num_circle_samples + j,
					i*num_circle_samples + ((j + 1) % num_circle_samples),
					i*num_circle_samples + j);
				glm::vec3 t2(((i + 1) % num_loop_samples)*num_circle_samples + j,
					((i + 1) % num_loop_samples)*num_circle_samples + ((j + 1) % num_circle_samples),
					i*num_circle_samples + ((j + 1) % num_circle_samples));
				// Add two triangles to the data buffer
				for (int k = 0; k < 3; k++) {
					face[(i*num_circle_samples + j)*face_att * 2 + k] = (GLuint)t1[k];
					face[(i*num_circle_samples + j)*face_att * 2 + k + face_att] = (GLuint)t2[k];
				}
			}
		}

		// Create OpenGL buffers and copy data


		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, vertex_num * vertex_att * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att * sizeof(GLuint), face, GL_STATIC_DRAW);

		// Free data buffers
		delete[] vertex;
		delete[] face;

		// Save info for rendering later
		size_ = face_num * face_att;

	}

	void Geom::CreateCylinder(float height = 0.6, float circle_radius = 0.2, int num_height_samples = 20, int num_circle_samples = 30) {

		// Create a cylinder

		// Number of vertices and faces to be created
		const GLuint vertex_num = num_height_samples * num_circle_samples + 2; // plus two for top and bottom
		const GLuint face_num = num_height_samples * (num_circle_samples - 1) * 2 + 2 * num_circle_samples; // two extra rings worth for top and bottom

																											// Number of attributes for vertices and faces
		const int vertex_att = 11;  // 11 attributes per vertex: 3D position (3), 3D normal (3), RGB color (3), 2D texture coordinates (2)
		const int face_att = 3; // Vertex indices (3)

								// Data buffers for the torus
		GLfloat *vertex = NULL;
		GLuint *face = NULL;

		// Allocate memory for buffers
		try {
			vertex = new GLfloat[vertex_num * vertex_att];
			face = new GLuint[face_num * face_att];
		}
		catch (std::exception &e) {
			throw e;
		}

		// Create vertices 
		float theta; // Angle for circle
		float h; // height
		float s, t; // parameters zero to one
		glm::vec3 loop_center;
		glm::vec3 vertex_position;
		glm::vec3 vertex_normal;
		glm::vec3 vertex_color;
		glm::vec2 vertex_coord;

		for (int i = 0; i < num_height_samples; i++) { // along the side

			s = i / (float)num_height_samples; // parameter s (vertical)
			h = (-0.5 + s)*height;
			for (int j = 0; j < num_circle_samples; j++) { // small circle
				t = j / (float)num_circle_samples;
				theta = 2.0*glm::pi<GLfloat>()*t; // circle sample (angle theta)

												  // Define position, normal and color of vertex
				vertex_normal = glm::vec3(cos(theta), 0.0f, sin(theta));
				vertex_position = glm::vec3(cos(theta)*circle_radius, h, sin(theta)*circle_radius);
				vertex_color = glm::vec3(1.0 - s,
					t,
					s);
				vertex_coord = glm::vec2(s, t);

				// Add vectors to the data buffer
				for (int k = 0; k < 3; k++) {
					vertex[(i*num_circle_samples + j)*vertex_att + k] = vertex_position[k];
					vertex[(i*num_circle_samples + j)*vertex_att + k + 3] = vertex_normal[k];
					vertex[(i*num_circle_samples + j)*vertex_att + k + 6] = vertex_color[k];
				}
				vertex[(i*num_circle_samples + j)*vertex_att + 9] = vertex_coord[0];
				vertex[(i*num_circle_samples + j)*vertex_att + 10] = vertex_coord[1];
			}
		}

		int topvertex = num_circle_samples * num_height_samples;
		int bottomvertex = num_circle_samples * num_height_samples + 1; // indices for top and bottom vertex

		vertex_position = glm::vec3(0, height / 2, 0); // location of top middle of cylinder
		vertex_normal = glm::vec3(0, 1, 0);
		vertex_color = glm::vec3(1, 0.6, 0.4);
		vertex_coord = glm::vec2(0, 0); // no good way to texture top and bottom

		for (int k = 0; k < 3; k++) {
			vertex[topvertex*vertex_att + k] = vertex_position[k];
			vertex[topvertex*vertex_att + k + 3] = vertex_normal[k];
			vertex[topvertex*vertex_att + k + 6] = vertex_color[k];
		}
		vertex[(topvertex)*vertex_att + 9] = vertex_coord[0];
		vertex[(topvertex)*vertex_att + 10] = vertex_coord[1];

		//================== bottom vertex
		vertex_position = glm::vec3(0, -height / 2, 0); // location of top middle of cylinder
		vertex_normal = glm::vec3(0, -1, 0);
		// leave the color and uv alone

		for (int k = 0; k < 3; k++) {
			vertex[bottomvertex*vertex_att + k] = vertex_position[k];
			vertex[bottomvertex*vertex_att + k + 3] = vertex_normal[k];
			vertex[bottomvertex*vertex_att + k + 6] = vertex_color[k];
		}
		vertex[(bottomvertex)*vertex_att + 9] = vertex_coord[0];
		vertex[(bottomvertex)*vertex_att + 10] = vertex_coord[1];

		//===================== end of vertices

		// Create triangles
		for (int i = 0; i < num_height_samples - 1; i++) {
			for (int j = 0; j < num_circle_samples; j++) {
				// Two triangles per quad
				glm::vec3 t1(((i + 1) % num_height_samples)*num_circle_samples + j,
					i*num_circle_samples + ((j + 1) % num_circle_samples),
					i*num_circle_samples + j);
				glm::vec3 t2(((i + 1) % num_height_samples)*num_circle_samples + j,
					((i + 1) % num_height_samples)*num_circle_samples + ((j + 1) % num_circle_samples),
					i*num_circle_samples + ((j + 1) % num_circle_samples));
				// Add two triangles to the data buffer
				for (int k = 0; k < 3; k++) {
					face[(i*num_circle_samples + j)*face_att * 2 + k] = (GLuint)t1[k];
					face[(i*num_circle_samples + j)*face_att * 2 + k + face_att] = (GLuint)t2[k];
				}
			}
		}
		int cylbodysize = num_circle_samples * (num_height_samples - 1) * 2; // amount of array filled so far, start adding from here
																			 // triangles for top disc (fan shape)
		int i = num_height_samples - 1;
		for (int j = 0; j < num_circle_samples; j++) {
			// Bunch of wedges pointing to the centre
			glm::vec3 topwedge(
				i*num_circle_samples + j,
				topvertex,
				i*num_circle_samples + (j + 1) % num_circle_samples
			);

			// note order reversed so that all triangles point outward
			glm::vec3 botwedge(
				0 + (j + 1) % num_circle_samples,
				bottomvertex,
				0 + j
			);

			// Add the triangles to the data buffer
			for (int k = 0; k < 3; k++) {
				face[(cylbodysize + j)*face_att + k] = (GLuint)topwedge[k];
				face[(cylbodysize + j + num_circle_samples)*face_att + k] = (GLuint)botwedge[k];
			}
		}

		// Create model 
		size_ = face_num * face_att;

		// Create OpenGL buffer for vertices
		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, vertex_num * vertex_att * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

		// Create OpenGL buffer for faces
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att * sizeof(GLuint), face, GL_STATIC_DRAW);

		// Free data buffers
		delete[] vertex;
		delete[] face;
	}



	void Geom::CreateSphere(float radius, int num_samples_theta, int num_samples_phi) {

		// Create a sphere using a well-known parameterization

		// Number of vertices and faces to be created
		const GLuint vertex_num = num_samples_theta * num_samples_phi;
		const GLuint face_num = num_samples_theta * (num_samples_phi - 1) * 2;

		// Number of attributes for vertices and faces
		const int vertex_att = 11;
		const int face_att = 3;

		// Data buffers 
		GLfloat *vertex = NULL;
		GLuint *face = NULL;

		// Allocate memory for buffers
		try {
			vertex = new GLfloat[vertex_num * vertex_att]; // 11 attributes per vertex: 3D position (3), 3D normal (3), RGB color (3), 2D texture coordinates (2)
			face = new GLuint[face_num * face_att]; // 3 indices per face
		}
		catch (std::exception &e) {
			throw e;
		}

		// Create vertices 
		float theta, phi; // Angles for parametric equation
		glm::vec3 vertex_position;
		glm::vec3 vertex_normal;
		glm::vec3 vertex_color;
		glm::vec2 vertex_coord;

		for (int i = 0; i < num_samples_theta; i++) {

			theta = 2.0*glm::pi<GLfloat>()*i / (num_samples_theta - 1); // angle theta

			for (int j = 0; j < num_samples_phi; j++) {

				phi = glm::pi<GLfloat>()*j / (num_samples_phi - 1); // angle phi

				// Define position, normal and color of vertex
				vertex_normal = glm::vec3(cos(theta)*sin(phi), sin(theta)*sin(phi), -cos(phi));
				// We need z = -cos(phi) to make sure that the z coordinate runs from -1 to 1 as phi runs from 0 to pi
				// Otherwise, the normal will be inverted
				vertex_position = glm::vec3(vertex_normal.x*radius,
					vertex_normal.y*radius,
					vertex_normal.z*radius),
					vertex_color = glm::vec3(((float)i) / ((float)num_samples_theta), 1.0 - ((float)j) / ((float)num_samples_phi), ((float)j) / ((float)num_samples_phi));
				vertex_coord = glm::vec2(((float)i) / ((float)num_samples_theta), 1.0 - ((float)j) / ((float)num_samples_phi));

				// Add vectors to the data buffer
				for (int k = 0; k < 3; k++) {
					vertex[(i*num_samples_phi + j)*vertex_att + k] = vertex_position[k];
					vertex[(i*num_samples_phi + j)*vertex_att + k + 3] = vertex_normal[k];
					vertex[(i*num_samples_phi + j)*vertex_att + k + 6] = vertex_color[k];
				}
				vertex[(i*num_samples_phi + j)*vertex_att + 9] = vertex_coord[0];
				vertex[(i*num_samples_phi + j)*vertex_att + 10] = vertex_coord[1];
			}
		}

		// Create faces
		for (int i = 0; i < num_samples_theta; i++) {
			for (int j = 0; j < (num_samples_phi - 1); j++) {
				// Two triangles per quad
				glm::vec3 t1(((i + 1) % num_samples_theta)*num_samples_phi + j,
					i*num_samples_phi + (j + 1),
					i*num_samples_phi + j);
				glm::vec3 t2(((i + 1) % num_samples_theta)*num_samples_phi + j,
					((i + 1) % num_samples_theta)*num_samples_phi + (j + 1),
					i*num_samples_phi + (j + 1));
				// Add two triangles to the data buffer
				for (int k = 0; k < 3; k++) {
					face[(i*(num_samples_phi - 1) + j)*face_att * 2 + k] = (GLuint)t1[k];
					face[(i*(num_samples_phi - 1) + j)*face_att * 2 + k + face_att] = (GLuint)t2[k];
				}
			}
		}

		// Create OpenGL buffers and copy data
		//GLuint vao;
		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);

		// Create model 
		size_ = face_num * face_att;

		// Create OpenGL buffer for vertices
		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, vertex_num * vertex_att * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

		// Create OpenGL buffer for faces
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att * sizeof(GLuint), face, GL_STATIC_DRAW);

		// Free data buffers
		delete[] vertex;
		delete[] face;

		
	}

} // namespace game;
