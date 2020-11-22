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
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 5000.0;
float camera_fov_g = 40.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_1st_g(0, 0, 1);
glm::vec3 camera_position_3rd_g(0, -1.5, 10);
glm::vec3 camera_look_at_g(-0.0, -0, 0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;


Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

	aspect_ratio_ = 1080.0 / 1920.0;
	window_width = 800;
	window_height = window_width * aspect_ratio_;
	
    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
	timeOfLastMove = 0.0;
	first_person_ = true;

}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width, window_height, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width, window_height, window_title_g.c_str(), NULL, NULL);
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

	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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

    camera_.SetView(camera_position_1st_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){
	float aspect_ratio = 1080.0 / 1920.0;

	glfwSetWindowAspectRatio(window_, 1920,1080);
    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);
	glfwSetScrollCallback(window_, scroll_callback);
    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){
	audio_ = new Audio();
    // Create geometry of the objects
    //resman_.CreateTorus("SimpleTorusMesh",0.8,0.35,30,30);
	resman_.CreateWall("FlatSurface");
	resman_.CreateCylinder("SimpleCylinderMesh", 2.0, 0.4, 30, 30);
	resman_.CreateSphere("SimpleSphereMesh");
	resman_.CreateTorus("SimpleTorusMesh");
	resman_.CreateCube("c",3,3,3);

	std::string filename;
	//load save data
	filename = SAVE_DIRECTORY + std::string("/game_state.json");
	resman_.LoadResource(Save, "save", filename.c_str());

	//load asset data
	filename = SAVE_DIRECTORY + std::string("/asset_data.json");
	resman_.LoadResource(Save, "assetList", filename.c_str());

	json assets = resman_.GetResource("assetList")->GetJSON();
	
	//load shaders
	for (auto& asset : assets["Shader"].items()) {
		filename = std::string(MATERIAL_DIRECTORY) + std::string(asset.value());
		resman_.LoadResource(Material, asset.key(), filename.c_str());
	}
	
	//load meshes
	for (auto& asset : assets["Mesh"].items()) {
		filename = std::string(ASSET_DIRECTORY) + "/meshes"+std::string(asset.value());
		resman_.LoadResource(Mesh, asset.key(), filename.c_str());
	}

	//load normal maps
	for (auto& asset : assets["Normal"].items()) {
		filename = std::string(ASSET_DIRECTORY) + "/graphics/normal/" + std::string(asset.value());
		resman_.LoadResource(Texture, asset.key(), filename.c_str());
	}

	//load diffuse textures
	for (auto& asset : assets["Texture"].items()) {
		filename = std::string(ASSET_DIRECTORY) + "/graphics/texture/" + std::string(asset.value());
		resman_.LoadResource(Texture, asset.key(), filename.c_str());
	}

	//load fonts
	for (auto& asset : assets["Font"].items()) {
		filename = std::string(ASSET_DIRECTORY) + "/fonts" + std::string(asset.value());
		text = Text(filename, resman_.GetResource("textMaterial"));
	}

	//load soundtrack
	for (auto& asset : assets["Soundtrack"].items()) {
		filename = std::string(ASSET_DIRECTORY) + "/sounds/soundtrack/" + std::string(asset.value());
		audio_->addAudio(filename,asset.key());
	}

	//load sound effects
	for (auto& asset : assets["SoundEffects"].items()) {
		filename = std::string(ASSET_DIRECTORY) + "/sounds/effects/" + std::string(asset.value());
		audio_->addAudio(filename, asset.key());
	}

	audio_->volume("ambience", 100);
	audio_->volume("playerEngine", 300);
	audio_->playRepeat("ambience");
	
}

void Game::SetupScene(void){
    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);
	
	//create player object
	SceneNode* player =CreateInstance("player", "ship", "TextureShader", PLAYER,"shipTexture");
	player->SetScale(glm::vec3(2));
	player->SetOrientation(-90, glm::vec3(0, 1, 0));
    // Create an object for showing the texture
	// instance contains identifier, geometry, shader, and texture
	
	//skybox->SetOrientation(180, glm::vec3(1, 0, 0));
	//create enemies
	CreateAsteroids(500);

	//ame::SceneNode *wall = CreateInstance("Canvas", "FlatSurface", "Procedural", "RockyTexture"); // must supply a texture, even if not used
	//create skybox
	SceneNode* skybox = CreateInstance("skybox", "skybox", "TextureShader", SKYBOX, "skyboxTexture");
	skybox->SetScale(glm::vec3(0.25));

	CreateHUD();

	//create particles:
	ParticleNode* pn = CreateParticleInstance(20000, "jetstream", "playerParticles", "ParticleMaterial");
	pn->SetOrientation(90, glm::vec3(0, 1, 0));
	pn->Rotate(-90, glm::vec3(1, 0, 0));
	pn->Translate(glm::vec3(1, 0.5, -3)); 
	pn->SetJoint(glm::vec3(0,0.2,-1));
	pn->SetScale(glm::vec3(8));
	pn->SetDraw(false);
	//pn->SetJoint(glm::vec3(0, 0, -1));
	//scene_.GetPlayer()->SetParticleSystem(pn);
	scene_.GetPlayer()->AddChild(pn);
	scene_.GetScreen("cockpit")->SetDraw(true);
	scene_.GetPlayer()->SetDraw(false);
	camera_.SetZoom(0);
	SetSaveState();
}

void Game::SetSaveState(void) {

	//access save data
	Resource* dataResource = resman_.GetResource("save");
	json data = dataResource->GetJSON();

	//modify save data
	data["fruit"] = "ace";

	//output save data
	dataResource->SetJSON(data);
	resman_.SaveResource("save");

	std::cout << data << std::endl;
	//std::string fruit = (*data)["fruit"];
}
void Game::MainLoop(void){

    // Loop while the user did not close the window
	float t = 0;
	int frames = 0;
	float second = glfwGetTime();
	int fps = 0.0;
    while (!glfwWindowShouldClose(window_)){

		static double last_time = glfwGetTime();
		static double last_update = glfwGetTime();

		double current_time = glfwGetTime();
		double deltaTime = current_time - last_time;
		last_time = current_time;
		// Animate the scene
		Player* player = scene_.GetPlayer();
		player->setCam(&camera_);
		player->setAsteroids(scene_.GetAsteroids());
		player->setEnemies(scene_.GetEnemies());

		GetUserInput(deltaTime);
		if (animating_) {
			static double last_time = 0;
			double current_time = glfwGetTime();
			if ((current_time - last_update) > 0.05) {
				scene_.Update(deltaTime);

				// Animate the scene
				//player->SetHealth(sin(deltaTime*100*t) / 2 + 0.5);
				//player->SetShields(sin(deltaTime* 100 *t) / 2 + 0.5);

				scene_.GetScreen("healthBar")->SetProgress(player->GetHealth());
				scene_.GetScreen("shieldBar")->SetProgress(player->GetShields());

				last_time = current_time;
				t += 0.01;
			}
			if (current_time - second >= 1.0) {
				second = current_time;
				fps = frames;
				//std::cout << "fps: " << frames << std::endl;
				frames = 0;
			}
			frames += 1;
		}
		
		//text.RenderText("hello nmime", glm::vec2(0, 0), 1.0f, glm::vec3(1.0));
        // Draw the scene
        scene_.Draw(&camera_);
		text.RenderText(std::to_string(fps), glm::vec2(-1, 0.9), 0.5f, glm::vec3(1.0,1.0,0));

		//text.RenderText("hello nmime", glm::vec2(0, 0), 1.0f, glm::vec3(1.0));
		//text.RenderText("hello r", glm::vec2(400, 0), 1.0f, glm::vec3(1.0));
        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}

void Game::GetUserInput(float deltaTime) {

	//quit game
	if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, true);
	}

	// Stop animation if escape  is pressed
	if (timeOfLastMove + 0.5 < glfwGetTime()) {

		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			if (animating_) {
				animating_ = false;
				scene_.SetCurrentScreen(PAUSE_MENU);
				glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				scene_.SetCurrentScreen(HUD_MENU);
				animating_ = true;
				glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}

			timeOfLastMove = glfwGetTime();
		}

	}
	if (animating_) {
		GetMouseCameraInput();

		Player* player = scene_.GetPlayer();
		//Get the factors used for movement
		//glm::vec3 pos = player->GetPosition();
		glm::vec3 foward = camera_.GetForward();


		//Fire a missile
		if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
			//std::cout << "\n\nFIRE\n\n";
			player->Fire();
			timeOfLastMove = glfwGetTime();
		}
		if ((timeOfLastMove < glfwGetTime() - 0.6) && glfwGetKey(window_, GLFW_KEY_TAB) == GLFW_PRESS) {
			player->nextWeapon();
			timeOfLastMove = glfwGetTime();
		}

		//move the player forward as well as the camera
		if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		//if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
			//glm::vec3 pos = player->GetPosition();
			glm::vec3 foward = camera_.GetForward();

			
			if (player->GetBoosted() == 0) {
				audio_->playRepeat("playerEngine");
				player->SetBoosted(1);
				player->GetChild("jetstream")->SetDraw(true);
			}
		}
		else if(player->GetBoosted() == 1) {
			player->SetBoosted(0);
			audio_->stop("playerEngine");
			player->GetChild("jetstream")->SetDraw(false);
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


		glm::vec3 diff = player->GetPosition();
		glm::vec3 zoom = camera_.GetZoomPos();
		diff -= zoom.z * camera_.GetForward();
		diff -= zoom.y * camera_.GetUp();
		camera_.SetPosition(diff);

		player->GetOrientationObj()->Yaw(-speed_x);
		player->GetOrientationObj()->Pitch(-speed_y);
		//player->GetParticle()->GetOrientationObj()->Roll(-speed_x*0.5);
		//player->GetParticle()->GetOrientationObj()->Roll(-speed_y);
		//player->GetParticle()->SetOrientation(player->GetOrientation());
		camera_.SetOrientation(player->RotLagBehind(speed_y / 2, speed_x / 2));

		ScreenNode* screen = scene_.GetScreen("cockpit");
		screen->SetPosition(glm::vec3(speed_x*0.5,-0.1+speed_y*0.5, 0));
	}

}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    
}



void Game::ResizeCallback(GLFWwindow* window, int width, int height) {

	void* ptr = glfwGetWindowUserPointer(window);
	Game *game = (Game *)ptr;

	// Set up viewport and camera projection based on new window size
	glViewport(0, 0, width, height);
	game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
	game->window_width = width;
	game->window_height = height;




}
//https://www.glfw.org/docs/3.3/input_guide.html#scrolling
void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	void* ptr = glfwGetWindowUserPointer(window);
	Game *game = (Game *)ptr;
	yoffset *= -1;
	float max_zoom = 0.01;
	float min_zoom = 30;
	float curr_zoom = game->camera_.GetZoom()+yoffset;
	std::cout << "scroll is " << yoffset << ", zoom is " << curr_zoom << std::endl;
	if (curr_zoom < 5) {
		if (yoffset > 0) {
			game->first_person_ = false;
			game->scene_.GetScreen("cockpit")->SetDraw(false);
			game->scene_.GetPlayer()->SetDraw(true);
			game->camera_.Zoom(5 + yoffset);
			game->camera_.SetView(camera_position_3rd_g, camera_look_at_g, camera_up_g);
		}
		else {
			game->first_person_ = true;
			game->scene_.GetScreen("cockpit")->SetDraw(true);
			game->scene_.GetPlayer()->SetDraw(false);
			game->camera_.SetZoom(0);
			game->camera_.SetView(camera_position_1st_g, camera_look_at_g, camera_up_g);
		}
	}
	else if (curr_zoom<min_zoom && curr_zoom > max_zoom && !game->first_person_) {
		game->camera_.Zoom(yoffset);

	}

}
Game::~Game(){
    
    glfwTerminate();
}

ParticleNode* Game::CreateParticleInstance(int count, std::string particle_name, std::string object_name, std::string material_name, std::string texture_name) {
	//create resource
	resman_.CreateSphereParticles(particle_name, count);
	Resource *geom = resman_.GetResource(particle_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	// Create asteroid instance
	ParticleNode *pn = new ParticleNode(object_name, geom, mat);
	
	return pn;
}
Enemy *Game::CreateEnemyInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name){

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


void Game::CreateAsteroids(int num_enemies){

	float radius = 2000;
    // Create a number of asteroid instances
    for (int i = 0; i < num_enemies; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;
		std::string asteroid_type = "Asteroid"+std::to_string((rand() % 10) + 1);
        // Create asteroid instance
        SceneNode *en = CreateInstance(name, asteroid_type+"Mesh", "NormalMaterial",ASTEROID, asteroid_type+"Texture", asteroid_type + "Normal");//,

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum

		en->SetPosition(glm::vec3(-radius + radius *((float) rand() / RAND_MAX), -radius + radius *((float) rand() / RAND_MAX), -radius+radius*((float) rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
		//en->SetScale(glm::vec3(1000));
		en->SetScale(glm::vec3((rand() % 50) + 1));
		/*
		en->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
		*/
    }
}

void Game::CreateHUD(void) {
	ScreenNode* node;
	node  = CreateScreenInstance("cockpit", "FlatSurface", "ScreenMaterial",NONE, "cockpitTexture");
	node->SetOrientation(180, glm::vec3(1,0,0));
	node->SetScale(glm::vec3(2,1.2,1));

	//shield
	node = CreateScreenInstance("shieldBox", "FlatSurface", "ScreenMaterial", HUD_MENU, "shieldBoxTexture");
	node->SetOrientation(180, glm::vec3(1, 0, 0));
	node->SetScale(glm::vec3(0.85, 0.05,1));//multiply by 17
	node->SetPosition(glm::vec3(0,0.92,0));

	node = CreateScreenInstance("shieldBar", "FlatSurface", "ScreenMaterial", HUD_MENU, "shieldBarTexture");
	node->SetOrientation(180, glm::vec3(1, 0, 0));
	node->SetScale(glm::vec3(0.843, 0.04, 1));//multiply by 21
	node->SetPosition(glm::vec3(0, 0.92, 0));
	
	//health
	node = CreateScreenInstance("healthBox", "FlatSurface", "ScreenMaterial", HUD_MENU, "healthBoxTexture");
	node->SetOrientation(180, glm::vec3(1, 0, 0));
	node->SetScale(glm::vec3(0.41, 0.03416, 1));//multiply by 12
	node->SetPosition(glm::vec3(0, 0.86, 0));

	node = CreateScreenInstance("healthBar", "FlatSurface", "ScreenMaterial", HUD_MENU, "healthBarTexture");
	node->SetOrientation(180, glm::vec3(1, 0, 0));
	node->SetScale(glm::vec3(0.40, 0.02257, 1));//multiply by 17.72
	node->SetPosition(glm::vec3(0, 0.86, 0));
	
	//crosshair
	node = CreateScreenInstance("crosshair", "FlatSurface", "ScreenMaterial", HUD_MENU, "crosshairDefaultTexture");
	node->SetOrientation(180, glm::vec3(1, 0, 0));
	node->SetScale(glm::vec3(0.1));

	//radar
	node = CreateScreenInstance("radar", "FlatSurface", "RadarMaterial", HUD_MENU, "radarTexture");

	node->SetOrientation(180, glm::vec3(1, 0, 0));
	node->Rotate(30);
	node->SetScale(glm::vec3(0.3));
	node->SetPosition(glm::vec3(-0.75, -0.6, 0));

	//PAUSE MENU
	node = CreateScreenInstance("pauseBackground", "FlatSurface", "OverlayMaterial", PAUSE_MENU, "healthBarTexture");
	node->SetOrientation(180, glm::vec3(1, 0, 0));
	node->SetScale(glm::vec3(2));//multiply by 17.72crosshairDefaultTexture

}

ScreenNode *Game::CreateScreenInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name, std::string normal_name) {

	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	Resource *tex = NULL;
	if (texture_name != "") {
		tex = resman_.GetResource(texture_name);
		if (!tex) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}
	}
	;
	if (entity_name == "radar") {
		RadarNode* scn = new RadarNode(entity_name, geom, mat, tex);
		scene_.AddRadar(scn);
		return scn;
	}
	else {
		ScreenNode *scn = new ScreenNode(entity_name, geom, mat, tex);
		scene_.AddScreen(scn, type);
		return scn;
	}

}
SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, NodeType type, std::string texture_name, std::string normal_name){

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
	
	Resource *norm = NULL;
	if (normal_name != "") {
		norm = resman_.GetResource(normal_name);
		if (!tex) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}

	}
    SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, type, tex, norm);
    return scn;
}

} // namespace game
