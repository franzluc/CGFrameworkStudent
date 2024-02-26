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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
    texture = Texture::Get("images/fruits.png");
    
    quad.CreateQuad();
    
    u_aspect_ratio = window_width/window_height;
    ex = 1;
    prob = 1;

    mesh0.LoadOBJ("meshes/lee.obj"); // Cargamos la malla
    entity0 = Entity(mesh0);
    
    entity0.shaderEntity = Shader::Get("shaders/simple.vs", "shaders/simple2.fs");
    entity0.entityTexture = Texture::Get("textures/lee_color_specular.tga");
    
    
    camara.SetPerspective(fov, framebuffer.width / framebuffer.height, near_plane, far_plane);
    camara.LookAt(eye, center, Vector3::UP);

}

// Render one frame
void Application::Render(void)
{
    if (ex != 4){
        shader->Enable();
        
        shader->SetFloat("u_aspect_ratio", u_aspect_ratio);
        shader->SetFloat("u_width", texture->width);
        shader->SetFloat("u_heigth", texture->height);
        shader->SetFloat("time", time);
        shader->SetUniform1("ex", ex);
        shader->SetUniform1("prob", prob);
        shader->SetTexture("u_texture", texture);
        
        quad.Render();
        
        shader->Disable();
   }
   if (ex == 4){
        entity0.Render(&camara);
   }
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
        ex = 1;
        prob = 1;
        Render();

    }

	if (event.keysym.sym == SDLK_2) {
        ex = 2;
        prob = 0;
        Render();
        
	}
    
    if (event.keysym.sym == SDLK_3) {
        ex = 3;
        prob = 1;
        Render();
        
    }
    
    if (event.keysym.sym == SDLK_4) {
        ex = 4;
        prob = 1;
        entity0.Render(&camara);
        
        
    }
    
    if (event.keysym.sym == SDLK_a) {
        prob = 1;
        Render();
        
    }
    
    if (event.keysym.sym == SDLK_b) {
        prob = 2;
        Render();
        
    }
    
    if (event.keysym.sym == SDLK_c) {
        prob = 3;
        Render();
        
    }
    
    if (event.keysym.sym == SDLK_d) {
        prob = 4;
        Render();
        
    }
    
    if (event.keysym.sym == SDLK_e) {
        prob = 5;
        Render();
        
    }
    
    if (event.keysym.sym == SDLK_f) {
        prob = 6;
        Render();
        
    }
    
    
    if (event.keysym.sym == SDLK_o) { // Cambio a proyección ortográfica
        
        
	}
    
	if (event.keysym.sym == SDLK_p) { // Cambio a perspectiva
        
        
	}

	if (event.keysym.sym == SDLK_n) { // Modificar near_plane
       
	}
    
    
    
    if (event.keysym.sym == SDLK_v){ // Modificar fov con + y -
       
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
