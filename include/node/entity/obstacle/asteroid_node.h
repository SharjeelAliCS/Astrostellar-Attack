/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a asteroid node class
 */

#ifndef ASTEROID_NODE_
#define ASTEROID_NODE_
#include "obstacle_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class AsteroidNode : public ObstacleNode {

	public:
		// Create game object
		AsteroidNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~AsteroidNode();

		std::map<std::string, int> GetDrops(void);

	private:
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
