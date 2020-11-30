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
#include "agent_node.h"
#include "asteroid_node.h"
#include "comet_node.h"
#include "resource_manager.h"
#include "camera.h"
class Game;
class ResourceManager;

namespace game {
	extern ResourceManager resman_;
	// Class for a single player object 
	class Player  : public AgentNode{
		

	public:
		// Create game object
		Player(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		// Destructor
		~Player();

		void SetShields(float h);
		float GetShields(void) const;
		float GetShieldPercent(void) const;
		//This lag behind function is essentially claculating the orientation with an input pitch and yaw,
		//but does not save it. This is used for custom settings for the camera lagging behind the player object. 
		glm::quat RotLagBehind(float pitch, float yaw);
		// Draw the node according to scene parameters in 'camera'
		// variable
		void Draw(Camera *camera);

		//The collision function detects all collisions with the enemies. 
		bool Collision();
		//Fires a single missile. 
		void Fire();
		void nextWeapon();

		void SetBoosted(int i);
		int GetBoosted(void) const;
		float GetBoostSpeed(void) const;
		std::map<std::string, int> getUpgrades(void) const {return upgrades;}

		float getCurSpeed() const;

		// Update the node
		void Update(float deltaTime);

		void setCam(Camera* c);

		void setAsteroids(std::vector<AsteroidNode*>* a);
		void setComets(std::vector<CometNode*>* a);
		void setEnemies(std::vector<Enemy*>* e);
		void setDeathAnimations(std::vector<ParticleNode*>* p);
		std::string GetCurrentWeapon(void) const;
		void damage(float dmg, bool health=true);
		bool NuclearOverload(void);
		// Update the node
		void SetFirstPerson(bool f) { first_person_ = f; }

		float getBoostPercent(void);
		float getNuclearOverloadPercent(void);
		Camera* GetCam(void) { return c_; }
		bool IsFirstPerson(void){return first_person_;}
		
	private:
		bool first_person_;
		int projType;
		const int numWeapons = 6;
		std::string projectileTypes[6] = { "laserBattery", "pursuer", "chargeBlast", "sniperShot", "shotgun", "naniteTorpedo"};

		std::map<int, bool> unlockedWeapons;
		std::map<int, float> rof;
		double lastShotTime;

		//0: no boost. 1: boosted. 2: boost off
		int boosted_;
		float boost_speed_;
		float boost_duration_;
		float boost_duration_left_;

		float nuclear_buildup_duration_;
		float nuclear_buildup_left_;

		float shield_recharge_speed_;
		float time_since_damage_;
		float shield_recharge_delay_;
		//player upgrades

		//default geom used for projectiles, will be replaced 
		

		std::vector<AsteroidNode*>* asteroids;
		std::vector<Enemy*>* enemies;
		std::vector<CometNode*>* comets;
		std::vector<ParticleNode*>* death_animations_;

		Camera* c_;


		float shields_;
		float max_shields_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
