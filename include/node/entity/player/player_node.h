/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef PLAYER_OBJ_H_
#define PLAYER_OBJ_H_
#include "enemy_node.h"
#include "projectile_node.h"
#include "entity_node.h"
#include "resource_manager.h"
class Game;
class ResourceManager;


namespace game {
	extern ResourceManager resman_;
	// Class for a single player object 
	class Player  : public Entity{
		

	public:
		// Create game object
		Player(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		// Destructor
		~Player();

		void SetShields(float h);
		float GetShields(void) const;
		//This lag behind function is essentially claculating the orientation with an input pitch and yaw,
		//but does not save it. This is used for custom settings for the camera lagging behind the player object. 
		glm::quat RotLagBehind(float pitch, float yaw);
		// Draw the node according to scene parameters in 'camera'
		// variable
		void Draw(Camera *camera);

		//The collision function detects all collisions with the enemies. 
		bool Collision(std::vector<Enemy*>* enemies);
		//Fires a single missile. 
		void Fire();
		void nextWeapon();

		void SetBoosted(int i);
		int GetBoosted(void) const;
		float GetBoostSpeed(void) const;
		std::string GetCurrentWeapon(void) const;
		// Update the node
		void Update(float deltaTime);
		
			


	private:
		//store all the missiles in here. 
		std::vector<Projectile*> missiles;

		int projType;
		const int numWeapons = 6;
		std::string projectileTypes[6] = { "laserBattery", "pursuer", "chargeBlast", "sniperShot", "shotgun", "naniteTorpedo"};

		std::map<int, bool> unlockedWeapons;
		std::map<int, float> rof;
		double lastShotTime;

		//0: no boost. 1: boosted. 2: boost off
		int boosted_;
		float boost_speed_;
		//player upgrades
		std::map<std::string, int> upgrades;

		//default geom used for projectiles, will be replaced 
		const Resource *geo;
		const Resource *mat;
		const Resource *tex;



		float shields_;
		float max_shields_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
