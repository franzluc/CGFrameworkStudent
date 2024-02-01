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
    
    // Se establecen los parametros de la camara. Establecemos por defecto la cámara cómo perspectiva.
	camara.SetPerspective(fov, framebuffer.width/framebuffer.height , near_plane, far_plane);
    
	// Web para entender las coordenadas de las camaras--> https://learnwebgl.brown37.net/07_cameras/camera_lookat/camera_lookat.html
	camara.LookAt(eye, center, Vector3::DOWN); //Vector3::DOWN = {0, -1, 0}
    
}

// Render one frame
void Application::Render(void)
{
	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{	
	// Fran, si te va lento, deja solo un entity girando
    entity0.Render(&framebuffer, &camara, Color::BLACK);
    entity0.Update(seconds_elapsed, 3);
    entity0.Render(&framebuffer, &camara, Color::GREEN);
    
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
        
        framebuffer.Fill(Color::BLACK);
        
        mesh0.LoadOBJ("meshes/lee.obj");
        entity0 = Entity(mesh0);
        entity0.matrixModel.Scale(1.35, 1.35, 1.35);
        entity0.matrixModel.Translate(0, -0.4, 0);
        entity0.Render(&framebuffer, &camara, Color::GREEN);
        
    }

	if (event.keysym.sym == SDLK_2) {
        
        
        framebuffer.Fill(Color::BLACK);
            
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
    
    if (event.keysym.sym == SDLK_o) {
        
        framebuffer.Fill(Color::BLACK);
        
        lastMode = 2;
        camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
        
	}
    
	if (event.keysym.sym == SDLK_p) {
        
        framebuffer.Fill(Color::BLACK);
        
        lastMode = 1;
        camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
	}

	if (event.keysym.sym == SDLK_n) {
        lastFigure = 1;
	}
    
    if (event.keysym.sym == SDLK_f){
        lastFigure = 2;
    }
    
    if (event.keysym.sym == SDLK_v){
        lastFigure = 3;
    }
    
    if (event.keysym.sym == SDLK_PLUS) {
        
        framebuffer.Fill(Color::BLACK);
        
        if(lastFigure == 1){
            near_plane = near_plane + 0.1;
            if (lastMode == 2){
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){
                camara.SetPerspective(fov,framebuffer.width/framebuffer.height, near_plane, far_plane);
            
           }
        }
        
        if(lastFigure == 2){
            far_plane++;
            
            if (lastMode == 2){
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
           }
        }
        
        if(lastFigure == 3){
            fov = fov+0.25;
            if (lastMode == 1){
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
            }
        }
    }
    
    if (event.keysym.sym == SDLK_MINUS) {
        
        framebuffer.Fill(Color::BLACK);
        
        if(lastFigure == 1){
            near_plane = near_plane - 0.1;
            if (lastMode == 2){
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
            }
        }
        
        if(lastFigure == 2){
            far_plane = far_plane - 1;
            if (lastMode == 2){
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
           }
        }
        
        if(lastFigure == 3){
            fov = fov-0.25;
            camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
        }
    }
}



void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        
        framebuffer.Fill(Color::BLACK);
        
        eye.x = mouse_position.x;
        eye.y = - mouse_position.y;
        camara.LookAt(eye, center, Vector3::DOWN);
    }
    
    if (event.button == SDL_BUTTON_RIGHT) {
        
        framebuffer.Fill(Color::BLACK);
        
        center.x = mouse_position.x;
        center.y = mouse_position.y;
        camara.LookAt(eye, center, Vector3::DOWN);
        
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
