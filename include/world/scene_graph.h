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
#include "button_node.h"
#include "comet_node.h"
#include "sound.h"
#include "text.h"

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
            SceneNode *GetNode(std::string node_name) const;

			void AddSkyBox(SkyBox *node);
			SkyBox *GetSkyBox() const;

			void AddPlayer(Player *node);
			Player *GetPlayer() const;

			void AddRadar(RadarNode *node);

			void AddEnemy(Enemy *node);
			Enemy *GetEnemy(std::string node_name) const;

			void AddAsteroid(AsteroidNode *node);
			AsteroidNode *GetAsteroid(std::string node_name) const;

			void AddComet(CometNode *node);
			CometNode *GetComet(std::string node_name) const;

			void AddScreen(ScreenNode *node,ScreenType type);
			ScreenNode *GetScreen(std::string node_name) const;

			void AddButton(ButtonNode *node, ScreenType type);
			ButtonNode *GetButton(std::string node_name) const;

			void SetCurrentScreen(ScreenType t);
            // Draw the entire scene
            void Draw(Camera *camera);

            // Update entire scene
            void Update(float deltaTime);

			std::string ButtonEvents(float x, float y);
			void UpdateScreenSizeNodes(float x, float y);

		private:
			// Background color
			glm::vec3 background_color_;

			// Scene nodes to render
			Player* player_;
			SkyBox* skybox_;
			RadarNode* radar_;
			std::vector<SceneNode*> node_;
			std::vector<AsteroidNode*> asteroid_;
			std::vector<CometNode*> comet_;
			std::vector<Enemy *> enemy_;


			Text text;
			
			std::map <ScreenType, std::vector<ScreenNode *>> screen_;
			ScreenType active_menu_;
			std::map<ScreenType, std::vector<ButtonNode *>> button_;

			void UpdateRadar();
			void UpdateRadarNode(glm::vec3 direction, glm::vec3 pos,glm::vec3 color);
			glm::vec3 CalculateDistanceFromPlayer(glm::vec3 pos);

			float radar_distance_;

    }; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_
