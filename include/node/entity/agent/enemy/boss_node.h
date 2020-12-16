/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef BOSS_NODE_H_
#define BOSS_NODE_H_
#include "enemy_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 

	class Boss : public Enemy {

	public:
		// Create game object
		Boss(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~Boss();

		void InitState(void);

		std::vector<Enemy*>* GetOrbs(void) { return &orbs_; }
		void AddOrb(Enemy* o) { orbs_.push_back(o); }

		void Draw(Camera* camera);
		void Update(float deltaTime);
	protected:
		std::vector<Enemy*> orbs_;
		glm::mat4 CalculateFinalTransformation(Camera* camera);
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
