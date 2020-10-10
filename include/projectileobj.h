/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef PROJECTILE_OBJ_H_
#define PROJECTILE_OBJ_H_
#include <gameobj.h>

namespace game {

	// Class for a single projectile object 
	class ProjectileObj : public GameObj {

	public:
		// Create game object
		ProjectileObj(GLuint shader, Transformation* m);
		ProjectileObj();

		// Destructor
		~ProjectileObj();

		void Update(double deltaTime);

		//Set the max range it can be
		void SetRange(float range);

	private:

		//maxium range for position before the object is deleted. 
		float range_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
