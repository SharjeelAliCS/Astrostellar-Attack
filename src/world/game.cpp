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
float camera_far_clip_distance_g = 3000.0;
float camera_fov_g = 40.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_1st_g(0, 0, 1);
glm::vec3 camera_position_3rd_g(0, -1.5, 8);
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
	timeOfLastMove = 0.0;
	tabNotPressedLastFrame = false;
	spaceNotPressedLastFrame = false;
	oneNotPressedLastFrame = false;
	twoNotPressedLastFrame = false;
	threeNotPressedLastFrame = false;
	fourNotPressedLastFrame = false;

	first_person_ = true;
	evasiveManeuversCooldown = 0;

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

	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	animating_ = false;
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

	resman_.CreateSphereParticles("particleExplosion", 2000);
	resman_.CreateJetParticles("particleStream", 20000);
	resman_.CreateCometParticles("cometParticles", 3000);

	std::string filename;
	//load save data
	filename = SAVE_DIRECTORY + std::string("/game_state.json");
	resman_.LoadResource(Save, "save", filename.c_str());


	//load asset data
	filename = SAVE_DIRECTORY + std::string("/asset_data.json");
	resman_.LoadResource(Save, "assetList", filename.c_str());

	json assets = resman_.GetResource("assetList")->GetJSON();

	LoadSaveFile();

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
		text = TextRenderer(filename, resman_.GetResource("textMaterial"));
	}
	scene_.SetTextRenderer(&text);
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
	audio_->volume("asteroidExplosion", 200);
	audio_->volume("missileShot", 30);
	audio_->volume("enemyHit", 30);
	audio_->playRepeat("ambience");

}


void Game::LoadSaveFile(void) {

	//todo David
	json saveData = resman_.GetResource("save")->GetJSON();

	for (auto& gameData : saveData["inventory"].items()) {
		loadedPlayerInventory[gameData.key()] = gameData.value();
	}
	for (auto& gameData : saveData["upgrades"].items()) {
		loadedPlayerUpgrades[gameData.key()] = gameData.value();
	}
	for (auto& gameData : saveData["loadout"].items()) {
		loadedPlayerLoadout[gameData.key()] = gameData.value();
	}
	for (auto& gameData : saveData["player_stats"].items()) {
		loadedPlayerStats[gameData.key()] = gameData.value();
	}

	for (auto& gameData : saveData["weapon_stats"].items()) {
		loadedWeaponStats[gameData.key()] = gameData.value();
	}

	for (auto& bounty : saveData["bounty_data"].items()) {
		for (auto& gameData : saveData["bounty_data"][bounty.key()].items()) {
			loadedBountyStats[bounty.key()][gameData.key()] = gameData.value();
		}
	}

	for (auto& upgrade_levels : saveData["upgrade_costs"].items()) {
		for (auto& gameData : saveData["upgrade_costs"][upgrade_levels.key()].items()) {
			loadedUpgradeCosts[upgrade_levels.key()][gameData.key()] = gameData.value();
		}
	}

	startTime = saveData["playtime"];
	startKills = loadedPlayerStats["kills"];
	mouse_speed = saveData["mouse_speed"];
}
//david todo
void Game::SaveGame(void) {

	Resource* dataResource = resman_.GetResource("save");
	json newData = dataResource->GetJSON();

	for (std::map<std::string, int>::const_iterator it = loadedPlayerInventory.begin(); it != loadedPlayerInventory.end(); ++it){
		std::cout << it->first << " " << it->second<< "\n";
		newData["inventory"][it->first] = it->second;
	}
	for (std::map<std::string, int>::const_iterator it = loadedPlayerUpgrades.begin(); it != loadedPlayerUpgrades.end(); ++it){
		std::cout << it->first << " " << it->second<< "\n";
		newData["upgrades"][it->first] = it->second;
	}
	for (std::map<std::string, int>::const_iterator it = loadedPlayerLoadout.begin(); it != loadedPlayerLoadout.end(); ++it){
		std::cout << it->first << " " << it->second<< "\n";
		newData["loadout"][it->first] = it->second;
	}
	for (std::map<std::string, int>::const_iterator it = loadedPlayerStats.begin(); it != loadedPlayerStats.end(); ++it){
		std::cout << it->first << " " << it->second<< "\n";
		newData["player_stats"][it->first] = it->second;
	}
	newData["playtime"] = (int)(startTime+glfwGetTime());

	dataResource->SetJSON(newData);
	resman_.SaveResource("save");
}

void Game::LoadLastSave(void) {
	int lastStartTime = startTime;
	LoadSaveFile();
	startTime = lastStartTime;
	Player* player = scene_.GetPlayer();
	player->SetUpgrades(&loadedPlayerUpgrades);
	player->SetPlayerLoadout(&loadedPlayerLoadout);
	player->SetPlayerStats(&loadedPlayerStats);
	player->SetPlayerInventory(&loadedPlayerInventory);
	player->SetWeaponStats(&loadedWeaponStats);

}

void Game::SetupScene(void){


	scene_.SetAudio(audio_);
	NodeResources* rsc = GetResources("particleExplosion", "ExplosionParticleMaterial", "jetParticleTexture", "");

	scene_.SetDeathAnimation(*rsc);

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

	SceneNode* skybox = CreateInstance("skybox", "skybox", "SkyBoxMaterial", SKYBOX, "skyboxTexture");
	skybox->SetScale(glm::vec3(0.25));

	CreateHUD();

	scene_.UpdateScreenSizeNodes(window_width, window_height);
	// Setup drawing to texture
	scene_.SetupDrawToTexture(window_width, window_height);

	SetUpWorld(true);

}

void Game::SetUpWorld(bool restart) {

	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	animating_ = false;
	//create player object
	Player* player = dynamic_cast<Player*>(CreateInstance("player", "ship", "TextureShader", PLAYER, "shipTexture"));
	NodeResources* proj_rsc = GetResources("SimpleCylinderMesh", "ColoredMaterial", "", "");
	scene_.GetPlayer()->SetProjRsc(proj_rsc);
	player->SetScale(glm::vec3(2));
	player->SetOrientation(-90, glm::vec3(0, 1, 0));
	player->SetAudio(audio_);

	LoadLastSave();

	CreateEnemies(numEnemies);
	numEnemies = scene_.GetEnemies()->size(); //accounts for any rounding errors, should be handled by chosen number (60) but just in case that changes later
	CreateAsteroids(numAsteroids);
	CreateComets();
	//ame::SceneNode *wall = CreateInstance("Canvas", "FlatSurface", "Procedural", "RockyTexture"); // must supply a texture, even if not used
	//create skybox

	//create particles:
	ParticleNode* pn = CreateParticleInstance(20000, "jetstream", "particleStream", "ParticleMaterial", "jetParticleTexture");
	pn->SetOrientation(90, glm::vec3(0, 1, 0));
	pn->Rotate(-90, glm::vec3(1, 0, 0));
	pn->Translate(glm::vec3(1, 0.5, -3));
	pn->SetJoint(glm::vec3(0, 0.2, -1));
	pn->SetScale(glm::vec3(8));
	pn->SetDraw(false);
	pn->SetBlending(true);
	pn->SetColor(glm::vec3(0, 0.749, 1));
	//pn->SetJoint(glm::vec3(0, 0, -1));
	//scene_.GetPlayer()->SetParticleSystem(pn);

	scene_.GetPlayer()->AddChild(pn);
	//scene_.GetPlayer()->AddChild(test);
	scene_.GetScreen("cockpit")->SetDraw(true);
	scene_.GetPlayer()->SetDraw(false);
	camera_.SetZoom(0);
	camera_.SetView(camera_position_1st_g, camera_look_at_g, camera_up_g);
	// Set projection

	player->setAsteroids(scene_.GetAsteroids());
	player->setComets(scene_.GetComets());
	player->setEnemies(scene_.GetEnemies());
	player->setDeathAnimations(scene_.GetDeathAnimations());
	player->setCam(&camera_);

	SetSaveState();
}
void Game::ResetWorld(void) {
	scene_.ClearData();
	SetUpWorld(true);
}

void Game::SetSaveState(void) {

	//access save data
	Resource* dataResource = resman_.GetResource("save");
	json data = dataResource->GetJSON();

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
	Player* player = scene_.GetPlayer();

    while (!glfwWindowShouldClose(window_)){

		if (scene_.GetResetWorld()) {
			ResetWorld();
			player = scene_.GetPlayer();
			scene_.SetResetWorld(false);
		}
		else if (scene_.GetCurrentMenu() == DEATH_MENU) {
			animating_ = false;
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		static double last_time = glfwGetTime();
		static double last_update = glfwGetTime();

		double current_time = glfwGetTime();
		double deltaTime = current_time - last_time;
		last_time = current_time;
		// Animate the scene

		GetUserInput(deltaTime);

		if (animating_) {
			static double last_time = 0;
			double current_time = glfwGetTime();
			if ((current_time - last_update) > 0.05) {
				scene_.Update(deltaTime);

				ScreenType menu = scene_.GetCurrentMenu();
				switch (menu) {
				case MAIN_MENU:
					break;
				case PAUSE_MENU:
				case HUD_MENU:

					// Animate the scene
					//player->SetHealth(sin(deltaTime*100*t) / 2 + 0.5);
					//player->SetShields(sin(deltaTime* 100 *t) / 2 + 0.5);

					float s = player->GetShields();
					scene_.GetScreen("healthBar")->SetProgressX(player->getHealthPercent());
					scene_.GetScreen("shieldBar")->SetProgressX(player->GetShieldPercent());
					scene_.GetScreen("bountyBar")->SetProgressX(scene_.GetBountyProgress());
					Boss* boss = scene_.GetBoss();
					if(boss!=NULL)scene_.GetScreen("bossHealthBar")->SetProgressX(boss->getHealthPercent());
					std::string currWeapon = player->GetCurrentWeapon() + "Texture";
					scene_.GetScreen("weaponsHUD")->SetTexture(resman_.GetResource(currWeapon));
					scene_.GetScreen("boostBar")->SetProgressY(player->getBoostPercent());
					last_time = current_time;
					t += 0.01;


					break;

				}




			}
			if (current_time - second >= 1.0) {
				second = current_time;
				fps = frames;
				//std::cout << "fps: " << frames << std::endl;
				frames = 0;
			}
			frames += 1;
		}


		//gen the screen
		//bool genScreen = true;//change this value
		bool genScreen = player->NuclearOverload();
		scene_.Draw(&camera_, fps, true, window_width, window_height);
		scene_.DisplayScreenSpace(resman_.GetResource("ScreenHealthMaterial")->GetResource(),"health", genScreen, window_width, window_height);

		//the "nuclear" overload bool is used to check when to apply the screen effect or not.
		//scene_.Draw(&camera_, genScreen, window_width, window_height);

		//generate the bloom material screen material
		//scene_.DisplayScreenSpace(resman_.GetResource("ScreenBloomMaterial")->GetResource(),"", genScreen,window_width, window_height);
		if (genScreen) {

			//generate the boost screen material (main part of the assignment)
			scene_.DisplayScreenSpace(resman_.GetResource("ScreenBoostMaterial")->GetResource(),"boost");

		}

		//std::cout << loadedPlayerInventory[player->GetCurrentWeapon()+"_Ammo"]<< std::endl;
		//text.RenderText(new Text(player->GetCurrentWeapon(), glm::vec2(0.6, -0.78), 0.4f, glm::vec3(0.0941, 0.698, 0.921)));
		//text.RenderText(new Text(std::to_string(fps), glm::vec2(-1, 0.9), 0.5f, glm::vec3(1.0, 1.0, 0)));

        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}

//type is one of upgrade, ammo, repair
void Game::BuySomething(std::string thing, std::string type) {
	std::cout << "thing: " << thing << "\ntype: " << type << "\n";
	Player* player = scene_.GetPlayer();
	if (type.compare("repair")==0) {
		int cost = loadedPlayerStats["repair_cost_per_hp"] * (player->GetMaxHealth() - player->GetHealth());
		if (cost > loadedPlayerInventory["credits"]) {
			float hpRestored = loadedPlayerInventory["credits"] / loadedPlayerStats["repair_cost_per_hp"];
			player->SetHealth(player->GetHealth() + hpRestored);
			loadedPlayerInventory["credits"] = 0;
		}
		else {
			player->SetHealth(player->GetMaxHealth());
			loadedPlayerInventory["credits"] -= cost;
		}
	}

	else if (type.compare("upgrade")==0) {
		int currentLevel = loadedPlayerUpgrades[thing]+1;
		if (currentLevel == 6) { return; }
		std::cout << "level_" + std::to_string(currentLevel) << "\n";
		std::map<std::string, int> cost = loadedUpgradeCosts["level_" + std::to_string(currentLevel)];
		if( cost["credits"] <= loadedPlayerInventory["credits"] &&
			cost["stellaranite_Fragments"] <= loadedPlayerInventory["stellaranite_Fragments"] &&
			cost["stellaranite_Slabs"] <= loadedPlayerInventory["stellaranite_Slabs"])
		{
			std::cout << "credits: " << cost["credits"] << std::endl;
			std::cout << "stellaranite_Fragments: " << cost["stellaranite_Fragments"] << std::endl;
			std::cout << "stellaranite_Slabs: " << cost["stellaranite_Slabs"] << std::endl;
			loadedPlayerUpgrades[thing]++;
			loadedPlayerInventory["credits"] -= cost["credits"];
			loadedPlayerInventory["stellaranite_Fragments"] -= cost["stellaranite_Fragments"];
			loadedPlayerInventory["stellaranite_Slabs"] -= cost["stellaranite_Slabs"];


			if (thing.compare("ship_Health_Level") == 0) {
				player->SetMaxHealth(pow(1.2, loadedPlayerUpgrades[thing]) * loadedPlayerStats["ship_base_health"]);
			}
			else if (thing.compare("ship_Shield_Level") == 0) {
				player->SetMaxShields(pow(1.2, loadedPlayerUpgrades[thing]) * loadedPlayerStats["ship_base_shield"]);
			}
			else if (thing.compare("ship_Boost_Speed_Level") == 0) {
				player->IncreaseBoostSpeed(pow(1.1, loadedPlayerUpgrades[thing]));
			}
		}


	}
	else {
		int purchaseNumber = 0;
		if (type.compare("ammo") == 0) { //purchased in units of 10
			purchaseNumber = 10;
		}
		else if (type.compare("ability-ammo") == 0) { //purchased in units of 1
			purchaseNumber = 1;
		}
		int cost = loadedWeaponStats[thing + "_Cost"] * purchaseNumber;
		std::cout << "cost is: " << cost << " player has: " << loadedPlayerInventory["credits"] << "\n";
		if (cost <= loadedPlayerInventory["credits"]) {
			loadedPlayerInventory[thing + "_Ammo"] += purchaseNumber;
			loadedPlayerInventory["credits"] -= cost;
		}
	}
}

void Game::GetUserInput(float deltaTime) {

	//quit game
	if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, true);
	}
	//save game
	if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS && scene_.GetCurrentMenu() == MAIN_MENU) {
		SaveGame();
	}
	//load last save (will be option on death)
	if (glfwGetKey(window_, GLFW_KEY_L) == GLFW_PRESS && scene_.GetCurrentMenu() == MAIN_MENU) {
		LoadLastSave();
	}

	//load last save (will be option on death)
	if (glfwGetKey(window_, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
		animating_ = false;
		scene_.SetCurrentScreen(MAIN_MENU);
		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// Stop animation if escape  is pressed
	if (timeOfLastMove + 0.5 < glfwGetTime()) {

		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			if (animating_) {
				animating_ = false;
				scene_.SetCurrentScreen(PAUSE_MENU);
				glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else if(scene_.GetCurrentMenu()==PAUSE_MENU){
				scene_.SetCurrentScreen(HUD_MENU);
				animating_ = true;
				glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}

			timeOfLastMove = glfwGetTime();
		}

	}
	double xpos, ypos;
	glfwGetCursorPos(window_, &xpos, &ypos);

	std::string btn = scene_.ButtonEvents(xpos, ypos);
	if (btn != "" && (timeOfLastMove < glfwGetTime() - 0.5) && glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			timeOfLastMove = glfwGetTime();

			if (btn == "resumeButton") {
				scene_.SetCurrentScreen(HUD_MENU);
				animating_ = true;
				glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			}
			else if (btn == "mainMenuRestartButton" || btn =="mainMenuRestartDeathButton") {
				scene_.SetCurrentScreen(MAIN_MENU);
				animating_ = false;
				scene_.SetResetWorld(true);
				glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			}
			else if (btn == "quitDesktopButton" || btn=="quitDesktopDeathButton") {
				glfwSetWindowShouldClose(window_, true);
			}
			else if (btn == "startButton") {
				if (scene_.GetCurrentBounty().compare("")) {
					scene_.SetCurrentScreen(HUD_MENU);
					animating_ = true;
					glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				}
			}
			else if (btn == "shopButton") {
				scene_.SetCurrentScreen(SHOP_MENU);
				std::cout << "shop clicked!" << std::endl;
			}
			else if (btn == "menuButton") {
				scene_.SetCurrentScreen(MAIN_MENU);
				std::cout << "menu clicked!" << std::endl;
			}
			else if (btn == "bounty1Button") {

				std::cout << "bounty1Button clicked!" << std::endl;
				scene_.SetBounty("destroy_60_asteroids_reward",60, loadedBountyStats["destroy_60_asteroids_reward"]);
			}
			else if (btn == "bounty2Button") {
				std::cout << "bounty2Button clicked!" << std::endl;
				scene_.SetBounty("kill_40_enemies_reward", 40,loadedBountyStats["kill_40_enemies_reward"]);
			}
			else if (btn == "bounty3Button") {
				std::cout << "bounty3Button clicked!" << std::endl;
				CreateBoss();
				scene_.SetBounty("kill_boss_reward", 1,loadedBountyStats["kill_boss_reward"]);
			}
			else if (btn == "shipHealthButton") {
				std::cout << "shipHealthButton clicked!" << std::endl;
				BuySomething("ship_Health_Level", "upgrade");
			}
			else if (btn == "RepairBtn") {
				std::cout << "RepairBtn clicked!" << std::endl;
				BuySomething("repair", "repair");
			}
			else if (btn == "shipShieldButton") {
				std::cout << "shipShieldButton clicked!" << std::endl;
				BuySomething("ship_Shield_Level", "upgrade");
			}
			else if (btn == "shipSpeedButton") {
				std::cout << "shipSpeedButton clicked!" << std::endl;
				BuySomething("ship_Boost_Speed_Level", "upgrade");
			}
			else if (btn == "laserRangeBtn") {
				std::cout << "laserRangeBtn clicked!" << std::endl;
				BuySomething("laser_Battery_Range_Level", "upgrade");
			}
			else if (btn == "laserDmgBtn") {
				std::cout << "laserDmgBtn clicked!" << std::endl;
				BuySomething("laser_Battery_Damage_Level", "upgrade");
			}
			else if (btn == "laserPierceBtn") {
				std::cout << "laserPierceBtn clicked!" << std::endl;
				BuySomething("laser_Battery_Pierce_Level", "upgrade");
			}
			else if (btn == "NaniteAmmoBtn") {
				std::cout << "NaniteAmmoBtn clicked!" << std::endl;
				BuySomething("nanite_Torpedo", "ammo");
			}
			else if (btn == "NaniteDmgBtn") {
				std::cout << "NaniteDmgBtn clicked!" << std::endl;
				BuySomething("nanite_Torpedo_Damage_Level", "upgrade");
			}
			else if (btn == "NaniteDurBtn") {
				std::cout << "NaniteDurBtn clicked!" << std::endl;
				BuySomething("nanite_Torpedo_Duration_Level", "upgrade");
			}
			else if (btn == "NaniteStackBtn") {
				std::cout << "NaniteStackBtn clicked!" << std::endl;
				BuySomething("nanite_Torpedo_Stack_Level", "upgrade");
			}
			else if (btn == "chargeAmmoBtn") {
				std::cout << "chargeAmmoBtn clicked!" << std::endl;
				BuySomething("charge_Blast", "ammo");
			}
			else if (btn == "chargeDmgBtn") {
				std::cout << "chargeDmgBtn clicked!" << std::endl;
				BuySomething("charge_Blast_Damage_Level", "upgrade");
			}
			else if (btn == "chargeSizeBtn") {
				std::cout << "chargeSizeBtn clicked!" << std::endl;
				BuySomething("charge_Blast_Radius_Level", "upgrade");
			}
			else if (btn == "sniperAmmoBtn") {
				std::cout << "sniperAmmoBtn clicked!" << std::endl;
				BuySomething("sniper_shot", "ammo");
			}
			else if (btn == "sniperDmgBtn") {
				std::cout << "sniperDmgBtn clicked!" << std::endl;
				BuySomething("sniper_Shot_Damage_Level", "upgrade");
			}
			else if (btn == "sniperRangeBtn") {
				std::cout << "sniperRangeBtn clicked!" << std::endl;
				BuySomething("sniper_Shot_Range_Level", "upgrade");
			}
			else if (btn == "shotgunAmmoBtn") {
				std::cout << "shotgunAmmoBtn clicked!" << std::endl;
				BuySomething("shotgun", "ammo");
			}
			else if (btn == "shotgunDmgBtn") {
				std::cout << "shotgunDmgBtn clicked!" << std::endl;
				BuySomething("shotgun_Damage_Level", "upgrade");
			}
			else if (btn == "shotgunNumBtn") {
				std::cout << "shotgunNumBtn clicked!" << std::endl;
				BuySomething("shotgun_NumShots_Level", "upgrade");
			}
			else if (btn == "pursuerAmmoBtn") {
				std::cout << "pursuerAmmoBtn clicked!" << std::endl;
				BuySomething("pursuer", "ammo");
			}
			else if (btn == "pursuerROFBtn") {
				std::cout << "pursuerROFBtn clicked!" << std::endl;
				BuySomething("pursuer_ROF_Level", "upgrade");
			}
			else if (btn == "naniteSwarmDmgBtn") {
				std::cout << "naniteSwarmDmgBtn clicked!" << std::endl;
				BuySomething("nanite_Swarm_Damage_Level", "upgrade");
			}
			else if (btn == "naniteSwarmRadiusBtn") {
				std::cout << "naniteSwarmRadiusBtn clicked!" << std::endl;
				BuySomething("nanite_Swarm_Radius_Level", "upgrade");
			}
			else if (btn == "naniteSwarmDurBtn") {
				std::cout << "naniteSwarmDurBtn clicked!" << std::endl;
				BuySomething("nanite_Swarm_Duration_Level", "upgrade");
			}
			else if (btn == "emgWarpAmmoBtn") {
				std::cout << "emgWarpAmmoBtn clicked!" << std::endl;
				BuySomething("emergency_Warp", "ability-ammo");
			}
			else if (btn == "chronoSurgeAmmoBtn") {
				std::cout << "chronoSurgeAmmoBtn clicked!" << std::endl;
				BuySomething("chrono_Surge", "ability-ammo");
			}
			else if (btn == "barrierDurBtn") {
				std::cout << "barrierDurBtn clicked!" << std::endl;
				BuySomething("barrier_Duration_Level", "upgrade");
			}
			else if (btn == "barrierSizeBtn") {
				std::cout << "barrierSizeBtn clicked!" << std::endl;
				BuySomething("barrier_Size_Level", "upgrade");
			}
			else if (btn == "batteryOverChargeBtn") {
				std::cout << "batteryOverChargeBtn clicked!" << std::endl;
				BuySomething("battery_Overcharge_Level", "upgrade");
			}
			else if (btn == "emergencyWarpBtn") {
				std::cout << "emergencyWarpBtn clicked!" << std::endl;
				BuySomething("emergency_Warp_Level", "upgrade");
			}
			else if (btn == "emergencyManuBtn") {
				std::cout << "emergencyManuBtn clicked!" << std::endl;
				BuySomething("evasive_Maneuvers_Level", "upgrade");
			}
			else if (btn == "chronoSurgeBtn") {
				std::cout << "chronoSurgeBtn clicked!" << std::endl;
				BuySomething("chrono_Surge_Level", "upgrade");
			}
			//shopButton
	//startButton
	//bounty1Button
	//bounty2Button
	//bounty3Button
	//shipHealthButton
	//shipShieldButton
	//shipSpeedButton

	//laserRangeBtn
	//laserDmgBtn
	//laserPierceBtn

	//NaniteAmmoBtn
	//NaniteDmgBtn
	//NaniteDurBtn
	//NaniteStackBtn

	//chargeAmmoBtn
	//chargeDmgBtn
	//chargeSizeBtn

	//sniperAmmoBtn
	//sniperDmgBtn
	//sniperRangeBtn

	//shotgunAmmoBtn
	//shotgunDmgBtn
	//shotgunNumBtn

	//pursuerAmmoBtn
	//pursuerROFBtn

	//naniteSwarmDmgBtn
	//pursuerROFBtn
	//naniteSwarmRadiusBtn


	//emgWarpAmmoBtn
	//chronoSurgeAmmoBtn

	//barrierDurBtn
	//barrierSizeBtn
	//batteryOverChargeBtn
	//chronoSurgeBtn
	//emergencyWarpBtn
	//emergencyManuBtn

	}

	if (animating_) {

		GetMouseCameraInput(xpos, ypos);

		Player* player = scene_.GetPlayer();
		//Get the factors used for movement
		//glm::vec3 pos = player->GetPosition();
		glm::vec3 foward = camera_.GetForward();
		//keep track of kills
		loadedPlayerStats["kills"] = startKills + numEnemies - scene_.GetEnemies()->size();
		//Fire a missile
		if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			//std::cout << "\n\nFIRE\n\n";
			player->Fire();
			timeOfLastMove = glfwGetTime();
		}


		if (tabNotPressedLastFrame && glfwGetKey(window_, GLFW_KEY_TAB) == GLFW_PRESS) {
			player->nextWeapon();
			timeOfLastMove = glfwGetTime();
		}
		tabNotPressedLastFrame = glfwGetKey(window_, GLFW_KEY_TAB) == GLFW_RELEASE;

		//slow time
		if (oneNotPressedLastFrame && glfwGetKey(window_, GLFW_KEY_1) == GLFW_PRESS) {
			//if ammo use ability
			if (loadedPlayerInventory["chrono_Surge_Ammo"] > 0) {
				//slow time for 10 seconds, up to 25 at max level
				scene_.SlowTime(10 * pow(1.2, loadedPlayerUpgrades["chrono_Surge_Level"]));
				loadedPlayerInventory["chrono_Surge_Ammo"]--;
			}
		}
		oneNotPressedLastFrame = glfwGetKey(window_, GLFW_KEY_1) == GLFW_RELEASE;

		//emergency warp, levels increase distance travelled.
		if (twoNotPressedLastFrame && glfwGetKey(window_, GLFW_KEY_2) == GLFW_PRESS) {
			//if ammo use ability
			if (loadedPlayerInventory["emergency_Warp_Ammo"] > 0) {
				loadedPlayerInventory["emergency_Warp_Ammo"]--;
				int range = (int)(200 * pow(1.22, loadedPlayerUpgrades["emergency_Warp_Level"]));
				scene_.SlowTime(1);    //slow time during the effect
				player->MakeInvuln(1); //player immune to damage during effect
				//warp the player to a random position within +/-[range/2, range]
				player->SetPosition(player->GetPosition() + glm::vec3(((((rand() % range) * 0.5) + (range * 0.5)) * ((double)(rand() % 2) - 1)),
																	  ((((rand() % range) * 0.5) + (range * 0.5)) * ((double)(rand() % 2) - 1)),
																	  ((((rand() % range) * 0.5) + (range * 0.5)) * ((double)(rand() % 2) - 1))));
			}
		}
		twoNotPressedLastFrame = glfwGetKey(window_, GLFW_KEY_2) == GLFW_RELEASE;
		//nanite_swarm, levels increase duration. all enemies cannot shoot while this effect lasts
		if (threeNotPressedLastFrame && glfwGetKey(window_, GLFW_KEY_3) == GLFW_PRESS) {
			//if ammo use ability
			if (loadedPlayerInventory["nanite_Swarm_Ammo"] > 0) {
				loadedPlayerInventory["nanite_Swarm_Ammo"]--;
				std::cout << "swarm active\n";
				scene_.DisableAllEnemiesFor(loadedWeaponStats["nanite_Swarm_BaseDisableDuration"] * pow(1.25, loadedPlayerUpgrades["nanite_Swarm_Duration_Level"]));
			}
		}
		threeNotPressedLastFrame = glfwGetKey(window_, GLFW_KEY_3) == GLFW_RELEASE;

		if (spaceNotPressedLastFrame && glfwGetTime()>evasiveManeuversCooldown && glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
			//evasive manuvers
			//if success, 1 sec cooldown and bonus damage next attack, else 5 second cooldown no bonus
			if (scene_.EvasiveManeuversSuccessCheck()) {
				std::cout << "success! cooldown 1 second\n";
				//bonus damage next attack
				//barrel roll! I wasn't able to get to this, if you got time it would def make the ability cooler. shar todo

				scene_.SlowTime(1.5);    //slow time during the effect
				player->MakeInvuln(1.5); //player immune to damage during effect
				player->ImproveNextShot(); //next bullet does increased damage

				evasiveManeuversCooldown = glfwGetTime() + 1;
			}
			else {
				std::cout << "failure! cooldown 5 seconds\n";
				evasiveManeuversCooldown = glfwGetTime() + 5;
			}
		}
		spaceNotPressedLastFrame = glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_RELEASE;


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

void Game::GetMouseCameraInput(float xpos, float ypos) {

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
		speed_x /= mouse_speed;
		speed_y /= mouse_speed;
		glfwSetCursorPos(window_, window_width / 2, window_height / 2);


		glm::vec3 diff = player->GetPosition();
		glm::vec3 zoom = camera_.GetZoomPos();
		diff -= zoom.z * camera_.GetForward();
		diff -= zoom.y * camera_.GetUp();
		camera_.SetPosition(diff);

		player->GetOrientationObj()->Yaw(-speed_x);
		player->GetOrientationObj()->Pitch(-speed_y);

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
	game->scene_.UpdateScreenSizeNodes(width, height);

	game->scene_.SetupDrawToTexture(width, height);




}
//https://www.glfw.org/docs/3.3/input_guide.html#scrolling
void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	void* ptr = glfwGetWindowUserPointer(window);
	Game *game = (Game *)ptr;
	yoffset *= -1;
	float max_zoom = 0.01;
	float min_zoom = 30;
	float curr_zoom = game->camera_.GetZoom()+yoffset;

	if (curr_zoom < 5) {
		if (yoffset > 0) {
			game->first_person_ = false;
			game->scene_.GetPlayer()->SetFirstPerson(false);
			game->scene_.GetScreen("cockpit")->SetDraw(false);
			game->scene_.GetPlayer()->SetDraw(true);
			game->camera_.Zoom(5 + yoffset);
			game->camera_.SetView(camera_position_3rd_g, camera_look_at_g, camera_up_g);
		}
		else {
			game->first_person_ = true;
			game->scene_.GetPlayer()->SetFirstPerson(true);
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

void Game::SetEnemyStats(std::string type, Enemy* en, json data) {

	en->SetEnemyType(type);

	en->SetMovementSpeed(data[type]["speed"]);
	en->SetMaxHealth(data[type]["health"]);

	en->SetProjectileDmg(data[type]["bullet_damage"]);
	en->SetDamage(data[type]["ram_damage"]);

	json saveData = resman_.GetResource("save")->GetJSON();
	std::map<std::string, int> drops;
	for (auto& gameData : saveData["enemies"][type]["drops"].items()) {
		drops[gameData.key()] = gameData.value();
	}
	en->SetDrops(drops);
}

void Game::CreateBoss(void) {
	Resource* dataResource = resman_.GetResource("save");
	json data = dataResource->GetJSON()["enemies"];

	Boss *en = CreateBossInstance("Boss", "bossMesh", "TextureShader", "bossTexture");
	NodeResources* proj = GetResources("SimpleCylinderMesh", "ColoredMaterial", "", "");
	en->SetScale(glm::vec3(30));
	en->SetPosition(glm::vec3(-1000, 0, 0));
	SetEnemyStats("Boss", en, data);
	en->SetProjRsc(proj);
	en->SetPlayer(scene_.GetPlayer());
	en->SetProjectileDmg(data["Boss"]["orbs"]["bullet_damage"]);
	en->SetProjColor(glm::vec3(1, 1, 0));
	NodeResources* rsc = GetResources("bossOrbMesh", "TextureShader", "bossOrbTexture", "");
	// Create asteroid instance
	int total_orbs = data["Boss"]["orbs"]["count"];
	en->SetRateOfFire(1);

	en->SetPlayer(scene_.GetPlayer());

	for (int i = 0; i < total_orbs; i++) {
		Enemy *pn = new Enemy("bossOrb", rsc->geom, rsc->mat, rsc->tex);
		pn->SetPosition(glm::vec3(100, 0, 0));
		pn->SetJoint(glm::vec3(100, 0, 0));
		pn->Rotate(360/total_orbs*i, glm::vec3(0, 0, 1));

		pn->RotateOrientationInit(40, glm::vec3(0, 0, 1));
		pn->SetScale(glm::vec3(10));
		pn->AddParent(en);

		pn->SetNodeResources(rsc);
		pn->SetProjRsc(proj);
		pn->SetPlayer(scene_.GetPlayer());
		pn->SetProjectileDmg(data["Boss"]["orbs"]["bullet_damage"]);
		pn->SetProjColor(glm::vec3(1, 1, 0));
		float rof = data["Boss"]["orbs"]["rof"];
		rof += -1 + 1 * ((float)rand() / RAND_MAX);
		pn->SetRateOfFire(rof);

		en->AddOrb(pn);


	}
}
void Game::CreateEnemies(int num_enemies) {

	float radius = 1000;
	float default_scale = 5;
	Resource* dataResource = resman_.GetResource("save");
	json data = dataResource->GetJSON()["enemies"];

	// Create a number of grunt enemies
	std::string enemy_type = "Grunt";
	for (int i = 0; i < floor(num_enemies*(float)data[enemy_type]["amount"]); i++) {
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		std::string name = "enemy_"+enemy_type + index;
		Enemy *en = CreateEnemyInstance(name, "EnemyMesh", "TextureShader","enemy"+enemy_type+"Texture");
		SetEnemyStats(enemy_type, en, data);

		en->SetPosition(glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		en->SetScale(glm::vec3(default_scale));
	}

	enemy_type = "Ram";
	for (int i = 0; i < floor(num_enemies*(float)data[enemy_type]["amount"]); i++) {
		// Create a number of ram enemies
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		std::string name = "enemy_" + enemy_type + index;
		Enemy *en = CreateEnemyInstance(name, "EnemyMesh", "TextureShader", "enemy" + enemy_type + "Texture");
		SetEnemyStats(enemy_type, en, data);

		en->SetPosition(glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		en->SetScale(glm::vec3(default_scale));
	}

	enemy_type = "Tank";
	for (int i = 0; i < floor(num_enemies*(float)data[enemy_type]["amount"]); i++) {
		// Create a number of ram enemies
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		std::string name = "enemy_" + enemy_type + index;
		Enemy *en = CreateEnemyInstance(name, "EnemyMesh", "TextureShader", "enemy" + enemy_type + "Texture");
		SetEnemyStats(enemy_type, en, data);

		en->SetPosition(glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		en->SetScale(glm::vec3(default_scale*2));
	}

	enemy_type = "Speedster";
	for (int i = 0; i < floor(num_enemies*(float)data[enemy_type]["amount"]); i++) {
		// Create a number of ram enemies
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		std::string name = "enemy_" + enemy_type + index;
		Enemy *en = CreateEnemyInstance(name, "EnemyMesh", "TextureShader", "enemy" + enemy_type + "Texture");
		SetEnemyStats(enemy_type, en, data);

		en->SetPosition(glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		en->SetScale(glm::vec3(default_scale));
	}

	enemy_type = "Splitter";
	for (int i = 0; i < floor(num_enemies*(float)data[enemy_type]["amount"]); i++) {
		// Create a number of ram enemies
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		std::string name = "enemy_" + enemy_type + index;
		Enemy *en = CreateEnemyInstance(name, "EnemyMesh", "TextureShader", "enemy" + enemy_type + "Texture");
		SetEnemyStats(enemy_type, en, data);

		en->SetPosition(glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		en->SetScale(glm::vec3(default_scale));
		en->SetPhase(data[enemy_type]["phases"]);
	}

	enemy_type = "Disrupter";
	for (int i = 0; i < floor(num_enemies*(float)data[enemy_type]["amount"]); i++) {
		// Create a number of ram enemies
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		std::string name = "enemy_" + enemy_type + index;
		Enemy *en = CreateEnemyInstance(name, "EnemyMesh", "TextureShader", "enemy" + enemy_type + "Texture");
		SetEnemyStats(enemy_type, en, data);

		en->SetPosition(glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX)));
		en->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		en->SetScale(glm::vec3(default_scale));
	}

}

void Game::CreateAsteroids(int num_asteroids){

	float radius = 4000;
    // Create a number of asteroid instances
    for (int i = 0; i < num_asteroids; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;
		std::string asteroid_type = "Asteroid"+std::to_string((rand() % 10) + 1);
        // Create asteroid instance
        AsteroidNode *ast = CreateAsteroidInstance(name, asteroid_type+"Mesh", "NormalMaterial", asteroid_type+"Texture", asteroid_type + "Normal");//,

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum

		ast->SetPosition(glm::vec3(-radius + radius *((float) rand() / RAND_MAX), -radius + radius *((float) rand() / RAND_MAX), -radius+radius*((float) rand() / RAND_MAX)));
		ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
		//en->SetScale(glm::vec3(1000));
		ast->SetScale(glm::vec3((rand() % 30) + 1));
		/*
		en->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
		*/
    }
}

void Game::CreateComets(int num_comets) {
	float radius = 4000;
	for (int i = 0; i < num_comets; i++) {
		// Create instance name
		std::stringstream ss;
		ss << i;
		std::string index = ss.str();
		std::string name = "Comet" + index;
		std::string asteroid_type = "Asteroid" + std::to_string((rand() % 10) + 1);
		// Create asteroid instance
		CometNode *cmt = CreateCometNode(name, asteroid_type + "Mesh", "NormalMaterial", asteroid_type + "Texture", asteroid_type + "Normal");//,

		// Set attributes of asteroid: random position, orientation, and
		// angular momentum

		cmt->SetPosition(glm::vec3(-radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX), -radius + radius * ((float)rand() / RAND_MAX)));
		cmt->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		float scale = 3;
		cmt->SetScale(glm::vec3(scale));
		cmt->SetColor(glm::vec3(1,0.5,0.5));


		ParticleNode* pn = CreateParticleInstance(20000, "cometStream", "cometParticles", "CometParticleMaterial", "jetParticleTexture");
		pn->SetOrientation(90, glm::vec3(0, -1, 0));
		float pos = scale / 2;
		pn->SetPosition(glm::vec3(-cmt->GetOrientationObj()->GetForward()*pos));
		pn->Rotate(-90, glm::vec3(-1, 0, 0));
		pn->SetJoint(glm::vec3(0, 0.2, -1));
		pn->SetScale(glm::vec3(scale*5));
		pn->SetDraw(false);
		pn->SetBlending(true);
		pn->SetColor(glm::vec3(0.8,0.8,1));
		pn->SetDraw(true);
		cmt->AddChild(pn);

	}
}
void Game::CreateHUD(void) {
	ScreenNode* node;
	node  = CreateScreenInstance("cockpit", "FlatSurface", "ScreenMaterial",NONE, "cockpitTexture");
	node->SetScale(glm::vec3(2,1.2,1));

	//shield
	node = CreateScreenInstance("shieldBox", "FlatSurface", "ScreenMaterial", HUD_MENU, "shieldBoxTexture");
	node->SetScale(glm::vec3(0.85, 0.05,1));//multiply by 17
	node->SetPosition(glm::vec3(0,0.92,0));

	node = CreateScreenInstance("shieldBar", "FlatSurface", "ScreenMaterial", HUD_MENU, "shieldBarTexture");
	node->SetScale(glm::vec3(0.843, 0.04, 1));//multiply by 21
	node->SetPosition(glm::vec3(0, 0.92, 0));

	//health
	node = CreateScreenInstance("healthBox", "FlatSurface", "ScreenMaterial", HUD_MENU, "healthBoxTexture");
	node->SetScale(glm::vec3(0.41, 0.03416, 1));//multiply by 12
	node->SetPosition(glm::vec3(0, 0.86, 0));

	node = CreateScreenInstance("healthBar", "FlatSurface", "ScreenMaterial", HUD_MENU, "healthBarTexture");
	node->SetScale(glm::vec3(0.40, 0.02257, 1));//multiply by 17.72
	node->SetPosition(glm::vec3(0, 0.86, 0));

	//boost
	node = CreateScreenInstance("boostBox", "FlatSurface", "ScreenMaterial", HUD_MENU, "boostBoxTexture");
	node->SetScale(glm::vec3(0.07, 0.07*4, 1));//multiply by 3.3333
	node->SetPosition(glm::vec3(-0.9, -0.0,0));

	node = CreateScreenInstance("boostBar", "FlatSurface", "ScreenMaterial", HUD_MENU, "boostBarTexture");
	node->SetScale(glm::vec3(0.063, 0.063 *3.6, 1));//multiply by 17.72
	node->SetPosition(glm::vec3(-0.9, 0.045, 0));

	//bounty progress
	node = CreateScreenInstance("bountyBox", "FlatSurface", "ScreenMaterial", HUD_MENU, "bountyBoxTexture");
	node->SetScale(glm::vec3(0.4, 0.04, 1));//multiply by 10
	node->SetPosition(glm::vec3(-0.75, 0.85, 0));

	node = CreateScreenInstance("bountyBar", "FlatSurface", "ScreenMaterial", HUD_MENU, "bountyBarTexture");
	node->SetScale(glm::vec3(0.397, 0.037, 1));//multiply by 10
	node->SetPosition(glm::vec3(-0.75, 0.85, 0));
	//enemy health bar
	node = CreateScreenInstance("enemyHealthBox", "FlatSurface", "ScreenMaterial", ENEMY_HEALTH, "enemyHealthBoxTexture");
	node->SetScale(glm::vec3(0.1, 0.1*0.1,1));//multiply by 0.2

	node = CreateScreenInstance("enemyHealthBar", "FlatSurface", "ScreenMaterial", ENEMY_HEALTH, "enemyHealthBarTexture");
	node->SetScale(glm::vec3(0.1, 0.1*0.1, 1));//multiply by 0.2

	//boss health bar
	node = CreateScreenInstance("bossHealthBox", "FlatSurface", "ScreenMaterial", HUD_MENU, "bossBoxTexture");
	node->SetScale(glm::vec3(0.8, 0.04, 1));//multiply by 0.2
	node->SetPosition(glm::vec3(0, -0.87, 0));

	node = CreateScreenInstance("bossHealthBar", "FlatSurface", "ScreenMaterial", HUD_MENU, "bossBarTexture");
	node->SetScale(glm::vec3(0.79, 0.034, 1));//multiply by 0.2
	node->SetPosition(glm::vec3(0, -0.87, 0));

	//crosshair
	node = CreateScreenInstance("crosshair", "FlatSurface", "ScreenMaterial", HUD_MENU, "crosshairDefaultTexture");
	node->SetScale(glm::vec3(0.1));

	//radar
	node = CreateScreenInstance("radar", "FlatSurface", "RadarMaterial", HUD_MENU, "radarTexture");
	node->Rotate(30);
	node->SetScale(glm::vec3(0.25));
	node->SetPosition(glm::vec3(-0.85, -0.6, 0));

	//currancy
	node = CreateScreenInstance("creditsIcon", "FlatSurface", "ScreenMaterial", HUD_MENU, "creditsTexture");
	node->SetScale(glm::vec3(0.1));
	node->SetPosition(glm::vec3(0.93, 0.9, 0));

	node = CreateScreenInstance("shardsIcon", "FlatSurface", "ScreenMaterial", HUD_MENU, "shardsTexture");
	node->SetScale(glm::vec3(0.1));
	node->SetPosition(glm::vec3(0.93, 0.77, 0));

	node = CreateScreenInstance("slabsIcon", "FlatSurface", "ScreenMaterial", HUD_MENU, "slabsTexture");
	node->SetScale(glm::vec3(0.1));
	node->SetPosition(glm::vec3(0.93, 0.64, 0));

	//WEAPONS
	node = CreateScreenInstance("weaponsHUD", "FlatSurface", "ScreenMaterial", HUD_MENU, "laser_BatteryTexture");
	node->SetScale(glm::vec3(0.384,0.08,1));//multiply by 4.8
	node->SetPosition(glm::vec3(0.75, -0.6, 0));
	//PAUSE MENU
	node = CreateScreenInstance("pauseBackground", "FlatSurface", "OverlayMaterial", PAUSE_MENU, "healthBarTexture");
	node->SetScale(glm::vec3(2));//multiply by 17.72crosshairDefaultTexture

	ButtonNode* btn = CreateButtonInstance("resumeButton", "FlatSurface", "ScreenMaterial", PAUSE_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.25,0.25,1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.8, 0.5, 0));
	btn->SetText(new Text("Resume", glm::vec2(-1.0,-1.7), 0.3, glm::vec3(0,0.6,0.83)));

	btn = CreateButtonInstance("quitDesktopButton", "FlatSurface", "ScreenMaterial", PAUSE_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.25, 0.25, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.45, 0.5, 0));
	btn->SetText(new Text("Desktop", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(0, 0.6, 0.83)));

	btn = CreateButtonInstance("mainMenuRestartButton", "FlatSurface", "ScreenMaterial", PAUSE_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.25, 0.25, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.05, 0.5, 0));
	btn->SetText(new Text("Main Menu", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(0, 0.6, 0.83)));

	//--------------------------DEATH MENU---------------------

	btn = CreateButtonInstance("quitDesktopDeathButton", "FlatSurface", "ScreenMaterial", DEATH_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.25, 0.25, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.45, 0.5, 0));
	btn->SetText(new Text("Desktop", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(0, 0.6, 0.83)));

	btn = CreateButtonInstance("mainMenuRestartDeathButton", "FlatSurface", "ScreenMaterial", DEATH_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.25, 0.25, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.05, 0.5, 0));
	btn->SetText(new Text("Main Menu", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(0, 0.6, 0.83)));

	//buttons
	btn = CreateButtonInstance("shopButton", "FlatSurface", "ScreenMaterial", MAIN_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.85, 0.7, 0));
	btn->SetText(new Text("Shop", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(1.0,1.0,1.0)));

	btn = CreateButtonInstance("menuButton", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.85, 0.7, 0));
	btn->SetText(new Text("Main Menu", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("startButton", "FlatSurface", "ScreenMaterial", MAIN_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.6, 0.7, 0));
	btn->SetText(new Text("Start", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("bounty1Button", "FlatSurface", "ScreenMaterial", MAIN_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.35, 0.7, 0));
	btn->SetText(new Text("Bounty 1", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("bounty2Button", "FlatSurface", "ScreenMaterial", MAIN_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.1, 0.7, 0));
	btn->SetText(new Text("Bounty 2", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("bounty3Button", "FlatSurface", "ScreenMaterial", MAIN_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.15, 0.7, 0));
	btn->SetText(new Text("Bounty 3", glm::vec2(-1.0, -1.7), 0.3, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("shipHealthButton", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.4, 0.7, 0));
	btn->SetText(new Text("Upgrade health", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("shipShieldButton", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.65, 0.7, 0));
	btn->SetText(new Text("Upgrade shields", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("shipSpeedButton", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.9, 0.7, 0));
	btn->SetText(new Text("Upgrade speeds", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("laserRangeBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.85, 0.3, 0));
	btn->SetText(new Text("laser range", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("laserDmgBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.85, -0.1, 0));
	btn->SetText(new Text("laser dmg", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("laserPierceBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.85, -0.5, 0));
	btn->SetText(new Text("laser pierce", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("NaniteAmmoBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.6, 0.3, 0));
	btn->SetText(new Text("nanite ammo", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("NaniteDmgBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.6, -0.1, 0));
	btn->SetText(new Text("nanite dmg", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("NaniteDurBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.6, -0.5, 0));
	btn->SetText(new Text("nanite duration", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("chargeAmmoBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.35, 0.3, 0));
	btn->SetText(new Text("charge ammo", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("chargeDmgBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.35, -0.1, 0));
	btn->SetText(new Text("charge dmg", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("chargeSizeBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.35, -0.5, 0));
	btn->SetText(new Text("charge size", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("sniperAmmoBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.1, 0.3, 0));
	btn->SetText(new Text("sniper ammo", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("sniperDmgBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.1, -0.1, 0));
	btn->SetText(new Text("sniper dmg", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("sniperRangeBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.1, -0.5, 0));
	btn->SetText(new Text("sniper range", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));


	btn = CreateButtonInstance("shotgunAmmoBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.15, 0.3, 0));
	btn->SetText(new Text("shotgun ammo", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("shotgunDmgBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.15, -0.1, 0));
	btn->SetText(new Text("shotgun dmg", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("shotgunNumBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.15, -0.5, 0));
	btn->SetText(new Text("shotgun #shot", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("pursuerAmmoBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.40, 0.3, 0));
	btn->SetText(new Text("pursuer ammo", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("pursuerROFBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.40, -0.1, 0));
	btn->SetText(new Text("pursuer ROF", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("NaniteStackBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.40, -0.5, 0));
	btn->SetText(new Text("nanite stack", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("naniteSwarmDmgBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.65, 0.3, 0));
	btn->SetText(new Text("naniteS Dmg", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("naniteSwarmDurBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.65, -0.1, 0));
	btn->SetText(new Text("naniteS Dur", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("naniteSwarmRadiusBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.65, -0.5, 0));
	btn->SetText(new Text("naniteS radius", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("emgWarpAmmoBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.9, 0.3, 0));
	btn->SetText(new Text("emgWarp Ammo", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("chronoSurgeAmmoBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.9, -0.1, 0));
	btn->SetText(new Text("chronoSurge Ammo", glm::vec2(-1.0, -1.7), 0.2, glm::vec3(1.0, 1.0, 1.0)));
	//-------------------
	btn = CreateButtonInstance("barrierDurBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.85, -0.9, 0));
	btn->SetText(new Text("Barrier Dur", glm::vec2(-1.0, 0), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("barrierSizeBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.60, -0.9, 0));
	btn->SetText(new Text("Barrier Size", glm::vec2(-1.0, 0), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("batteryOverChargeBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.35, -0.9, 0));
	btn->SetText(new Text("Barrier OChrg", glm::vec2(-1.0, 0), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("chronoSurgeBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(-0.1, -0.9, 0));
	btn->SetText(new Text("Chrono Surge", glm::vec2(-1.0, 0), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("emergencyWarpBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.15, -0.9, 0));
	btn->SetText(new Text("Emg Warp", glm::vec2(-1.0, 0), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("emergencyManuBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.4, -0.9, 0));
	btn->SetText(new Text("Emg Manu", glm::vec2(-1.0, 0), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	btn = CreateButtonInstance("RepairBtn", "FlatSurface", "ScreenMaterial", SHOP_MENU, "pauseButton");
	btn->SetScale(glm::vec3(0.20, 0.20, 1));//multiply by 17.72crosshairDefaultTexture
	btn->SetPosition(glm::vec3(0.65, -0.9, 0));
	btn->SetText(new Text("Repair", glm::vec2(-1.0, 0), 0.2, glm::vec3(1.0, 1.0, 1.0)));

	//shopButton
	//startButton
	//bounty1Button
	//bounty2Button
	//bounty3Button
	//shipHealthButton
	//shipShieldButton
	//shipSpeedButton

	//laserRangeBtn
	//laserDmgBtn
	//laserPierceBtn

	//NaniteAmmoBtn
	//NaniteDmgBtn
	//NaniteDurBtn
	//NaniteStackBtn

	//chargeAmmoBtn
	//chargeDmgBtn
	//chargeSizeBtn

	//sniperAmmoBtn
	//sniperDmgBtn
	//sniperRangeBtn

	//shotgunAmmoBtn
	//naniteSwarmDurBtn
	//shotgunNumBtn

	//pursuerAmmoBtn
	//pursuerROFBtn

	//naniteSwarmDmgBtn
	//pursuerROFBtn
	//naniteSwarmRadiusBtn


	//emgWarpAmmoBtn
	//chronoSurgeAmmoBtn

	//barrierDurBtn
	//barrierSizeBtn
	//batteryOverChargeBtn
	//chronoSurgeBtn
	//emergencyWarpBtn
	//emergencyManuBtn

}


ParticleNode* Game::CreateParticleInstance(int count, std::string particle_name, std::string object_name, std::string material_name, std::string texture_name) {
	//create resource
	NodeResources* rsc = GetResources(object_name, material_name, texture_name, "");

	// Create asteroid instance
	ParticleNode *pn = new ParticleNode(particle_name, rsc->geom, rsc->mat, rsc->tex);
	return pn;
}
Boss *Game::CreateBossInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name) {
	NodeResources* rsc = GetResources(object_name, material_name, normal_name, "");
	// Create asteroid instance
	NodeResources* proj = GetResources("SimpleCylinderMesh", "ColoredMaterial", "", "");
	Boss *en = new Boss(entity_name, rsc->geom, rsc->mat, rsc->tex, rsc->norm);

	en->SetNodeResources(rsc);

	en->SetProjRsc(proj);
	scene_.AddBoss(en);
	en->SetScale(glm::vec3(3));
	en->SetPlayer(scene_.GetPlayer());
	return en;
}

Enemy *Game::CreateEnemyInstance(std::string entity_name, std::string object_name, std::string material_name, std::string normal_name) {
	NodeResources* rsc = GetResources(object_name, material_name, normal_name,"");
	// Create asteroid instance
	NodeResources* proj = GetResources("SimpleCylinderMesh", "ColoredMaterial", "", "");
	Enemy *en = new Enemy(entity_name, rsc->geom, rsc->mat, rsc->tex, rsc->norm);

	en->SetNodeResources(rsc);

	en->SetProjRsc(proj);
	scene_.AddEnemy(en);
	en->SetScale(glm::vec3(3));
	en->SetPlayer(scene_.GetPlayer());
	return en;
}

ScreenNode *Game::CreateScreenInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name, std::string normal_name) {
	NodeResources* rsc = GetResources(object_name, material_name, texture_name, normal_name);

	if (entity_name == "radar") {
		RadarNode* scn = new RadarNode(entity_name, rsc->geom, rsc->mat, rsc->tex, rsc->norm);
		scene_.AddRadar(scn);
		return scn;
	}
	else {
		ScreenNode *scn = new ScreenNode(entity_name, rsc->geom, rsc->mat, rsc->tex, rsc->norm);
		scene_.AddScreen(scn, type);
		return scn;
	}

}

AsteroidNode *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, std::string normal_name) {

	NodeResources* rsc = GetResources(object_name, material_name, texture_name, normal_name);
	AsteroidNode *ast = new AsteroidNode(entity_name, rsc->geom, rsc->mat, rsc->tex, rsc->norm);

	json saveData = resman_.GetResource("save")->GetJSON();
	std::map<std::string, int> drops;
	for (auto& gameData : saveData["obstacle_stats"]["asteroid_drops"].items()) {
		drops[gameData.key()] = gameData.value();
	}
	ast->SetDrops(drops);
	scene_.AddAsteroid(ast);
	return ast;
}
CometNode *Game::CreateCometNode(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, std::string normal_name) {

	NodeResources* rsc = GetResources(object_name, material_name, texture_name, normal_name);
	CometNode *ast = new CometNode(entity_name, rsc->geom, rsc->mat, rsc->tex, rsc->norm);
	json saveData = resman_.GetResource("save")->GetJSON();
	std::map<std::string, int> drops;
	for (auto& gameData : saveData["obstacle_stats"]["comet_drops"].items()) {
		drops[gameData.key()] = gameData.value();
	}
	ast->SetDrops(drops);
	scene_.AddComet(ast);
	return ast;
}

ButtonNode *Game::CreateButtonInstance(std::string entity_name, std::string object_name, std::string material_name, ScreenType type, std::string texture_name, std::string normal_name) {

	NodeResources* rsc = GetResources(object_name, material_name, texture_name, normal_name);

	ButtonNode *btn = new ButtonNode(entity_name, rsc->geom, rsc->mat, rsc->tex, rsc->norm);
	btn->SetTextObj(&text);
	scene_.AddButton(btn, type);
	return btn;

}

SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, NodeType type, std::string texture_name, std::string normal_name){
	NodeResources* rsc = GetResources(object_name, material_name, texture_name, normal_name);

    SceneNode *scn = scene_.CreateNode(entity_name, rsc->geom, rsc->mat, type, rsc->tex, rsc->norm);
    return scn;
}

NodeResources* Game::GetResources(std::string object_name, std::string material_name, std::string texture_name, std::string normal_name) {
	NodeResources* rsc = new NodeResources();
	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}
	else {
		rsc->geom = geom;
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}
	else {
		rsc->mat = mat;
	}

	Resource *tex = NULL;
	if (texture_name != "") {
		tex = resman_.GetResource(texture_name);
		if (!tex) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}
		else {
			rsc->tex = tex;
		}
	}

	Resource *norm = NULL;
	if (normal_name != "") {
		norm = resman_.GetResource(normal_name);
		if (!tex) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}
		else {
			rsc->norm = norm;
		}
	}
	return rsc;

}

} // namespace game
