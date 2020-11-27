/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#ifndef PARTICLE_NODE_H_
#define PARTICLE_NODE_H_
#include "scene_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class ParticleNode : public SceneNode {

	public:
		// Create game object
		ParticleNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~ParticleNode();
		void Draw(Camera *camera);

		void Update(float deltaTime);
		void SetDuration(float d) { duration_ = d; max_duration_ = d; }
		float GetDuration(void) const { return duration_; }
		void SetStartTime(float t) { start_time_ = t; }
	private:
		float duration_;
		float max_duration_;
		float start_time_;
		void SetupShader(GLuint program, Camera* camera);
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
