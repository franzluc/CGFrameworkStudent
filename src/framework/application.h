/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"

class Application
{
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	bool drawingInCourse = false; // Checks if there is a drawing in course
	Vector2 mouse_delta; // Mouse movement in the last frame
	// Variables per guardar les coordenades del cursor utilitzades per dibuixar linies i rectangles
	float initPosX;
	float initPosY;
	float endPosX;
	float endPosY;
	float width;
	float height;
	// Variable per el radi del cercle
	int radio;
	// Variable per controlar el proces de dibuix del rectangle
	int triangleChecker = 0;
	// Vectors que guardaran les posicions del cursor per dibuixar triangles
	Vector2 vector0;
	Vector2 vector1;
	Vector2 vector2;
	// Boolea per comprobar que l'imatge s'ha carregar correctament
	bool exitCarrega;
	// Int per saber la ultima figura dibuixada (per tal de poder ampliar el border del rectangle o el cercle
	int lastFigure;
		// 1 --> linia
		// 2 --> rectangle
		// 3 --> cercle
		// 4 --> triangle

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}

private:
	int borderWidth = 4;
};
