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
#include "particle_node.h"

namespace game {

	
	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	class Entity : public SceneNode {

	public:
		// Create game object
		Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~Entity();


		//Rotate over time simply rotates over the given norma axis per update iteration. 
		void RotateOverTime(float Orientation_speed, glm::vec3 Orientation_normal);
		void SetMovementSpeed(float s);
		bool Exists(void) const;

		void SetHealth(float h);
		void ReduceHealth(float h);

		void SetMaxHealth(float h);
		float GetHealth(void) const;
		float getHealthPercent(void) const;

		void SetSpeed(float s);
		float GetSpeed(void) const;

		std::map<std::string, int> GetDrops(void) { return drops; }
		void SetDrops(std::map<std::string, int> m) { drops = m; }
		

		void SetParticleSystem(ParticleNode* particles);
		ParticleNode* GetParticle(void);
		void Update(float deltaTime);

		virtual bool damage(float dmg, bool health= true);
		virtual float GetDamage(void) ;
		bool SeeHealth(bool see);

	protected:

		float health_;
		float max_health_;
		//float speed_;
		bool exists_;
		ParticleNode* particles_;
		std::map<std::string, int> drops;

		float see_health_duration_;
		float see_health_left_;

		glm::mat4 CalculateFinalTransformation(Camera* camera);


	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
