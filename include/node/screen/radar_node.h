/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a radar node class
 */

#ifndef RADAR_NODE_H_
#define RADAR_NODE_H_
#include "screen_node.h"

namespace game {

	// Class for a single game object 
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class RadarNode : public ScreenNode {

	public:
		// Create game object
		RadarNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);

		// Destructor
		~RadarNode();
		void Draw(Camera *camera);
		void Update(float deltaTime);
		void SetupShader(GLuint program, Camera* camera);
		
		/*Input:       dot (vec2 dot position to add to radar)
		  Output:      void
		  Description: Adds a dot to the dot list to render on screen
		*/
		void AddDotPos(glm::vec2 dot);

		/*Input:       color (vec2 color to add to radar for specific dot)
		  Output:      void
		  Description: Adds a color to the color list to render on screen
		*/
		void AddDotColor(glm::vec3 color);

		glm::mat4 CalculateFinalTransformation(Camera* camera);

	private:
		
		std::vector<glm::vec2> dot_pos_; //list of dot positions
		std::vector<glm::vec3> dot_color_; //list of color positions
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
