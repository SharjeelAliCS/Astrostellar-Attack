/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef PROJECTILE_OBJ_H_
#define PROJECTILE_OBJ_H_
#include <scene_node.h>

namespace game {

	// Class for a single projectile object 
	class Projectile : public SceneNode {

	public:
		// Create game object
		Projectile(const std::string name, const Resource *geometry, const Resource *material, Transformation* model);
		Projectile();

		// Destructor
		~Projectile();

		void Update(double deltaTime);

		//Set the max range it can be
		void SetRange(float range);

	private:

		//maxium range for position before the object is deleted. 
		float range_;
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
