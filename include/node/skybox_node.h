/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef SKYBOX_NODE_H_
#define SKYBOX_NODE_H_
#include "scene_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class SkyBox : public SceneNode {

	public:
		// Create game object
		SkyBox(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);
		~SkyBox();

		
		void Draw(Camera *camera);



	private:
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
