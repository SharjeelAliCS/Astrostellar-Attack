/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains a scene graph class

 */
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

	//node type
	enum NodeType {
		PLAYER,
		ENEMY,
		HUD,
		NODE,
		SKYBOX,
		ASTEROID,
		PARTICLE
	};

	//screen type
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

			/*Input:       color (vec3 color)
			  Output:      void
			  Description: Set the background color
			*/
            void SetBackgroundColor(glm::vec3 color);

			/*Input:       void
			  Output:      vec3 (color)
			  Description: Get the background color
			*/
            glm::vec3 GetBackgroundColor(void) const;
            
			/*Input:       node_name (name of node), geometry (mesh), material (shader program), type (type of node), texture (texture for node), normal (normal map for node)
			  Output:      SceneNode
			  Description: Creates a node
			*/
            SceneNode *CreateNode(std::string node_name, Resource *geometry, Resource *material,NodeType type, Resource *texture = NULL, Resource *normal = NULL);
            
			/*Input:       node (scene node)
			  Output:      void
			  Description: Add a scene node
			*/
            void AddNode(SceneNode *node);

			/*Input:       node_name (name of node)
			  Output:      SceneNode (scene node)
			  Description: Get a scene node  by name
			*/
            SceneNode *GetNode(std::string node_name) const;

			/*Input:       node (skybox node)
			  Output:      void
			  Description: set a skybox node
			*/
			void AddSkyBox(SkyBox *node);

			/*Input:       void
			  Output:      SkyBox (skybox node)
			  Description: Get the skybox
			*/
			SkyBox *GetSkyBox() const;

			/*Input:       node (player node)
			  Output:      void
			  Description: Sets the player node
			*/
			void AddPlayer(Player *node);

			/*Input:       void
			  Output:      player (player node)
			  Description: Get the player node
			*/
			Player *GetPlayer() const;

			/*Input:       node (radar node)
			  Output:      void
			  Description: Add a radar node
			*/
			void AddRadar(RadarNode *node);

			/*Input:       node (enemy node)
			  Output:      void
			  Description: Add a enemy node
			*/
			void AddEnemy(Enemy *node);

			/*Input:       node_name (name of node)
			  Output:      Enemy (enemy node)
			  Description: Get a enemy node  by name
			*/
			Enemy *GetEnemy(std::string node_name) const;

			/*Input:       node (asteroid node)
			  Output:      void
			  Description: Add a asteroid node
			*/
			void AddAsteroid(AsteroidNode *node);

			/*Input:       node_name (name of node)
			  Output:      AsteroidNode (asteroid node)
			  Description: Get a asteroid node  by name
			*/
			AsteroidNode *GetAsteroid(std::string node_name) const;

			/*Input:       node (comet node)
			  Output:      void
			  Description: Add a comet node
			*/
			void AddComet(CometNode *node);

			/*Input:       node_name (name of node)
			  Output:      CometNode (comet node)
			  Description: Get a comet node  by name
			*/
			CometNode *GetComet(std::string node_name) const;

			/*Input:       node (screen node)
			  Output:      void
			  Description: Add a screen node
			*/
			void AddScreen(ScreenNode *node,ScreenType type);

			/*Input:       node_name (name of node)
			  Output:      ScreenNode (screen node)
			  Description: Get a screen node  by name
			*/
			ScreenNode *GetScreen(std::string node_name) const;

			/*Input:       node (button node)
			  Output:      void
			  Description: Add a button node
			*/
			void AddButton(ButtonNode *node, ScreenType type);

			/*Input:       node_name (name of node)
			  Output:      ButtonNode (button node)
			  Description: Get a button node  by name
			*/
			ButtonNode *GetButton(std::string node_name) const;

			/*Input:       node (satellite node)
			  Output:      void
			  Description: Add a satellite node
			*/
			void AddSatellite(SatelliteNode *node);

			/*Input:       node_name (name of node)
			  Output:      SatelliteNode (satellite node)
			  Description: Get a satellite node  by name
			*/
			SatelliteNode *GetSatellite(std::string node_name) const;

			/*Input:       node (boss node)
			  Output:      void
			  Description: set a boss node
			*/
			void AddBoss(Boss* boss) { boss_ = boss; }

			/*Input:       void
			  Output:      Boss (boss node)
			  Description: get the boss node
			*/
			Boss* GetBoss(void) { return boss_; }

			/*Input:       t (type of screen)
			  Output:      void
			  Description: Set the current screen for menus/hud
			*/
			void SetCurrentScreen(ScreenType t);

			/*Input:       void
			  Output:      bool (flag for if EV manuveur succeeded)
			  Description: Check if EV menuever was a success
			*/
			bool EvasiveManeuversSuccessCheck(void);

			/*Input:       seconds (time)
			  Output:      void
			  Description: Slowdown time 
			*/
			void SlowTime(double seconds);

			/*Input:       bountyType (string bounty name), total (total number of kills required), reward (map of rewards)
			  Output:      void
			  Description: Sets the current bounty
			*/
			void SetBounty(std::string bountyType, int total, std::map<std::string, int> reward);

			/*Input:       void
			  Output:      void
			  Description: check if current bounty has been completed each frame
			*/
			void CheckBounty();

			/*Input:       frame_width (frame width), frame_height (frame height)
			  Output:      void
			  Description: Setup the draw to texture for screen space effect
			*/
			void SetupDrawToTexture(float frame_width, float frame_height);
            
			/*Input:       camera (camera), fps (frames per second), to_texture (draw to texture or not),frame_width (frame width), frame_height (frame height)
			  Output:      void
			  Description: Draw the entire scene
			*/
            void Draw(Camera *camera, int fps, bool to_texture = false,float frame_width=0, float frame_height=0);

			/*Input:       program (shader program), name (name of screen space effect), to_texture (save screen space into texture), frame_width (frame width), frame_height (frame height)
			  Output:      void
			  Description: Draw the entire screen space effect
			*/
			void DisplayScreenSpace(GLuint program,std::string name, bool to_texture = false, float frame_width = 0, float frame_height = 0);
            
			/*Input:       deltatime (time between frames)
			  Output:      void
			  Description: Update the scene
			*/
            void Update(float deltaTime);

			/*Input:       void
			  Output:      vector (asteroids)
			  Description: Get the asteroid vector
			*/
			std::vector<AsteroidNode*>* GetAsteroids()const { return asteroid_; };

			/*Input:       void
			  Output:      vector (CometNode)
			  Description: Get the comets vector
			*/
			std::vector<CometNode*>* GetComets()const { return comet_; };

			/*Input:       void
			  Output:      vector (Enemy)
			  Description: Get the Enemy vector
			*/
			std::vector<Enemy*>* GetEnemies()const { return enemy_; };

			/*Input:       void
			  Output:      vector (ParticleNode)
			  Description: Get the GetDeathAnimations vector
			*/
			std::vector<ParticleNode*>* GetDeathAnimations()const { return death_animations_; };

			/*Input:       x (x mouse pos), y (y mouse pos)
			  Output:      string (name of button clicked)
			  Description: Get the button name that was clicked
			*/
			std::string ButtonEvents(float x, float y);

			/*Input:       x (screen width), y (screen height)
			  Output:      void
			  Description: Update the screen space dimensions
			*/
			void UpdateScreenSizeNodes(float x, float y);

			/*Input:       dem (resources for the particle explosion)
			  Output:      void
			  Description: Set the particle explosion resources
			*/
			void SetDeathAnimation(NodeResources dm);

			/*Input:       a (audio object)
			  Output:      void
			  Description: Set the audio object
			*/
			void SetAudio(Audio* a) { audio_ = a; }

			/*Input:       void
			  Output:      ScreenType (Current menu)
			  Description: Get the current menu
			*/
			ScreenType GetCurrentMenu(void) { return active_menu_; }

			/*Input:       t (menu type)
			  Output:      void
			  Description: Set the current menu
			*/
			void SetCurrentMenu(ScreenType t) { active_menu_ = t; }

			/*Input:       t (text renderer object)
			  Output:      void
			  Description: Set the text renderer
			*/
			void SetTextRenderer(TextRenderer* t) { text_renderer_ = t; }

			/*Input:       void
			  Output:      int (current bounty kills)
			  Description: Get the current bounty kills
			*/
			int GetCurrentBountyKills(void);

			/*Input:       void
			  Output:      float (bounty percent progress)
			  Description: Get the bounty progress as a percent
			*/
			float GetBountyProgress(void);

			/*Input:       sec (time to disable for)
			  Output:      void
			  Description: Disables all enemeis but the boss for a period of sec
			*/
			void DisableAllEnemiesFor(double sec);

			/*Input:       void
			  Output:      string (bounty name)
			  Description: Get the current bounty
			*/
			std::string GetCurrentBounty(void) { return currentBounty; }

			/*Input:       b (flag to reset world)
			  Output:      void
			  Description: Set the reset world flag
			*/
			void SetResetWorld(bool b) { reset_world_ = b; }

			/*Input:       void
			  Output:      bool (reset world flag)
			  Description: Get the reset world flag
			*/
			bool GetResetWorld(void) { return reset_world_; }

			/*Input:       b (flag to complete level)
			  Output:      void
			  Description: Set the complete level flag
			*/
			void SetLevelComplete(bool b) { level_complete_ = b; }

			/*Input:       void
			  Output:      bool (complete level flag)
			  Description: Get the complete level flag
			*/
			bool GetLevelComplete(void) { return level_complete_; }

			/*Input:       void
			  Output:      void
			  Description: Clear all object node vectors
			*/
			void ClearData(void);

			/*Input:       void
			  Output:      bool (flag for satellite shop)
			  Description: Check if the satellite's shop is openable
			*/
			bool GetSatelliteShop() { return satellite_shop_; }
		private:

			/*Input:       node (node to create particle explosion on)
			  Output:      void
			  Description: Create the particle explosion using the node
			*/
			void CreateDeathAnimation(SceneNode* node);

			/*Input:       node (node to check), player (flag indicating if player is being inputted or enemy)
			  Output:      bool (flag for collision)
			  Description: check for a collision between node and all game objects
			*/
			bool Collision(Entity* node, bool player);

			/*Input:       node (node to check), player (flag indicating if player is being inputted or enemy)
			  Output:      bool (flag for collision)
			  Description: check for a collision between node's projectiles and all game objects
			*/
			bool ProjectileCollision(AgentNode* node, bool player);
			
			/*Input:       void
			  Output:      void
			  Description: Update the radar
			*/
			void UpdateRadar();

			/*Input:      direction (given direction to node), pos (position of object), color (color on radar), edge (flag indicating if dot should always show on radar at edges)
			  Output:      void
			  Description: update the radar per node object's position
			*/
			void UpdateRadarNode(glm::vec3 direction, glm::vec3 pos, glm::vec3 color, bool edge = false);

			/*Input:       camera (camera), fps (frames per second)
			  Output:      void
			  Description: Draw all game text
			*/
			void DrawAllText(Camera* camera, int fps);

			/*Input:       camera (camera), pos (position on screen)
			  Output:      void
			  Description: Draw enemy health
			*/
			void DrawEnemyHealth(Camera* camera, glm::vec2 pos);

			/*Input:       pos (position of object)
			  Output:      vec3 (distancd)
			  Description: Calculate distance of object to player in 2d space
			  Source:      https://stackoverflow.com/questions/23472048/projecting-3d-points-to-2d-plane
			*/
			glm::vec3 CalculateDistanceFromPlayer(glm::vec3 pos);

			double secondsSlow;


			GLuint frame_buffer_;// Frame buffer for drawing to texture
			
			GLuint quad_array_buffer_;// Quad vertex array for drawing from texture
			
			GLuint texture_;// Render targets
			GLuint depth_buffer_; //depth buffer

			glm::vec3 background_color_;// Background color

			Audio* audio_;//audio

			// Scene nodes to render

			Player* player_;
			SkyBox* skybox_;
			RadarNode* radar_;
			Boss* boss_;
			bool satellite_shop_;

			TextRenderer* text_renderer_;

			//vectors to render
			std::vector<SceneNode*>* node_;
			std::vector<Enemy *>* enemy_;
			std::vector<AsteroidNode*>* asteroid_;
			std::vector<CometNode*>* comet_;
			std::vector<SatelliteNode*>* satellite_;
			std::vector<ParticleNode*>* death_animations_;

			std::map <ScreenType, std::vector<ScreenNode *>> screen_;
			ScreenType active_menu_;
			std::map<ScreenType, std::vector<ButtonNode *>> button_;

			std::string currentBounty;//current selected bounty
			int currentBountyTotal;//current total bounty number completed
			std::map<std::string, int> bountyReward;
			int enemiesKilled;
			int asteroidsDestroyed;

			NodeResources death_animation_rsc; //resources for the particle explosion
			Text text;

			float enemy_healthbar_distance_;//distance of enemy to show health
			float radar_distance_;  //max radar distance

			bool reset_world_; //flag to reset world
			bool level_complete_; //flag to complete level

    }; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_
