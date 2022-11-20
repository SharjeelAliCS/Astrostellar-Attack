/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains an obstacle node class
 */

#ifndef OBSTACLE_NODE_H_
#define OBSTACLE_NODE_H_
#include "entity_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class ObstacleNode : public Entity {

	public:
		// Create game object
		ObstacleNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~ObstacleNode();

	private:
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
