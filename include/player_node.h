/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef PLAYER_OBJ_H_
#define PLAYER_OBJ_H_
#include <enemy_node.h>
#include <projectile_node.h>
#include <scene_node.h>

namespace game {

	// Class for a single player object 
	class Player  : public SceneNode{

	public:
		// Create game object
		Player(const std::string name, const Resource *geometry, const Resource *material, Transformation* model);
		Player();
		
		// Destructor
		~Player();

		//This lag behind function is essentially claculating the orientation with an input pitch and yaw,
		//but does not save it. This is used for custom settings for the camera lagging behind the player object. 
		glm::quat RotLagBehind(float pitch, float yaw);
		// Draw the node according to scene parameters in 'camera'
		// variable
		void Draw(Camera *camera);

		//The collision function detects all collisions with the enemies. 
		bool Collision(std::vector<Enemy*>* enemies);

		//Fires a single missile. 
		void Fire();



		// Update the node
		void Update(double deltaTime);


	private:

		//store all the missiles in here. 
		std::vector<Projectile*> missiles;

	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
