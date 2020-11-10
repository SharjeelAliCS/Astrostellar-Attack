/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 2: A multi-component model
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates camera model with the tori.
 * For further info, please read the README file provided.
 *
 */

#ifndef RADAR_NODE_H_
#define RADAR_NODE_H_
#include "screen_node.h"

namespace game {

	// Class for a single game object 
	class RadarNode : public ScreenNode {

	public:
		// Create game object
		RadarNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);

		// Destructor
		~RadarNode();
		void Draw(Camera *camera);
		void Update(float deltaTime);
		void SetupShader(GLuint program, Camera* camera);
		
		void AddDotPos(glm::vec2 dot);
		void AddDotColor(glm::vec3 color);

		glm::mat4 CalculateFinalTransformation(Camera* camera);

	private:
		
		std::vector<glm::vec2> dot_pos_;
		std::vector<glm::vec3> dot_color_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
