#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "camera.h"
#include "entity.h"
#include "mesh.h"
#include <cmath>
#include <chrono>
#include <thread>


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
    
	Mesh holaa = Mesh();
	holaa.LoadOBJ("meshes/lee.obj");
	Entity hola = Entity(holaa);	
	Camera camara = Camera();
	camara.SetPerspective(45, framebuffer.width / framebuffer.height, 0.01, 100);
	/* Web para entender las coordenadas de las camaras--> https://learnwebgl.brown37.net/07_cameras/camera_lookat/camera_lookat.html */
	camara.LookAt({ 0, 0, 1.2 }, { 0, 0, 0 }, Vector3::DOWN); //Vector3::DOWN = {0, -1, 0}
	hola.Render(&framebuffer, &camara, Color::WHITE);

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

	if (event.keysym.sym == SDLK_1) {
	}

	if (event.keysym.sym == SDLK_2) {
	}
    
    if (event.keysym.sym == SDLK_3) {
	}
    
	if (event.keysym.sym == SDLK_PLUS && borderWidth <= 20) {
	
	}

	if (event.keysym.sym == SDLK_MINUS && borderWidth >= 1) {
	}

	if (event.keysym.sym == SDLK_4) {     
	}

	if (event.keysym.sym == SDLK_5) {
	}
    
    if (event.keysym.sym == SDLK_6){
    }

	if (event.keysym.sym == SDLK_f) {
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
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

}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
