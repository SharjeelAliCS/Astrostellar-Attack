/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 2: A multi-component model
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates camera model with the tori.
 * For further info, please read the README file provided.
 *
 */

#ifndef HUD_OBJ_H_
#define HUD_OBJ_H_
#include "scene_node.h"

namespace game {

	// Class for a single game object 
	class HUDNode : public SceneNode {

	public:
		// Create game object
		HUDNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);
		
		// Destructor
		~HUDNode();

		void Draw(Camera *camera);
		void Update(float deltaTime);

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
