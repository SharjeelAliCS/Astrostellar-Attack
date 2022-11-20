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
#include "resource.h"
#include "camera.h"
namespace game {
	// Class for a single player object 
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class AgentNode : public Entity {


	public:
		// Create game object
		AgentNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		// Destructor
		~AgentNode();

		/*Input:       void
		  Output:      vector (missiles)
		  Description: Get all projectiles an agent has currently fired
		*/
		std::vector<Projectile*>* GetMissiles(void)  { return &missiles; }

		void Update(float deltaTime);
		void Draw(Camera* camera);

		/*Input:       rsc (projectile resources)
		  Output:      void
		  Description: Set the projectile resources to create a projectile
		*/
		void SetProjRsc(NodeResources* rsc) { proj_rsc_ = rsc; }

		float GetDamage(void) ;
		void SetDamage(float d) { damage_ = d; }

		/*Input:       mult (boost speed to increase to)
		  Output:      void
		  Description: Increase the overall boost speed by a multipier
		*/
		void IncreaseBoostSpeed(float mult) { boost_speed_ *= mult; }

		/*Input:       m (the map of upgrades)
		  Output:      void
		  Description: Set the agent upgrades map
		*/
		void SetUpgrades(std::map<std::string, int>* m) { upgrades = m; }

		/*Input:       void
		  Output:      NodeResources (projectile resources)
		  Description: Get the resources used to create a projectile
		*/
		NodeResources* GetProjRsc(void) { return proj_rsc_; }

	protected:
		float damage_;//damage entity can do

		float boost_speed_;//boost speed (to add to regular speed at certain times)
		std::map<std::string, int>* upgrades;//map of all upgrades
		std::map<std::string, float>* weaponStats;//map of all weapon stats
		
		std::vector<Projectile*> missiles;//store all the missiles in here. 

		NodeResources* proj_rsc_;//resources used to create a projectile
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
