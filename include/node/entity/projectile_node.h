/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef PROJECTILE_OBJ_H_
#define PROJECTILE_OBJ_H_
#include "entity_node.h"
#include <map>
#include <functional>

namespace game {

	// Class for a single projectile object 
	class Projectile : public Entity {

	public:
		// Create game object
		Projectile(const std::string name, const std::string type, const std::map<std::string, int> upgrades, glm::quat orient, const Resource *geometry, const Resource *material,const Resource *texture = NULL);

		// Destructor
		~Projectile();

		//defines stats for player attack types
		void init(const std::string type, const std::map<std::string, int> upgrades);

		void Update(float deltaTime);

		//Set the max range it can be as a function of time
		void SetTTL(double t);

		void displayStats();

	private:
		float lastTime=0; //just for debugging

		//time to live, a cheaper way to simulate range
		double ttl;
		//
		int pierce;
		float dotDmg;
		float dotDuration;
		int dotStackMax;
		SceneNode *target;
		std::map<std::string, int> upg;

		//damage is a function because of the sniper existing, and maybe aoe shots will have more damage in the center of the blast, etc
		std::function<double()> dmg;


		//movement is a function because of the pursuer existing
		std::function<void(float)> move;



	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
