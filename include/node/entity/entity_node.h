/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef ENTITY_NODE_H_
#define ENTITY_NODE_H_
#include "scene_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class Entity : public SceneNode {

	public:
		// Create game object
		Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);
		~Entity();

		//Check if the object has been hit using a pos and a range.
		bool Hit(glm::vec3 pos, float range);
		//Rotate over time simply rotates over the given norma axis per update iteration. 
		void RotateOverTime(float Orientation_speed, glm::vec3 Orientation_normal);
		void SetMovementSpeed(float s);
		bool Exists(void) const;




	private:

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
