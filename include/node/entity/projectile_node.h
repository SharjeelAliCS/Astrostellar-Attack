/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a projectile node class
 */

#ifndef PROJECTILE_OBJ_H_
#define PROJECTILE_OBJ_H_
#include "entity_node.h"
#include <map>
#include <functional>
#include "asteroid_node.h"
#include "comet_node.h"


namespace game {

	class Player;
	class Enemy;
	// Class for a single projectile object 
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class Projectile : public Entity {

	public:
		// Create game object
		Projectile(const std::string name, const std::string type, std::map<std::string, int> upgrades, std::map<std::string, float> weaponStats, bool boosted,
			const Resource *geometry, const Resource *material,const Resource *texture = NULL, const Resource *normal = NULL);

		// Destructor
		~Projectile();

		/*Input:       void
		  Output:      void
		  Description: Initalizes the projectile information
		*/
		void init();

		/*Input:       ast (asteroid list)
		  Output:      void
		  Description: Set the asteroid list of a projectile
		*/
		void SetAsteroids(std::vector<AsteroidNode*>* ast) { asteroids = ast; }

		/*Input:       cmt (comet list)
		  Output:      void
		  Description: Set the comet list of a projectile
		*/
		void SetComets(std::vector<CometNode*>* cmt) { comets = cmt; }

		/*Input:       en (Enemy list)
		  Output:      void
		  Description: Set the Enemy list of a projectile
		*/
		void SetEnemies(std::vector<Enemy*>* en) { enemies = en; }

		/*Input:       p (player node)
		  Output:      void
		  Description: Set the target player of a projectile
		*/
		void SetPlayer(Player* p);

		/*Input:       s (speed of projectile)
		  Output:      void
		  Description: Set the speed of a projectile
		*/
		void SetSpeed(float s) { speed = s; }

		/*Input:       void
		  Output:      float (Speed of projectile)
		  Description: Get the speed of a projectile
		*/
		float GetSpeed(void) { return speed; }
		void Update(float deltaTime);

		/*Input:       void
		  Output:      string (type of projctile)
		  Description: Get the type of a projectile
		*/
		std::string GetType(void) { return type; }


		/*Input:       t (maximum range in time)
		  Output:      void
		  Description: Set the max range in time projectile exists for
		*/
		void SetTTL(double t);

		/*Input:       void
		  Output:      t (maximum range in time)
		  Description: Get the max range in time projectile exists for
		*/
		double GetTTL(void) const { return ttl; }

		/*Input:       void
		  Output:      void
		  Description: Debug only. Prints out stats
		*/
		void displayStats();

		/*Input:       void
		  Output:      float (damage of projectile)
		  Description: Get the damage a projectile does
		*/
		float GetDamage(void);

		std::function<double()> disableFor;//time to disable for

	private:
		float lastTime=0; //just for debugging

		std::string type;
		//time to live, a cheaper way to simulate range
		double ttl;
		
		int pierce;//how many objects projectile attacks before it disspaears
		float speed;//speed of projectile
		bool boosted;//flag to increase speed if player boosts
		SceneNode *target;//target of projectile
		std::map<std::string, int> upg;//upgrade list
		std::map<std::string, float> stats;//stats list

		//damage is a function because of the sniper existing, and maybe aoe shots will have more damage in the center of the blast, etc
		std::function<double()> dmg;


		//movement is a function because of the pursuer existing
		std::function<void(float)> move;

		//important nodes for projectile
		std::vector<AsteroidNode*>* asteroids;
		std::vector<CometNode*>* comets;
		std::vector<Enemy*>* enemies;
		Player* player_;

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
