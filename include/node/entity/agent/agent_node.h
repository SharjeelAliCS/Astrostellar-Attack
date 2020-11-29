/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef AGENT_NODE_H_
#define AGENT_NODE_H_
#include "entity_node.h"
#include "projectile_node.h"
#include "resource_manager.h"
#include "camera.h"
namespace game {
	// Class for a single player object 
	class AgentNode : public Entity {


	public:
		// Create game object
		AgentNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		// Destructor
		~AgentNode();

		std::vector<Projectile*>* GetMissiles(void)  { return &missiles; }

		void Update(float deltaTime);
		void Draw(Camera* camera);

	protected:

		std::map<std::string, int> upgrades;
		//store all the missiles in here. 
		std::vector<Projectile*> missiles;

		const Resource *geo;
		const Resource *mat;
		const Resource *tex;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
