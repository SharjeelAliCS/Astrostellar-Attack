/*
COMP 3501 A FALL 2020: Astrostellar Attack Final Project
OWNERS:                SHARJEEL ALI #101070889, DAVID NEUDORF #101029913
CONTENTS:              Contains the main game class

 */
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
#include "satellite_node.h"
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

			/*Input:       message (message to print)
			  Output:      void
			  Description: Prints the error exceptions
			*/
            GameException(std::string message) : message_(message) {};
            virtual const char* what() const throw() { return message_.c_str(); };

			/*Input:       void
			  Output:      void
			  Description: Throws the error exception
			*/
            virtual ~GameException() throw() {};
    };

    // Game application
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();
            
			/*Input:       void
			  Output:      void
			  Description: Inits the game class
			*/
            void Init(void); 
           
			/*Input:       void
			  Output:      void
			  Description: Loads and sets up resources and assets
			*/
            void SetupResources(void);
            
			/*Input:       void
			  Output:      void
			  Description: Set up the inital game scene
			*/
            void SetupScene(void);
            
			/*Input:       void
			  Output:      void
			  Description: Run the game loop
			*/
            void MainLoop(void); 

            ResourceManager resman_;// Resources available to the game

        private:

			// Methods to initialize the game

			/*Input:       void
			  Output:      void
			  Description: Restart the world (reload all objects)
			*/
			void ResetWorld(void);

			/*Input:       restart (bool to completely restart or not). 
			  Output:      void
			  Description: Setup the world
			*/
			void SetUpWorld(bool restart);

			/*Input:       void
			  Output:      void
			  Description: Initalize the window
			*/
			void InitWindow(void);

			/*Input:       void
			  Output:      void
			  Description: Initalize the view
			*/
			void InitView(void);

			/*Input:       void
			  Output:      void
			  Description: Initalize the event handlers
			*/
			void InitEventHandlers(void);

			// Methods to save/load the game

			/*Input:       restart (restart the save)
			  Output:      void
			  Description: Loads the last save file and variables
			*/
			void LoadSaveFile(bool restart = false);

			/*Input:       restart (restart the save)
			  Output:      void
			  Description: Loads the save attributes into player object
			*/
			void LoadLastSave(bool restart = false);

			/*Input:       void
			  Output:      void
			  Description: Saves the game into the JSON files
			*/
			void SaveGame(void);

			/*Input:       void
			  Output:      void
			  Description: Sets the save state 
			*/
			void SetSaveState(void);
			
			/*Input:       deltatime (time between frames)
			  Output:      void
			  Description: Gets the user input from keyboard/mouse
			*/
			void GetUserInput(float deltaTime);

			/*Input:       xpos (x position of mouse), ypos (y position of mouse)
			  Output:      void
			  Description: Gets the mouse position and rotates camera
			*/
			void GetMouseCameraInput(float xpos, float ypos);

			/*Input:       thing (item name), type (type of item)
			  Output:      void
			  Description: Buys an item or upgrade from the shop
			*/
			void BuySomething(std::string thing, std::string type);

			/*Input:       window (screen window), width (screen width), height (screen height)
			  Output:      void
			  Description: Resizes the screen 
			*/
			static void ResizeCallback(GLFWwindow* window, int width, int height);

			/*Input:       window (screen window), xoffset (offset in x direction), yoffset (scroll offset in y direction)
			  Output:      void
			  Description: Scrolls the player camera zoom using the mouse wheel
			  Source:      https://www.glfw.org/docs/3.3/input_guide.html#scrolling
			*/
			static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

			//Methods to create object scenery

			/*Input:       num_enemies (number of enemies to create)
			  Output:      void
			  Description: Creates the enemies
			*/
			void CreateEnemies(int num_enemies = 100);

			/*Input:       void
			  Output:      void
			  Description: Creates the boss
			*/
			void CreateBoss(void);

			/*Input:       num_asteroids (number of asteroids to create)
			  Output:      void
			  Description: Creates the asteroids
			*/
			void CreateAsteroids(int num_asteroids = 1500);

			/*Input:       num_satellites (number of satellites to create)
			  Output:      void
			  Description: Creates the satellites
			*/
			void CreateSatellites(int num_satellites = 10);

			/*Input:       num_comets (number of comets to create)
			  Output:      void
			  Description: Creates the comets
			*/
			void CreateComets(int num_comets = 10);

			/*Input:       type (type of enemy), en (enemy object), data (json data)
			  Output:      void
			  Description: Sets the enemy stats for a particular enemy object
			*/
			void SetEnemyStats(std::string type, Enemy* en, json data);


			//methods to create the menu buttons and visual feedbacks

			/*Input:       void
			  Output:      void
			  Description: Create the screen menus 
			*/
			void CreateScreenMenus(void);

			/*Input:       void
			  Output:      void
			  Description: Create the HUD 
			*/
			void CreateHUD(void);

			/*Input:       void
			  Output:      void
			  Description: Create the main menu
			*/
			void CreateMainMenu(void);

			/*Input:       void
			  Output:      void
			  Description: Create the bounty selection menu
			*/
			void CreateBountyMenu(void);

			/*Input:       void
			  Output:      void
			  Description: Create the pause menu
			*/
			void CreatePauseMenu(void);

			/*Input:       void
			  Output:      void
			  Description: Create the shop ship menu
			*/
			void CreateShopShipMenu(void);

			/*Input:       void
			  Output:      void
			  Description: Create the shop weapons menu
			*/
			void CreateShopWeaponsMenu(void);

			/*Input:       void
			  Output:      void
			  Description: Create the shop abilites menus
			*/
			void CreateShopAbilitesMenu(void);

			/*Input:       void
			  Output:      void
			  Description: Create the level completion  scren
			*/
			void CreateLevelCompleteMenu(void);

			/*Input:       void
			  Output:      void
			  Description: Create the death screen
			*/
			void CreateDeathMenu(void);

			/*Input:       type (type of screen to assign headers to)
			  Output:      void
			  Description: Create the shop button headers
			*/
			void CreateShopHeaders(ScreenType type);

			/*Input:       obj_name (object name), text_name (name of item to show), type (type of screen to assign to), x (x grid position), y (y grid position), upgrade_type (type of item)
			  Output:      void
			  Description: Create the shop upgrade button
			*/
			void CreateShopButton(std::string obj_name, std::string text_name, ScreenType type, int x, int y, std::string upgrade_type);

			/*Input:       obj_name (object name),upgrade_type (type of item)
			  Output:      void
			  Description: Updates the shop button's costs
			*/
			void UpdateShopText(std::string obj_name, std::string upgrade_type);
			
			//methods to create single node instance

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)s
			  Output:      asteroid node
			  Description: Creates a single asteroid instance
			*/
			AsteroidNode *CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)s
			  Output:      satellite node
			  Description: Creates a single satellite instance
			*/
			SatelliteNode* CreateSatelliteInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)s
			  Output:      comet node
			  Description: Creates a single comet instance
			*/
			CometNode *CreateCometNode(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			/*Input:       count (number of particles), entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)s
			  Output:      particle node
			  Description: Creates a single particle instance
			*/
			ParticleNode* CreateParticleInstance(int count, std::string particle_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)s
			  Output:      boss node
			  Description: Creates a single boss instance
			*/
			Boss* CreateBossInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name = std::string(""));

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)
			  Output:      enemy node
			  Description: Creates a single enemy instance
			*/
			Enemy *CreateEnemyInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name = std::string(""));

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)s
			  Output:      screen node
			  Description: Creates a single screen instance
			*/
			ScreenNode *CreateScreenInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)s
			  Output:      button node
			  Description: Creates a single button instance
			*/
			ButtonNode *CreateButtonInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			/*Input:       entity_name (name of node), object_name (name of mesh), material_name (name of shader), type (type of node), texture_name (name of texture), normal_name (name of normal)s
			  Output:      scene node
			  Description: Creates a single node instance
			*/
			SceneNode *CreateInstance(std::string entity_name, std::string object_name, std::string material_name, NodeType type, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			/*Input:       object_name (name of mesh), material_name (name of shader), texture_name (name of texture), normal_name (name of normal)
			  Output:      NodeResources (struct of resources)
			  Description: Collects node resources
			*/
			NodeResources* GetResources(std::string object_name, std::string material_name, std::string texture_name, std::string normal_name);

			ScreenType prev_menu_;//previous menu to the current one

            GLFWwindow* window_;// GLFW window

            SceneGraph scene_; // Scene graph containing all nodes to render
			Audio* audio_; //audio class
			TextRenderer text; //text rendering class
            Camera camera_;// Camera abstraction

            
            bool animating_;// Flag to turn animation on/off

           
			bool first_person_; //flag for first person fview 
			float aspect_ratio_; //aspect ratio of screen
			int window_width; //screen width
			int window_height; //screen height
			float timeOfLastMove; //time of the last move
			bool tabNotPressedLastFrame; //change guns
			bool spaceNotPressedLastFrame; //evasive maneuvers
			bool oneNotPressedLastFrame; //chrono surge
			bool twoNotPressedLastFrame; // emergency warp
			bool threeNotPressedLastFrame; //reactor overload TODO
			bool fourNotPressedLastFrame; //whatever we have time for TODO

            double evasiveManeuversCooldown; //cooldown for ability

			//Maps to store player inventory and stats
			std::map<std::string, int> loadedPlayerUpgrades;
            std::map<std::string, int> loadedPlayerInventory;
            std::map<std::string, int> loadedPlayerLoadout;
            std::map<std::string, int> loadedPlayerStats;
            std::map<std::string, float> loadedWeaponStats;
            std::map<std::string, std::map<std::string, int>> loadedBountyStats;
            std::map<std::string, std::map<std::string, int>> loadedUpgradeCosts;

            int startTime;//start time of game
            int startKills; //kills at start
            int mouse_speed=1250; //speed of mouse
            int numEnemies = 60; //number of enemies to load
            int numAsteroids = 500; //number of asteroids to load

			std::string chosen_bounty_;//current selected bounty from menu

    }; // class Game

} // namespace game

#endif // GAME_H_
