/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef ENEMY_OBJ_H_
#define ENEMY_OBJ_H_
#include <gameobj.h>
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class EnemyObj : public GameObj {

	public:
		// Create game object
		EnemyObj(GLuint shader, Transformation* m);
		EnemyObj(GLuint shader, Transformation* m, glm::vec3 forward);
		EnemyObj();
		~EnemyObj();

		//Create planet creates the specific geometry/vbo/ebo used to create a "planet shape". 
		//Its main purpose is to create the Geom objects, for the main base (planet sphere) and the
		//ring that rotates around it. 
		void CreatePlanet(void);

		



	private:

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
