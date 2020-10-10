/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 2: A multi-component model
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates camera model with the tori.
 * For further info, please read the README file provided.
 *
 */

#ifndef HUD_OBJ_H_
#define HUD_OBJ_H_
#include <gameobj.h>

namespace game {

	// Class for a single game object 
	class UIObj : public GameObj {

	public:
		// Create game object
		UIObj(GLuint shader, Transformation* m);
		UIObj();

		// Destructor
		~UIObj();

		
		void LoadTexture(const char* texImagePath);


		void Draw(Camera *camera);
		void Update(double deltaTime);

	private:
		int CreateSquare(void);
		void setthisTexture(GLuint w, char *fname);
		void setallTexture(void);
	}; // class GameObj

} // namespace game

#endif // GAME_OBJ_H_
