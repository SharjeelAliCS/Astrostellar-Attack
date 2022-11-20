/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains an enemy node class
 */

#ifndef ENEMY_NODE_H_
#define ENEMY_NODE_H_
#include "agent_node.h"
namespace game {

	//type def used to create the finate state machine for enemy AI
	typedef void(Enemy::*EnemyState)(float);

	class Player;

	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class Enemy : public AgentNode {

	public:
		// Create game object
		Enemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~Enemy();

		/*Input:       void
		  Output:      void
		  Description: Setup the finate state machine for enemy AI
		*/
		virtual void InitState(void);

		//methods for finate state machine

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: Follow the player, and fire a shot using the enemy rate of fire value. If the enemy is too close to player, turn to a random direction to avoid collision
		*/
		void FollowPlayer(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: Simply fire at the player then switch to follow player state
		*/
		void AttackPlayer(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: find the player in the map. If player not within distance, move in a circle. if player is too far away, move towards it so player doesnt need to spend ages finding enemies. 
		*/
		void FindPlayer(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: Move to a random direction
		*/
		void MoveToRandomDirection(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: if enemy is too close to player, move away 
		*/
		void MoveAwayFromPlayer(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: Do nothing. 
		*/
		void DoNothing(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: Fire a projectile at the player
		*/
		void Fire(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: Calculate a new direction to move in 
		*/
		void FindNewDirection(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: If the enemy type is the ram enemy, then ram the player instead of moving away if enemy is too close
		*/
		void RamPlayer(float deltaTime);

		/*Input:       deltatime (time between frames)
		  Output:      void
		  Description: For the boss specifically. If the player is too close to the boss, damage the player. 
		*/
		void PoisonAura(float deltaTime);

		void Update(float deltaTime);
		void Draw(Camera* camera);

		/*Input:       p (player node)
		  Output:      void
		  Description: Set the player node for an enemy
		*/
		void SetPlayer(Player* p) { player_ = p; }

		/*Input:       e (enemy string)
		  Output:      void
		  Description: Set the enemy type
		*/
		void SetEnemyType(std::string e) { enemy_type_ = e; }

		/*Input:       void
		  Output:      e (enemy string)
		  Description: Get the enemy type
		*/
		std::string GetEnemyType(void) { return enemy_type_; }

		/*Input:       d (projectile damage value)
		  Output:      void
		  Description: Set the projectile damage
		*/
		void SetProjectileDmg(float d) { projectile_dmg_ = d; }

		/*Input:       sec (time to disable attacks for)
		  Output:      void
		  Description: if player activated ability, then simply disable all enemy attacks for sec time
		*/
		void DisableAttackFor(double sec);


		std::map<std::string, int> GetDrops(void);

		/*Input:       p (phase number)
		  Output:      void
		  Description: Set the enemy phase. This is used for the splitter, which tells how many splits has happened. 
		*/
		void SetPhase(float p) { phase_ = p; }

		/*Input:       void
		  Output:      p (phase number)
		  Description: Gets the enemy phase
		*/
		float GetPhase(void){return phase_; }

		/*Input:       f (rate of fire)
		  Output:      void
		  Description: Sets the enemy rate of fire for attacks
		*/
		void SetRateOfFire(float f) { rate_of_fire_ = f; }

		/*Input:       c (projectile color)
		  Output:      void
		  Description: Set the enemy projectile color (to green by default)
		*/
		void SetProjColor(glm::vec3 c) { proj_color_ = c; }

	protected:
		int phase_; //phase number. this is used for the splitter enemy  type, and decreases for each split the enemy does. 

		EnemyState active_state_;//the Finate state machine active state. 

		Player* player_;
		std::string enemy_type_;//type of enemy

		float rate_of_fire_;//rate of fire 
		float time_since_fire_; //the time since the previous fire
		float follow_duration_; //how long to follow the player for
		float move_away_duration_;//how long to move away from the player for
		
		double attackDisabledFor_; //how long attacks are disabled for

		float time_since_last_move_; 
		float projectile_dmg_;//how much damage each projectile does

		float detect_distance_; //the distance at which enemy "sees" player
		float min_distance_; //min distance before enemy moves away from player
		float max_stationary_distance_; //the max distance that enemy circles itself. if player is further than this, the enemy follows the player just so that the player is never like 10,000 units away from the enemy

		glm::vec3 rotation_axis_;

		/*Input:       p (phase number)
		  Output:      void
		  Description: Calculates the  aim position at which the projectile must shoot the player. this uses velocites to calculate where the player will be after x seconds 
		  Source:      https://gamedev.stackexchange.com/questions/35859/algorithm-to-shoot-at-a-target-in-a-3d-game
		*/
		glm::vec3 CalculateAimPosition(float speed);

		glm::vec3 proj_color_;//projectile color


	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
