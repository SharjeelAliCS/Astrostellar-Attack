#ifndef SCENE_GRAPH_H_
#define SCENE_GRAPH_H_

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene_node.h"
#include "enemy_node.h"
#include "player_node.h"
#include "projectile_node.h"
#include "hud_node.h"
#include "skybox_node.h"
#include "resource.h"
#include "camera.h"
namespace game {


	enum NodeType {
		PLAYER,
		ENEMY,
		HUD,
		NODE,
		SKYBOX
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
            SceneNode *CreateNode(std::string node_name, Resource *geometry, Resource *material,NodeType type, Resource *texture = NULL);
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

			void AddEnemy(Enemy *node);
			// Find a scene node with a specific name
			Enemy *GetEnemy(std::string node_name) const;

			void AddHUD(HUDNode *node);
			// Find a scene node with a specific name
			HUDNode *GetHUD(std::string node_name) const;
            // Get node const iterator

            // Draw the entire scene
            void Draw(Camera *camera);

            // Update entire scene
            void Update(float deltaTime);

		private:
			// Background color
			glm::vec3 background_color_;

			// Scene nodes to render
			Player* player_;
			SkyBox* skybox_;
			std::vector<SceneNode*> node_;
			std::vector<Enemy *> enemy_;
			std::vector<HUDNode *> hud_;


    }; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_