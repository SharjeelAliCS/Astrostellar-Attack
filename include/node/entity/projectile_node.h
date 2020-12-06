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
#include "asteroid_node.h"
#include "comet_node.h"


namespace game {

	class Player;
	class Enemy;
	// Class for a single projectile object 
	class Projectile : public Entity {

	public:
		// Create game object
		Projectile(const std::string name, const std::string type, std::map<std::string, int> upgrades, 
			const Resource *geometry, const Resource *material,const Resource *texture = NULL, const Resource *normal = NULL);

		// Destructor
		~Projectile();

		//defines stats for player attack types
		void init();
		void SetAsteroids(std::vector<AsteroidNode*>* ast) { asteroids = ast; }
		void SetComets(std::vector<CometNode*>* cmt) { comets = cmt; }
		void SetEnemies(std::vector<Enemy*>* en) { enemies = en; }
		void SetPlayer(Player* p);

		void setSpeed(float s) { speed = s; }
		float GetSpeed(void) { return speed; }
		void Update(float deltaTime);

		//Set the max range it can be as a function of time
		void SetTTL(double t);
		double GetTTL(void) const { return ttl; }
		void displayStats();
		float GetDamage(void);

	private:
		float lastTime=0; //just for debugging

		std::string type;
		//time to live, a cheaper way to simulate range
		double ttl;
		//
		int pierce;
		float dotDmg;
		float dotDuration;
		int dotStackMax;
		float speed;
		SceneNode *target;
		std::map<std::string, int> upg;

		//damage is a function because of the sniper existing, and maybe aoe shots will have more damage in the center of the blast, etc
		std::function<double()> dmg;


		//movement is a function because of the pursuer existing
		std::function<void(float)> move;

		std::vector<AsteroidNode*>* asteroids;
		std::vector<CometNode*>* comets;
		std::vector<Enemy*>* enemies;
		Player* player_;

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
