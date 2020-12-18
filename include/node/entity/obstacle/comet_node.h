/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef COMET_NODE_
#define COMET_NODE_
#include "obstacle_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class CometNode : public ObstacleNode {

	public:
		// Create game object
		CometNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~CometNode();

		// Update the node
		void Update(float deltaTime);
		std::map<std::string, int> GetDrops(void);
	private:
		
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
