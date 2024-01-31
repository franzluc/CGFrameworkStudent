/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "camera.h"
#include "mesh.h"
#include "entity.h"

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
	// Variables para guardar las coordenadas para dibujar lineas, rectangulos y circulos
	float initPosX = 0;
	float initPosY = 0;
	float endPosX = 0;
	float endPosY = 0;
	float width = 0;
	float height = 0;
	// Variable per guardar el valor de border
	int borderWidth = 4;
	// Variable para guardar el radio del circulo
	int radio;
	// Variable para controlar el proceso de dibujo del rectangulo
	// Si el valor de la variable es diferente a 0, 1 o 2, significa
	// que no se esta dibujando el triangulo, sino otra figura
	int triangleChecker = 3;
	// Vectores que guardan la posicion del cursor
	Vector2 vector0;
	Vector2 vector1;
	Vector2 vector2;
	// Booleano para comprobar que se ha cargado exitosamente la imagen
	bool exitCarrega;
	// Int para saber la ultima figura dibujada
	int lastFigure;
		// 1 --> linea
		// 2 --> rectangulo
		// 3 --> circulo
		// 4 --> triangulo
		// otro --> se han eliminado los dibujos

	// Booleano que indica si se ha seleccionado un boton de la barra inferior
	bool modeSelected = false;

	// Variable para el color del relleno y de borde
	Color colorFill = Color::WHITE;
	Color borderFill = Color::WHITE;

	// Variable para comprobar si la siguiente figura tiene que ser filleada de color
	bool isFigureFilled = false;

	// Variables para permitir el dibujo de lineas con el cursor
	bool cursorPaintTool = false;
	bool isDrawing = false;

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
    
    
};


