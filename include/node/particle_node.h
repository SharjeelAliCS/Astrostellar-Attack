/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a particle node class
 */

#ifndef PARTICLE_NODE_H_
#define PARTICLE_NODE_H_
#include "scene_node.h"
namespace game {

	// Stores the enemy object, which for the demo is simply a "planet". It is a child of the GameObj class. 
	// NOTE: CLASS IS DERIVED FROM SCENENODE.FOR FUNCTIONS THAT DO NOT HAVE COMMENTS OR HEADERS BELOW, LOOK FOR DOCUMENTATION IN SCENENODE.H
	class ParticleNode : public SceneNode {

	public:
		// Create game object
		ParticleNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL, const Resource *normal = NULL);
		~ParticleNode();
		void Draw(Camera *camera);

		void Update(float deltaTime);

		/*Input:       d (duration of particle effect) 
		  Output:      void
		  Description: Set the particle effect duration
		*/
		void SetDuration(float d) { duration_ = d; max_duration_ = d; }

		/*Input:       void
		  Output:      float (get the duration of a particle)s
		  Description: Get the duration for a particl effect
		*/
		float GetDuration(void) const { return duration_; }

		/*Input:       t (set the start time for an object)
		  Output:      void
		  Description: set the start time of an object
		*/
		void SetStartTime(float t) { start_time_ = t; }

	private:
		float duration_;//duration in seconds for a particle
		float max_duration_; //max duration
		float start_time_;//start time of the particle effect

		void SetupShader(GLuint program, Camera* camera);
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
