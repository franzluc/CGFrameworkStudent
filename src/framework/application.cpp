#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include <cmath>

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Image imatge;
	bool exitCarrega = imatge.LoadPNG("res/images/toolbar.png", false);

	SDL_RenderClear(renderer);
	imatge.Render();
}

// Render one frame
void Application::Render(void)
{
	// ...

	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app	
	}

	if (event.keysym.sym == SDLK_1) { //Si es pressiona 1, es dibuixa una l�nia	
		framebuffer.DrawLineDDA(initPosX, initPosY, endPosX, endPosY, Color::WHITE);
	}

	if (event.keysym.sym == SDLK_2) { //Si es pressiona 2, es dibuixa un rectangle
		width = endPosX - initPosX;
		height = endPosY - initPosY;
		framebuffer.DrawRect(initPosX, initPosY, width, height, Color::WHITE, borderWidth, true, Color::BLUE);
	}
    
    if (event.keysym.sym == SDLK_3) { // Si se presiona 3, se dibuja un circulo.
        if (!drawingInCourse){
            initPosX = mouse_position.x;
            initPosY = mouse_position.y;
            drawingInCourse = true;
        } else {
            endPosX = mouse_position.x;
            endPosY = mouse_position.y;
            int radio = sqrt((endPosX-initPosX)*(endPosX-initPosX)+(endPosY-initPosY)*(endPosY-initPosY));
            framebuffer.DrawCircle(initPosX, initPosY, radio, Color::WHITE, borderWidth, true, Color::BLUE);
            drawingInCourse = false;
        }
    }
    
	if (event.keysym.sym == SDLK_PLUS && borderWidth <= 20) { //Si es pressiona "+", s'incrementa l'amplada del borde
		framebuffer.DrawRect(initPosX, initPosY, width, height, Color::BLACK, borderWidth, true, Color::BLACK);
		borderWidth++;
		framebuffer.DrawRect(initPosX, initPosY, width, height, Color::WHITE, borderWidth, true, Color::BLUE);
	}

	if (event.keysym.sym == SDLK_MINUS && borderWidth >= 1) { //Si es pressiona "-", es redueix l'amplada del borde
		framebuffer.DrawRect(initPosX, initPosY, width, height, Color::BLACK, borderWidth, true, Color::BLACK);
		borderWidth--;
		framebuffer.DrawRect(initPosX, initPosY, width, height, Color::WHITE, borderWidth, true, Color::BLUE);
	}

	if (event.keysym.sym == SDLK_3) {
		framebuffer.DrawTriangle(vector0, vector1, vector2, Color::WHITE, false, Color::BLUE);
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		// Deteccio de coordenades per la linia i el rectangle
		if (!drawingInCourse) {
			initPosX = mouse_position.x;
			initPosY = mouse_position.y;
			drawingInCourse = true;
		}
		else {
			endPosX = mouse_position.x;
			endPosY = mouse_position.y;
			drawingInCourse = false;
		}

		// Deteccio de coordenades per el triangle
		if (triangleChecker == 0) {
			vector0 = mouse_position;
			triangleChecker = 1;
		}
		else if (triangleChecker == 1) {
			vector1 = mouse_position;
			triangleChecker = 2;
		}
		else if (triangleChecker == 2) {
			vector2 = mouse_position;
			triangleChecker = 0;
		}
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
