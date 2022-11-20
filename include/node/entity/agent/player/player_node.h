/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a player node class
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
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class Player  : public AgentNode{
		

	public:
		// Create game object
		Player(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		// Destructor
		~Player();

		/*Input:       h (shield to set to)
		  Output:      void
		  Description: Set the player shield value
		*/
		void SetShields(float h);

		/*Input:       void
		  Output:      float (shield to get)
		  Description: Get the player shield value
		*/
		float GetShields(void) const;

		/*Input:       void
		  Output:      float (shield to get)
		  Description: Get the player shield value as a percent
		*/
		float GetShieldPercent(void) const;
		
		/*Input:       pitch (pitch angle), yaw (yaw angle)
		  Output:      quat (quaternion to rotate camera by)
		  Description: This lag behind function is essentially claculating the orientation with an input pitch and yaw, but does not save it. This is used for custom settings for the camera lagging behind the player object. 
		*/
		glm::quat RotLagBehind(float pitch, float yaw);

		void Draw(Camera *camera);

		//The collision function detects all collisions with the enemies. 
		bool Collision();
		
		/*Input:       void
		  Output:      void
		  Description: Creates a projectile, and then fires it at the enemy
		*/
		void Fire();

		/*Input:       void
		  Output:      void
		  Description: Cycle to the next weapon in the inventory
		*/
		void nextWeapon();

		/*Input:       i (the boosted value)
		  Output:      void
		  Description: Set the player boost value
		*/
		void SetBoosted(int i);

		/*Input:       s (max shield value)
		  Output:      void
		  Description: Set the player max shield value
		*/
		void SetMaxShields(float s) { max_shields_ = s; shields_ = s; }

		/*Input:       void
		  Output:      int (boosted value)
		  Description: Get the phase of the player's boost 
		*/
		int GetBoosted(void) const;

		/*Input:       void
		  Output:      float (Boosted speed)
		  Description: Get the boosted speed of player
		*/
		float GetBoostSpeed(void) const;

		/*Input:       void
		  Output:      map (map of all player upgrades)
		  Description: Get all the upgrades for a player
		*/
		std::map<std::string, int>* getUpgrades(void) const {return upgrades;}

		/*Input:       void
		  Output:      float (current speed)
		  Description: Get the  player's current speed
		*/
		float getCurSpeed() const;

		void Update(float deltaTime);

		/*Input:       c (camera to set)
		  Output:      void
		  Description: Set the player camera
		*/
		void setCam(Camera* c);

		//set the following vectors using their inputs

		void setAsteroids(std::vector<AsteroidNode*>* a);
		void setComets(std::vector<CometNode*>* a);
		void setEnemies(std::vector<Enemy*>* e);
		void setDeathAnimations(std::vector<ParticleNode*>* p);

		/*Input:       void
		  Output:      string (current weapon name)
		  Description: Get the current weapon name
		*/
		std::string GetCurrentWeapon(void) const;

		bool damage(float dmg, bool health=true);

		/*Input:       void
		  Output:      bool (flag for nuclear overload when boosting)
		  Description: Check if player is currently nucelar overloading
		*/
		bool NuclearOverload(void);
		
		/*Input:       f (first person to set)
		  Output:      void
		  Description: Set the first person flag
		*/
		void SetFirstPerson(bool f) { first_person_ = f; }

		/*Input:       seconds (seconds in time)
		  Output:      void
		  Description: make player invulnerable after parry for seconds
		*/
		void MakeInvuln(double seconds);

		/*Input:       void
		  Output:      void
		  Description: After a parry, the next shot will be improved (deals more damage)
		*/
		void ImproveNextShot(void);

		//set the player's stats and inventories using the input map
		void SetPlayerInventory(std::map<std::string, int>* m) { playerInventory = m; }
		void SetPlayerLoadout(std::map<std::string, int>* m) { playerLoadout = m; }
		void SetPlayerStats(std::map<std::string, int>* m) { playerStats = m; }
		void SetWeaponStats(std::map<std::string, float>* m);

		/*Input:       m (loot from an entity)
		  Output:      void
		  Description: Collect  the entity loot
		*/
		void CollectLoot(std::map<std::string, int> m);

		/*Input:       void
		  Output:      float (boost percent)
		  Description: Get how much the player has boosted out of their max amount as a percent out of 1
		*/
		float getBoostPercent(void);

		/*Input:       void
		  Output:      float (nuclear overload percent)
		  Description: Get how much the player has nuclear overloaded out of their max amount as a percent out of 1
		*/
		float getNuclearOverloadPercent(void);

		/*Input:       void
		  Output:      camera (camera)
		  Description: return the cam
		*/
		Camera* GetCam(void) { return c_; }

		/*Input:       void
		  Output:      bool (flag for first person)
		  Description: Check if player is in first person
		*/
		bool IsFirstPerson(void){return first_person_;}
		
		/*Input:       void
		  Output:      int (weapon ammo)
		  Description: Get the current equipped weapon's ammo
		*/
		int getCurrentWeaponAmmo(void);

		/*Input:       string (currancy type)
		  Output:      int (number of currancy)
		  Description: Get the current amount of currancy using the type as input
		*/
		int getCurrency(std::string type) { return playerInventory->at(type); }
	private:
		bool first_person_; //flag for first person
		int projType; //type of projectile
		const int numWeapons = 6;//number of weapons
		std::string projectileTypes[6] = { "laser_Battery", "pursuer", "charge_Blast", "sniper_Shot", "shotgun", "nanite_Torpedo"};

		std::map<int, bool> unlockedWeapons; //map of unlocked weapons
		std::map<int, float> rof;//rate of fire for weapons

		double lastShotTime;//when the last shot took place
		double invulnFor;//how long player is invulnerable for
		bool boostNextShot;//improve the next shot or not

		//0: no boost. 1: boosted. 2: boost off
		int boosted_;

		float boost_duration_;//time to boost 
		float boost_duration_left_;//time left for boosting

		float nuclear_buildup_duration_;//duration of nuclear buildup
		float nuclear_buildup_left_;//amount of nuclear buildup left

		float shield_recharge_speed_;//speed of shield recharging
		float time_since_damage_;//time since player took damage
		float shield_recharge_delay_; //timie for the shield to recharge as a delay (x seconds after player last hit)
		//player upgrades

		//player inventory and stats maps
		std::map<std::string, int>* playerInventory;
		std::map<std::string, int>* playerLoadout;
		std::map<std::string, int>* playerStats;
		std::map<std::string, float>* weaponStats;

		//player node lists
		std::vector<AsteroidNode*>* asteroids;
		std::vector<Enemy*>* enemies;
		std::vector<CometNode*>* comets;
		std::vector<ParticleNode*>* death_animations_;

		Camera* c_;

		float shields_;//current sheild value
		float max_shields_;//max shield value
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
