/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains an entity node class
 */

#ifndef ENTITY_NODE_H_
#define ENTITY_NODE_H_
#include "scene_node.h"
#include "particle_node.h"

namespace game {

	
	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	//NOTE: CLASS IS DERIVED FROM SCENENODE. FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class Entity : public SceneNode {

	public:
		// Create game object
		Entity(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~Entity();

		/*Input:       orientation_speed (speed of rotaiton), orientaiton_normal (axis of rotation)
		  Output:      void
		  Description: Rotate the node over time
		*/
		void RotateOverTime(float Orientation_speed, glm::vec3 Orientation_normal);

		/*Input:       s (movement speed)
		  Output:      void
		  Description: Set the movement speed of an entity
		*/
		void SetMovementSpeed(float s);
		bool Exists(void) const;

		/*Input:       h (health to set)
		  Output:      void
		  Description: Set the health
		*/
		void SetHealth(float h);

		/*Input:       h (health to reduce by)
		  Output:      void
		  Description: reduce the health
		*/
		void ReduceHealth(float h);

		/*Input:       h (max health to set)
		  Output:      void
		  Description: Set the max health
		*/
		void SetMaxHealth(float h);

		/*Input:       void
		  Output:      h (current health)
		  Description: Get the health
		*/
		float GetHealth(void) const;

		/*Input:       void
		  Output:      h (current health)
		  Description: Get the health as a percent of max health
		*/
		float getHealthPercent(void) const;

		/*Input:       void
		  Output:      h (max health)
		  Description: Get the max health
		*/
		float GetMaxHealth(void) { return max_health_; }

		/*Input:       void
		  Output:      map (map of drops)
		  Description: Get the drops that the entity provides to the player upon death/defeat
		*/
		std::map<std::string, int> GetDrops(void) { return drops; }

		/*Input:       map (map of drops)
		  Output:      void
		  Description: Set the drops that the entity provides to the player upon death/defeat
		*/
		void SetDrops(std::map<std::string, int> m) { drops = m; }
		
		/*Input:       particles (particle system to set)
		  Output:      void
		  Description: Set the particle system of a node
		*/
		void SetParticleSystem(ParticleNode* particles);

		/*Input:       void
		  Output:      particles (particle system to get)
		  Description: Get the particle system of a node
		*/
		ParticleNode* GetParticle(void);
		void Update(float deltaTime);

		/*Input:       dmg (amount to damage by), health (ignore health flag)
		  Output:      void
		  Description: Damage the entity by a given damage amount
		*/
		virtual bool damage(float dmg, bool health= true);

		/*Input:       void
		  Output:      float (damage of entity)
		  Description: Get the damage an entity does
		*/
		virtual float GetDamage(void);

		bool SeeHealth(bool see);

	protected:

		float health_;//current health
		float max_health_;//max health
		bool exists_;
		ParticleNode* particles_; 
		std::map<std::string, int> drops;//item drops that the entity provides player

		float see_health_duration_; //health duration for screen space 
		float see_health_left_;//health duration for screen space 

		glm::mat4 CalculateFinalTransformation(Camera* camera);


	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
