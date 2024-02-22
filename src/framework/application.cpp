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
    shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
    //texture = Texture::Get("image/fruits.png");
    quad.CreateQuad();
    
    u_aspect_ratio = window_width/window_height;
    ex = 1;
    prob = 4;
    
	
}

// Render one frame
void Application::Render(void)
{
    
    shader->Enable();
    
    shader->SetFloat("u_aspect_ratio", u_aspect_ratio);
    shader->SetUniform1("ex", ex);
    shader->SetUniform1("prob", prob);
    
    quad.Render();
    shader->Disable();
    //shader->SetTexture("u_texture", texture);
    
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
    
    if (event.keysym.sym == SDLK_o) { // Cambio a proyección ortográfica
        
        
	}
    
	if (event.keysym.sym == SDLK_p) { // Cambio a perspectiva
        
        
	}

	if (event.keysym.sym == SDLK_n) { // Modificar near_plane
       
	}
    
    if (event.keysym.sym == SDLK_f){ // Modificar far_plane con + y -
       
    }
    
    if (event.keysym.sym == SDLK_v){ // Modificar fov con + y -
       
    }

    if (event.keysym.sym == SDLK_c) { // Cambia entre plain color y interpolated
       
    }
    
    if (event.keysym.sym == SDLK_z) { // Cambia entre oclusión y no oclusión
       
    }
    
    if (event.keysym.sym == SDLK_t) { // Cambia entre textura y plain color
        
    }
    
    if (event.keysym.sym == SDLK_PLUS) { // Aumentamos el valor dado
        
        
    }
    
    if (event.keysym.sym == SDLK_MINUS) { // Realizamos lo mismo pero disminuyendo
        
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) { 
        moveCam = true;
    }
    
    if (event.button == SDL_BUTTON_RIGHT) {
        moveCam2 = true;
     }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        moveCam = false;
    }

    if (event.button == SDL_BUTTON_RIGHT) {
        moveCam2 = false;
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{       
    if (moveCam) {
        // Para orbitar alrededor del objeto:  
        
        // Utilizamos la función de camara "rotate", utilizando la variación de posición del mouse (mouse_delta) en cada uno de los ejes. Multiplicamos por DEG2RAD para pasar de grados a radianes
        camara.Rotate(mouse_delta.x * DEG2RAD, Vector3(0, 1, 0)); // Si se mueve el mouse en el eje x, gira la camara en el eje y
        camara.Rotate(mouse_delta.y * DEG2RAD, Vector3(1, 0, 0)); // Viceversa
    }

    if (moveCam2) {
        
        // Utilizamos la función de camara 'move', nuevamente usando mouse_delta, en la que cambiamos la posición del centro de enfoque
        camara.Move(Vector3(mouse_delta.x * DEG2RAD, mouse_delta.y * DEG2RAD, 0));
    }
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
