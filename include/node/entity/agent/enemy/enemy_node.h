/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef ENEMY_NODE_H_
#define ENEMY_NODE_H_
#include "agent_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 

	typedef void(Enemy::*EnemyState)(float);

	class Player;
	class Enemy : public AgentNode {

	public:
		// Create game object
		Enemy(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~Enemy();

		void InitState(void);

		void FollowPlayer(float deltaTime);
		void AttackPlayer(float deltaTime);
		void FindPlayer(float deltaTime);
		void MoveToRandomDirection(float deltaTime);
		void MoveAwayFromPlayer(float deltaTime);

		void Update(float deltaTime);
		void Draw(Camera* camera);
		void SetPlayer(Player* p) { player_ = p; }

		void Fire(float deltaTime);
	protected:
		EnemyState active_state_;
		Player* player_;

		float rate_of_fire_;
		float time_since_fire_;
		float follow_duration_;
		float move_away_duration_;
		
		float time_since_last_move_;

		float detect_distance_;
		float min_distance_;

		glm::vec3 CalculateAimPosition(float speed);

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
