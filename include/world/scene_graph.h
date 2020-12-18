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
#include "boss_node.h"
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
#include "satellite_node.h"
#include "particle_node.h"
#include "agent_node.h"
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
		BOUNTY_MENU,
		MAIN_MENU,
		DEATH_MENU,
		PAUSE_MENU,
		COMPLETE_MENU,
		SHOP_SHIP_MENU,
		SHOP_WEAPONS_1_MENU,
		SHOP_WEAPONS_2_MENU,
		SHOP_ABILITES_MENU,
		ENEMY_HEALTH,
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

			void AddSatellite(SatelliteNode *node);
			SatelliteNode *GetSatellite(std::string node_name) const;

			void AddBoss(Boss* boss) { boss_ = boss; }
			Boss* GetBoss(void) { return boss_; }
			void SetCurrentScreen(ScreenType t);

			bool EvasiveManeuversSuccessCheck(void);

			void SlowTime(double seconds);

			void SetBounty(std::string bountyType, int total, std::map<std::string, int> reward);
			void CheckBounty();

			// Setup the texture
			void SetupDrawToTexture(float frame_width, float frame_height);
            // Draw the entire scene
            void Draw(Camera *camera, int fps, bool to_texture = false,float frame_width=0, float frame_height=0);

			// Process and draw the texture on the screen
			void DisplayScreenSpace(GLuint program,std::string name, bool to_texture = false, float frame_width = 0, float frame_height = 0);
            // Update entire scene
            void Update(float deltaTime);

			std::vector<AsteroidNode*>* GetAsteroids()const { return asteroid_; };
			std::vector<CometNode*>* GetComets()const { return comet_; };
			std::vector<Enemy*>* GetEnemies()const { return enemy_; };
			std::vector<ParticleNode*>* GetDeathAnimations()const { return death_animations_; };

			std::string ButtonEvents(float x, float y);
			void UpdateScreenSizeNodes(float x, float y);

			void SetDeathAnimation(NodeResources dm);
			void SetAudio(Audio* a) { audio_ = a; }

			ScreenType GetCurrentMenu(void) { return active_menu_; }
			void SetCurrentMenu(ScreenType t) { active_menu_ = t; }

			void SetTextRenderer(TextRenderer* t) { text_renderer_ = t; }

			int GetCurrentBountyKills(void);
			float GetBountyProgress(void);

			void DisableAllEnemiesFor(double sec);

			std::string GetCurrentBounty(void) { return currentBounty; }

			void SetResetWorld(bool b) { reset_world_ = b; }
			bool GetResetWorld(void) { return reset_world_; }

			void SetLevelComplete(bool b) { level_complete_ = b; }
			bool GetLevelComplete(void) { return level_complete_; }

			void ClearData(void);

			bool GetSatelliteShop() { return satellite_shop_; }
		private:

			// Frame buffer for drawing to texture
			GLuint frame_buffer_;
			// Quad vertex array for drawing from texture
			GLuint quad_array_buffer_;
			// Render targets
			GLuint texture_;
			GLuint depth_buffer_;

			// Background color
			glm::vec3 background_color_;

			Audio* audio_;
			// Scene nodes to render
			Player* player_;
			SkyBox* skybox_;
			RadarNode* radar_;
			Boss* boss_;
			bool satellite_shop_;

			TextRenderer* text_renderer_;
			std::vector<SceneNode*>* node_;
			std::vector<Enemy *>* enemy_;
			std::vector<AsteroidNode*>* asteroid_;
			std::vector<CometNode*>* comet_;
			std::vector<SatelliteNode*>* satellite_;
			std::vector<ParticleNode*>* death_animations_;

			std::string currentBounty;
			int currentBountyTotal;
			std::map<std::string, int> bountyReward;
			int enemiesKilled;
			int asteroidsDestroyed;

			NodeResources death_animation_rsc;
			Text text;
			std::map <ScreenType, std::vector<ScreenNode *>> screen_;
			ScreenType active_menu_;
			std::map<ScreenType, std::vector<ButtonNode *>> button_;

			void CreateDeathAnimation(SceneNode* node);
			bool Collision(Entity* node, bool player);
			bool ProjectileCollision(AgentNode* node, bool player);
			double secondsSlow;

			void UpdateRadar();
			void UpdateRadarNode(glm::vec3 direction, glm::vec3 pos, glm::vec3 color, bool edge = false);
			void DrawAllText(Camera* camera, int fps);
			void DrawEnemyHealth(Camera* camera,glm::vec2 pos);
			glm::vec3 CalculateDistanceFromPlayer(glm::vec3 pos);

			float enemy_healthbar_distance_;
			float radar_distance_;

			bool reset_world_;
			bool level_complete_;

    }; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_
