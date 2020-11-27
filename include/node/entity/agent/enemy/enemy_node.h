/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef ENEMY_NODE_H_
#define ENEMY_NODE_H_
#include "agent_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class Enemy : public AgentNode {

	public:
		// Create game object
		Enemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~Enemy();


		



	private:

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
