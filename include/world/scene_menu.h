#ifndef SCENE_MENU_H_
#define SCENE_MENU_H_

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
#include <map>
namespace game {

	

	// Class that manages all the objects in a scene
	class SceneMenu {

	public:
		// Constructor and destructor
		SceneMenu();
		~SceneMenu();

		// Draw the entire scene
		void Draw(Camera *camera);

		// Update entire scene
		void Update(float deltaTime);

	private:
		// Background color
		
	}; // class SceneGraph

} // namespace game

#endif // SCENE_GRAPH_H_
