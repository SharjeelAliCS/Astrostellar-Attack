#ifndef GAME_H_
#define GAME_H_

#include <exception>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene_graph.h"
#include "resource_manager.h"
#include "camera.h"
#include "enemy_node.h"
#include "player_node.h"
#include "projectile_node.h"
#include "screen_node.h"
#include "skybox_node.h"
#include "boss_node.h"
#include "radar_node.h"
#include "asteroid_node.h"
#include "particle_node.h"
#include "button_node.h"
#include "text.h"
#include "sound.h"
namespace game {

    // Exception type for the game
    class GameException: public std::exception
    {
        private:
            std::string message_;
        public:
            GameException(std::string message) : message_(message) {};
            virtual const char* what() const throw() { return message_.c_str(); };
            virtual ~GameException() throw() {};
    };

    // Game application
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();
            // Call Init() before calling any other method
            void Init(void); 
            // Set up resources for the game
            void SetupResources(void);
            // Set up initial scene
            void SetupScene(void);
            // Run the game: keep the application active
            void MainLoop(void); 

            // Resources available to the game
            ResourceManager resman_;
        private:


            // GLFW window
            GLFWwindow* window_;

            // Scene graph containing all nodes to render
            SceneGraph scene_;

			Audio* audio_;
           
			TextRenderer text;

            // Camera abstraction
            Camera camera_;

            // Flag to turn animation on/off
            bool animating_;

            // Methods to initialize the game
            void InitWindow(void);
            void InitView(void);
            void InitEventHandlers(void);

            void LoadSaveFile(void);
            void LoadLastSave(void);
            void SaveGame(void);
 
			bool first_person_;
			float aspect_ratio_;
			int window_width;
			int window_height;
			float timeOfLastMove;
			bool tabNotPressedLastFrame; //change guns
			bool spaceNotPressedLastFrame; //evasive maneuvers
			bool oneNotPressedLastFrame; //chrono surge
			bool twoNotPressedLastFrame; // emergency warp
			bool threeNotPressedLastFrame; //reactor overload TODO
			bool fourNotPressedLastFrame; //whatever we have time for TODO

            double evasiveManeuversCooldown;

			std::map<std::string, int> loadedPlayerUpgrades;
            std::map<std::string, int> loadedPlayerInventory;
            std::map<std::string, int> loadedPlayerLoadout;
            std::map<std::string, int> loadedPlayerStats;

            std::map<std::string, float> loadedWeaponStats;
            std::map<std::string, std::map<std::string, int>> loadedBountyStats;
            std::map<std::string, std::map<std::string, int>> loadedUpgradeCosts;

            int startTime;
            int startKills;
            int mouse_speed=1250;
            int numEnemies = 60;
            int numAsteroids = 500;

			std::string chosen_bounty_;

			void SetSaveState(void);
			//New user input function more multiple inputs
			void GetUserInput(float deltaTime);

			//Camera input for mouse (See .cpp file for more info)
			void GetMouseCameraInput(float xpos, float ypos);

            void BuySomething(std::string thing, std::string type);

            // Methods to handle events
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void ResizeCallback(GLFWwindow* window, int width, int height);

			//code taken from here: https://www.glfw.org/docs/3.3/input_guide.html#scrolling
			static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
            // Asteroid field
            // Create instance of one asteroid

            // Create entire random asteroid field
			void ResetWorld(void);
			void SetUpWorld(bool restart);

			void CreateEnemies(int num_enemies = 100);
			void CreateBoss(void);
			void CreateAsteroids(int num_asteroids = 1500);
			void CreateComets(int num_comets = 10);
			// Create entire random asteroid field

			void CreateScreenMenus(void);
			void CreateHUD(void);
			void CreateMainMenu(void);
			void CreateBountyMenu(void);
			void CreatePauseMenu(void);
			void CreateShopShipMenu(void);
			void CreateShopWeaponsMenu(void);
			void CreateShopAbilitesMenu(void);
			
			void CreateShopHeaders(ScreenType type);
			void CreateShopButton(std::string obj_name,std::string text_name,ScreenType type, int x, int y,std::string upgrade_type);
			void UpdateShopText(std::string obj_name, std::string upgrade_type);
			void CreateDeathMenu(void);

			ScreenType prev_menu_;

			void SetEnemyStats(std::string type, Enemy* en, json data);

			AsteroidNode *CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			CometNode *CreateCometNode(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			ParticleNode* CreateParticleInstance(int count, std::string particle_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

			Boss* CreateBossInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name = std::string(""));

			Enemy *CreateEnemyInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name = std::string(""));

			ScreenNode *CreateScreenInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			ButtonNode *CreateButtonInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

            SceneNode *CreateInstance(std::string entity_name, std::string object_name, std::string material_name, NodeType type,std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			NodeResources* GetResources(std::string object_name,std::string material_name, std::string texture_name, std::string normal_name);
    }; // class Game

} // namespace game

#endif // GAME_H_
