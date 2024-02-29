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
    // Creamos las variables que posteriormente pasaremos al fragment shader (el u_aspect_ratio no nos funciona bien)
    
    u_aspect_ratio = window_width/window_height;
    
    // Estas variables funcionan para el control de los apartados y los ejercicios
    //ex = 2;
    //prob = 1;
    
    mesh0.LoadOBJ("meshes/lee.obj"); // Cargamos la malla
    
    material0.shader = Shader::Get("shaders/phong.vs", "shaders/phong.fs");
    material0.textura = Texture::Get("textures/lee_color_specular.tga");
    material0.nText = Texture::Get("textures/lee_normal.tga");
    material0.Ka = {1.0, 1.0, 1.0};
    material0.Kd = {0.8, 0.7, 0.2};
    material0.Ks = {1.0, 1.0, 1.0};
    material0.Shininess = 10;
    
    entity0 = Entity(mesh0, material0);
    
    // Configuramos la camara
    camara.SetPerspective(fov, framebuffer.width / framebuffer.height, near_plane, far_plane);
    camara.LookAt(eye, center, Vector3::UP);
    
    lights.posicion = {1.0, 2.0, 0.0};
    
    lights.intensidadId = {1.0, 1.0, 1.0};
    lights.intensidadIs = {1.0, 1.0, 1.0};
    ambientIntensity = {1.0, 1.0, 1.0};
    
    // Habilitamos el test de profundidad 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    sUniform.ex = 1;
    sUniform.modelMatrix = entity0.matrixModel;
    sUniform.camara = &camara;
    sUniform.lightPosition = lights.posicion;
    sUniform.Ia = ambientIntensity;
    sUniform.Id = lights.intensidadId;
    sUniform.Is = lights.intensidadIs;
    sUniform.Ka = material0.Ka;
    sUniform.Kd = material0.Kd;
    sUniform.Ks = material0.Ks;
    sUniform.brillo = material0.Shininess;

}

// Render one frame
void Application::Render(void)
{
   entity0.Render(sUniform);
  
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
        //material0.shader = Shader::Get("shaders/phong2.vs", "shaders/phong2.fs");
        
        sUniform.ex = 1;
        Render();

    }

	if (event.keysym.sym == SDLK_2) {
        //material0.shader = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
        //Render();
        sUniform.ex = 2;
        Render();
        
        
	}
    
    if (event.keysym.sym == SDLK_3) {
        sUniform.ex = 3;
        Render();
        
    }
    
    if (event.keysym.sym == SDLK_4) {
        sUniform.ex = 4;
        Render();
    }
    
    if (event.keysym.sym == SDLK_5) {
        sUniform.ex = 5;
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
