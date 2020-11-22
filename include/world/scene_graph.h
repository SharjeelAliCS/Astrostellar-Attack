#ifndef SCENE_GRAPH_H_
#define SCENE_GRAPH_H_

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#include "scene_node.h"
#include "enemy_node.h"
#include "player_node.h"
#include "projectile_node.h"
#include "screen_node.h"
#include "skybox_node.h"
#include "resource.h"
#include "camera.h"
#include "radar_node.h"
#include "asteroid_node.h"

#include "sound.h"
namespace game {


	enum NodeType {
		PLAYER,
		ENEMY,
		HUD,
		NODE,
		SKYBOX,
		ASTEROID,
		PARTICLE
	};

	enum ScreenType {
		HUD_MENU,
		MAIN_MENU,
		PAUSE_MENU,
		CREDITS_MENU,
		SHOP_MENU,
		NONE
	};

    // Class that manages all the objects in a scene
    class SceneGraph {

        public:
            // Constructor and destructor
            SceneGraph(void);
            ~SceneGraph();

            // Background color
            void SetBackgroundColor(glm::vec3 color);
            glm::vec3 GetBackgroundColor(void) const;
            
            // Create a scene node from the specified resources
            SceneNode *CreateNode(std::string node_name, Resource *geometry, Resource *material,NodeType type, Resource *texture = NULL, Resource *normal = NULL);
            // Add an already-created node

            void AddNode(SceneNode *node);
            // Find a scene node with a specific name
            SceneNode *GetNode(std::string node_name) const;

			void AddSkyBox(SkyBox *node);
			// Find a scene node with a specific name
			SkyBox *GetSkyBox() const;

			void AddPlayer(Player *node);
			// Find a scene node with a specific name
			Player *GetPlayer() const;

			void AddRadar(RadarNode *node);
			// Find a scene node with a specific name

			void AddEnemy(Enemy *node);
			// Find a scene node with a specific name
			Enemy *GetEnemy(std::string node_name) const;

			void AddScreen(ScreenNode *node,ScreenType type);
			// Find a scene node with a specific name
			ScreenNode *GetScreen(std::string node_name) const;
            // Get node const iterator

			void SetCurrentScreen(ScreenType t);
            // Draw the entire scene
            void Draw(Camera *camera);

            // Update entire scene
            void Update(float deltaTime);

			
			std::vector<SceneNode*>* GetAsteroids()const { return node_; };

		private:
			// Background color
			glm::vec3 background_color_;

			// Scene nodes to render
			Player* player_;
			SkyBox* skybox_;
			RadarNode* radar_;
			std::vector<SceneNode*>* node_;
			std::vector<Enemy *>* enemy_;
			
			std::map <ScreenType, std::vector<ScreenNode *>> screen_;
			ScreenType active_menu_;

			void UpdateRadar();
			void UpdateRadarNode(glm::vec3 direction, glm::vec3 pos,glm::vec3 color);
			glm::vec3 CalculateDistanceFromPlayer(glm::vec3 pos);

			float radar_distance_;

    }; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_
