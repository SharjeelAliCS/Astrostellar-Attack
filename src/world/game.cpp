#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 40.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_g(0, -1.5, 10);
glm::vec3 camera_look_at_g(-0.0, -0, 0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;


Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
	timeOfLastMove = 0.0;
	window_width = window_width_g;
	window_height = window_height_g;
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){


    // Set up z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    // Create geometry of the objects
    //resman_.CreateTorus("SimpleTorusMesh",0.8,0.35,30,30);
	resman_.CreateWall("FlatSurface");
	resman_.CreateCylinder("SimpleCylinderMesh", 2.0, 0.4, 30, 30);
	resman_.CreateSphere("SimpleSphereMesh");
	resman_.CreateCube("SimpleCubeMesh",3,3,3);
    // Load shader for texture mapping
	std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/textured_material");
	resman_.LoadResource(Material, "TextureShader", filename.c_str());

	std::string assets_dir = std::string(MATERIAL_DIRECTORY) + "/"+std::string(ASSET_DIRECTORY);
	// shader for corona effect
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/procedural");
	resman_.LoadResource(Material, "Procedural", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/procedural");
	resman_.LoadResource(Material, "ObjectMaterial", filename.c_str());

    // Load texture to be used on the object
	filename = assets_dir + std::string("/Pumpkin_Color.png");
	resman_.LoadResource(Texture, "RockyTexture", filename.c_str());
	resman_.LoadResource(Mesh, "pumpkin", "meshes/Pumkin.obj");

	//player
	filename = assets_dir + std::string("/player.png");
	resman_.LoadResource(Texture, "shipTexture", filename.c_str());
	resman_.LoadResource(Mesh, "ship", "meshes/player.obj");

	//skybox
	filename = assets_dir + std::string("/skybox.png");
	resman_.LoadResource(Texture, "skyboxTexture", filename.c_str());
	resman_.LoadResource(Mesh, "skybox", "meshes/cube.obj");

}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);
	
	//create player object
	SceneNode* player =CreateInstance("player", "ship", "TextureShader", PLAYER,"shipTexture");
	player->SetScale(glm::vec3(0.05));
	player->SetOrientation(-90, glm::vec3(0, 1, 0));
    // Create an object for showing the texture
	// instance contains identifier, geometry, shader, and texture
	
	
	//skybox->SetOrientation(180, glm::vec3(1, 0, 0));
	//create enemies
	CreateEnemies(1000);
	//ame::SceneNode *wall = CreateInstance("Canvas", "FlatSurface", "Procedural", "RockyTexture"); // must supply a texture, even if not used
	//create skybox
	SceneNode* skybox = CreateInstance("skybox", "skybox", "TextureShader", SKYBOX, "skyboxTexture");
	skybox->SetScale(glm::vec3(0.25));
}


void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){

		static double last_time = glfwGetTime();
		static double last_update = glfwGetTime();

		double current_time = glfwGetTime();
		double deltaTime = current_time - last_time;
		last_time = current_time;
		// Animate the scene
		GetUserInput();
		if (animating_) {
			static double last_time = 0;
			double current_time = glfwGetTime();
			if ((current_time - last_update) > 0.05) {
				scene_.Update(deltaTime);

				// Animate the scene

				last_time = current_time;
			}

		}

        // Draw the scene
        scene_.Draw(&camera_);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}

void Game::GetUserInput(void) {

	//quit game
	if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, true);
	}

	// Stop animation if escape  is pressed
	if (timeOfLastMove + 0.5 < glfwGetTime()) {

		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			animating_ = !animating_;

			timeOfLastMove = glfwGetTime();
		}

	}
	if (animating_) {
		GetMouseCameraInput();

		Player* player = scene_.GetPlayer();
		//Get the factors used for movement
		float side_factor = 0.3;
		float foward_factor = 1;
		float const_foward = 0.2;
		//glm::vec3 pos = player->GetPosition();
		glm::vec3 foward = camera_.GetForward();

		//Translate player and camera by a given amount constantly. 
		//camera_.Translate(foward * const_foward);
		//player->Translate(foward * const_foward);

		//Fire a missile with a cooldown of 1 second. 
		if (timeOfLastMove + 1 < glfwGetTime()) {
			if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
				//player->Fire();
				timeOfLastMove = glfwGetTime();
			}
		}

		//move the player forward as well as the camera
		if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
			//glm::vec3 pos = player->GetPosition();
			glm::vec3 foward = camera_.GetForward();

			camera_.Translate(foward * foward_factor);
			player->Translate(foward * foward_factor);
		}

		//move the player back as well as the camera
		if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {

			glm::vec3 foward = camera_.GetForward();

			camera_.Translate(-foward * foward_factor);
			player->Translate(-foward * foward_factor);
		}

		//move the player left as well as the camera
		if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {

			glm::vec3 side = camera_.GetSide();

			camera_.Translate(-side * side_factor);
			player->Translate(-side * side_factor);
		}
		//move the player right as well as the camera
		if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {

			glm::vec3 side = camera_.GetSide();

			camera_.Translate(side * side_factor);
			player->Translate(side*side_factor);
		}
	}




}

void Game::GetMouseCameraInput() {
	double xpos, ypos;
	glfwGetCursorPos(window_, &xpos, &ypos);

	//For this, I decided the best way to do it was simply to keep the cursor set at the center
	//so that the user only moves the game WHEN their mouse is within the screen. 
	//The next issue I had was seeing how to make the range of the mouse sufficient. So I 
	//set the mouse to the center constantly, and measure it from there (difference)
	//which gives me the overall speed of the mouse that I offset the player with. 
	//After this, I "lag" behind the camera by 2x less than the current mouse speed.
	if (xpos >= 0 && xpos <= window_width && ypos >= 0 && ypos <= window_height && animating_) {

		Player* player = scene_.GetPlayer();

		float speed_x = xpos - window_width / 2;
		float speed_y = ypos - window_height / 2;
		speed_x /= 3000;
		speed_y /= 3000;
		glfwSetCursorPos(window_, window_width / 2, window_height / 2);

		//camera_.Yaw(-speed_x);
		//camera_.Pitch(-speed_y);
		
		glm::vec3 diff = player->GetPosition();

		diff -= camera_position_g.z * camera_.GetForward();
		diff -= camera_position_g.y * camera_.GetUp();
		camera_.SetPosition(diff);

		
		player->GetOrientationObj()->Yaw(-speed_x);
		player->GetOrientationObj()->Pitch(-speed_y);
	
		camera_.SetOrientation(player->RotLagBehind(speed_y / 2, speed_x / 2));
	}

}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    
}



void Game::ResizeCallback(GLFWwindow* window, int width, int height) {

	// Set up viewport and camera projection based on new window size
	glViewport(0, 0, width, height);
	void* ptr = glfwGetWindowUserPointer(window);
	Game *game = (Game *)ptr;
	game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
	game->window_width = width;
	game->window_height = height;
}

Game::~Game(){
    
    glfwTerminate();
}


Enemy *Game::CreateEnemyInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Enemy *en = new Enemy(entity_name, geom, mat);
    scene_.AddEnemy(en);
	en->SetScale(glm::vec3(3));
    return en;
}


void Game::CreateEnemies(int num_enemies){

    // Create a number of asteroid instances
    for (int i = 0; i < num_enemies; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;

        // Create asteroid instance
        Enemy *en = CreateEnemyInstance(name, "SimpleSphereMesh", "ObjectMaterial");

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum
		en->SetPosition(glm::vec3(-600.0 + 1200.0*((float) rand() / RAND_MAX), -600.0 + 1200.0*((float) rand() / RAND_MAX), 1200.0*((float) rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
		/*
		en->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
		*/
    }
}


SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, NodeType type, std::string texture_name){

    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = NULL;
    if (texture_name != ""){
        tex = resman_.GetResource(texture_name);
        if (!tex){
            throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
        }
    }
	
    SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, type, tex);
    return scn;
}

} // namespace game
