/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a boss node class
 */

#ifndef BOSS_NODE_H_
#define BOSS_NODE_H_
#include "enemy_node.h"
namespace game {

	
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class Boss : public Enemy {

	public:
		// Create game object
		Boss(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~Boss();

		void InitState(void);

		/*Input:       void
		  Output:      vector (list of enemy orbs)
		  Description: Get the list of enemy boss orbs that encircle and orbit the boss using hierchial transformations
		*/
		std::vector<Enemy*>* GetOrbs(void) { return &orbs_; }

		/*Input:       o (enemy orb to add)
		  Output:      void
		  Description: Add an enemy orb to the boss
		*/
		void AddOrb(Enemy* o) { orbs_.push_back(o); }

		void Draw(Camera* camera);
		void Update(float deltaTime);
	protected:
		std::vector<Enemy*> orbs_;//list of orbs that use hierchial tranformations to orbit player
		glm::mat4 CalculateFinalTransformation(Camera* camera);
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
