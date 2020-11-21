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
           
			Text text;

            // Camera abstraction
            Camera camera_;

            // Flag to turn animation on/off
            bool animating_;

            // Methods to initialize the game
            void InitWindow(void);
            void InitView(void);
            void InitEventHandlers(void);
 
			bool first_person_;
			float aspect_ratio_;
			int window_width;
			int window_height;
			float timeOfLastMove;

			void SetSaveState(void);
			//New user input function more multiple inputs
			void GetUserInput(float deltaTime);

			//Camera input for mouse (See .cpp file for more info)
			void GetMouseCameraInput(float xpos, float ypos);

            // Methods to handle events
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void ResizeCallback(GLFWwindow* window, int width, int height);

			//code taken from here: https://www.glfw.org/docs/3.3/input_guide.html#scrolling
			static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
            // Asteroid field
            // Create instance of one asteroid

			ParticleNode* CreateParticleInstance(int count, std::string particle_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

            Enemy *CreateEnemyInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name = std::string(""));
            // Create entire random asteroid field

			void CreateAsteroids(int num_enemies = 1500);
			// Create entire random asteroid field
			void CreateHUD(void);


			// Create an instance of an object stored in the resource manager
			ScreenNode *CreateScreenInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

			// Create an instance of an object stored in the resource manager
			ButtonNode *CreateButtonInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name = std::string(""), std::string normal_name = std::string(""));

            // Create an instance of an object stored in the resource manager
            SceneNode *CreateInstance(std::string entity_name, std::string object_name, std::string material_name, NodeType type,std::string texture_name = std::string(""), std::string normal_name = std::string(""));


    }; // class Game

} // namespace game

#endif // GAME_H_
