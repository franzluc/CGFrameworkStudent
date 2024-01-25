#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include <cmath>
<<<<<<< Updated upstream
#include <chrono>
#include <thread>
=======
#include "button.h"
>>>>>>> Stashed changes

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

	toolbarButton.DrawButton(framebuffer);
	clearButton.DrawButton(framebuffer);
	loadButton.DrawButton(framebuffer);
	saveButton.DrawButton(framebuffer);
	eraserButton.DrawButton(framebuffer);
	lineButton.DrawButton(framebuffer);
	rectangleButton.DrawButton(framebuffer);
	circleButton.DrawButton(framebuffer);
	triangleButton.DrawButton(framebuffer);
	blackButton.DrawButton(framebuffer);
	whiteButton.DrawButton(framebuffer);
	pinkButton.DrawButton(framebuffer);
	yellowButton.DrawButton(framebuffer);
	redButton.DrawButton(framebuffer);
	blueButton.DrawButton(framebuffer);
	cyanButton.DrawButton(framebuffer);
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

	if (event.keysym.sym == SDLK_1) { //Si se presiona 1 se dibuja una linea
		lastFigure = 1;
	}

	if (event.keysym.sym == SDLK_2) { //Si se presiona 2 se dibuja un rectangulo
		lastFigure = 2;
	}
    
    if (event.keysym.sym == SDLK_3) { //Si se presiona 3 se dibuja un circulo
		lastFigure = 3;
    }
    
	if (event.keysym.sym == SDLK_PLUS && borderWidth <= 20) { //Si se presiona + se incrementa el borde
		if (lastFigure == 2) {
			framebuffer.DrawRect(initPosX, initPosY, width, height, Color::BLACK, borderWidth, true, Color::BLACK);
			borderWidth++;
			framebuffer.DrawRect(initPosX, initPosY, width, height, Color::WHITE, borderWidth, true, Color::BLUE);
		}
		else if (lastFigure == 3) {
			framebuffer.DrawCircle(initPosX, initPosY, radio, Color::BLACK, borderWidth, true, Color::BLACK);
			borderWidth++;
			framebuffer.DrawCircle(initPosX, initPosY, radio, Color::WHITE, borderWidth, true, Color::BLUE);
		}
		
	}

	if (event.keysym.sym == SDLK_MINUS && borderWidth >= 1) { //Si se presiona - se incrementa el borde
		if (lastFigure == 2) {
			framebuffer.DrawRect(initPosX, initPosY, width, height, Color::BLACK, borderWidth, true, Color::BLACK);
			borderWidth--;
			framebuffer.DrawRect(initPosX, initPosY, width, height, Color::WHITE, borderWidth, true, Color::BLUE);
		}
		else if (lastFigure == 3) {
			framebuffer.DrawCircle(initPosX, initPosY, radio, Color::BLACK, borderWidth, true, Color::BLACK);
			borderWidth--;
			framebuffer.DrawCircle(initPosX, initPosY, radio, Color::WHITE, borderWidth, true, Color::BLUE);
		}
	}

	if (event.keysym.sym == SDLK_4) {     
		lastFigure = 4;
	}
    
    if (event.keysym.sym == SDLK_6){
        float total_time = 10000;
        float current_time = 0;
        float dt = 0.05;
        
        ParticleSystem ps;
        ps.Init();
        ps.Render(&framebuffer);
        
        while (current_time < total_time){
            ps.Update(dt);
            ps.Render(&framebuffer);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            current_time = current_time + dt;
        }
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		
		// Deteccion de coordenadas para la linea, el rectangulo y el circulo
		if (!drawingInCourse && modeSelected) {
			initPosX = mouse_position.x;
			initPosY = mouse_position.y;
			drawingInCourse = true;
		}
		else {
			endPosX = mouse_position.x;
			endPosY = mouse_position.y;
			drawingInCourse = false;
			if (lastFigure == 1) {
				framebuffer.DrawLineDDA(initPosX, initPosY, endPosX, endPosY, Color::WHITE);
			}
			else if (lastFigure == 2) {
				width = endPosX - initPosX;
				height = endPosY - initPosY;
				framebuffer.DrawRect(initPosX, initPosY, width, height, Color::WHITE, borderWidth, false, Color::BLUE);
			}
			else if (lastFigure == 3) {
				radio = sqrt((endPosX - initPosX) * (endPosX - initPosX) + (endPosY - initPosY) * (endPosY - initPosY));
				framebuffer.DrawCircle(initPosX, initPosY, radio, Color::WHITE, borderWidth, false, Color::BLUE);
			}
		}

		// Deteccion del coordenadas para el triangulo
		if (triangleChecker == 0 && modeSelected) {
			vector0 = mouse_position;
			triangleChecker = 1;
		}
		else if (triangleChecker == 1 && modeSelected) {
			vector1 = mouse_position;
			triangleChecker = 2;
		}
		else if (triangleChecker == 2 && modeSelected) {
			vector2 = mouse_position;
			triangleChecker = 0;
			if (lastFigure == 4) {
				framebuffer.DrawTriangle(vector0, vector1, vector2, Color::WHITE, false, Color::BLUE);
			}
		}

		if (lineButton.isMouseInside(mouse_position) == true) {
			lastFigure = 1;
			modeSelected = true;
		} else if (rectangleButton.isMouseInside(mouse_position) == true){
			lastFigure = 2;
			modeSelected = true;
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