#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "camera.h"
#include "entity.h"
#include "mesh.h"
#include "camera.h"
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

	// Se establecen los parametros de la camara
	camara.SetPerspective(13, framebuffer.width / framebuffer.height, 0.01, 100);
	// Web para entender las coordenadas de las camaras--> https://learnwebgl.brown37.net/07_cameras/camera_lookat/camera_lookat.html
	camara.LookAt({ 0, -0.2, 5 }, { 0, 0, 0 }, Vector3::DOWN); //Vector3::DOWN = {0, -1, 0}

	// Entidad 1
	mesh1.LoadOBJ("meshes/lee.obj");
	entity1 = Entity(mesh1);
	entity1.matrixModel.Scale(1.35, 1.35, 1.35);
	entity1.matrixModel.Translate(0, -0.4, 0);
	entity1.Render(&framebuffer, &camara, Color::WHITE);

	// Entidad 2
	mesh2.LoadOBJ("meshes/anna.obj");
	entity2 = Entity(mesh2);
	entity2.matrixModel.Translate(-0.55, 0.35, 0);

	// Entidad 3
	mesh3.LoadOBJ("meshes/cleo.obj");
	entity3 = Entity(mesh3);
	entity3.matrixModel.Scale(1.6, 1.6, 1.6);
	entity3.matrixModel.Translate(0.75, 0, 0);
	entity3.matrixModel.Rotate(0.40, { -1, 1, 0 });
	entity3.Render(&framebuffer, &camara, Color::BLUE);

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
	// Fran, si te va lento, deja solo un entity girando
	entity1.Render(&framebuffer, &camara, Color::BLACK);
	entity1.Update(seconds_elapsed, 0);
	entity1.Render(&framebuffer, &camara, Color::WHITE);
	entity2.Render(&framebuffer, &camara, Color::BLACK);
	entity2.Update(seconds_elapsed, 0);
	entity2.Render(&framebuffer, &camara, Color::RED);
	entity3.Render(&framebuffer, &camara, Color::BLACK);
	entity3.Update(seconds_elapsed, 0);
	entity3.Render(&framebuffer, &camara, Color::BLUE);
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
