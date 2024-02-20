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
    
    zetaBuffer.Resize(this->window_width, this->window_height);
    zetaBuffer.Fill(FLT_MAX);
    
    // Entidad no animada, que igualmente responde a los cambios en las perspectivas
    mesh0.LoadOBJ("meshes/lee.obj");
    textura.LoadTGA("textures/lee_color_specular.tga");
    entity0 = Entity(mesh0, textura);
    entity0.matrixModel.Scale(1.35, 1.35, 1.35);
    entity0.matrixModel.Translate(0, -0.4, 0);
    entity0.Render(&framebuffer, &camara, &zetaBuffer);
    
}

// Render one frame
void Application::Render(void)
{
    framebuffer.Fill(Color::BLACK);
    zetaBuffer.Fill(FLT_MAX);
    entity0.Render(&framebuffer, &camara, &zetaBuffer);
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
    
    if (event.keysym.sym == SDLK_o) { // Cambio a proyección ortográfica
        // Establecemos la siguiente variable para que los valores near_plane y far_plane se actualicen en la proyección adecuada
        lastMode = 2;
        
        // Realizamos el cambio
        camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
        
	}
    
	if (event.keysym.sym == SDLK_p) { // Cambio a perspectiva
        
        lastMode = 1; // Volvemos a utilizar la variable para marcar en qué proyección estamos
        
        // Realizamos el cambio
        camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
	}

	if (event.keysym.sym == SDLK_n) { // Modificar near_plane
        //Utilizamos la siguiente varaible para saber qué valor se cambia con el PLUS y el MINUS (near_plane, far_plane o fov)
        lastFigure = 1;
	}
    
    if (event.keysym.sym == SDLK_c) { // Cambia entre plain color y interpolated
        
    }

    if (event.keysym.sym == SDLK_z) { // Cambia entre oclusion y no oclusion

    }

    if (event.keysym.sym == SDLK_t) { // Cambia entre mesh texture y plain color

    }
    
    if (event.keysym.sym == SDLK_PLUS) { // Aumentamos el valor dado
        
        if(lastFigure == 1){ // Si es near_plane:
            near_plane = near_plane + 0.1; // Aumentamos poco a poco el near plane para que se vea cómo va desapareciendo la entidad
            
            if (lastMode == 2){ // Actualizamos si es ortográfica
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){ // Actualizamos si es perspectiva
                camara.SetPerspective(fov,framebuffer.width/framebuffer.height, near_plane, far_plane);
            
           }
        }
        
        if(lastFigure == 2){ // Si es far plane:
            
            // Ya que el valor del far plane por defecto es 100, si aumentamos no ocurrirá nada. Si lo disminuimos primero,
            // hasta donde ya no se ve el objeto, despues podemos aumentarlo para que se vuelva a ver.
            
            far_plane++;
            
            if (lastMode == 2){ // Actualizamos si es ortográfica
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){ // Actualizamos si es perspectiva
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
           }
        }
        
        if(lastFigure == 3){ // Si es field of view:
            
            if (lastMode == 1){
                fov = fov + 0.25; // El fov sólo se actualiza en la perspectiva
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
            }
        }
    }
    
    if (event.keysym.sym == SDLK_MINUS) { // Realizamos lo mismo pero disminuyendo
        
        if(lastFigure == 1){ // Si disminuimos el near plane sin haberlo aumentado, tampoco ocurrirá nada.
            
            near_plane = near_plane - 0.1;
            
            if (lastMode == 2){
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
            }
        }
        
        if(lastFigure == 2){ // Para que se vea el cambio, hay que disminuirlo mucho.
            
            far_plane = far_plane - 1;
            
            if (lastMode == 2){
                camara.SetOrthographic(left, right, top, bottom, near_plane, far_plane);
            }
            if (lastMode == 1){
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
           }
        }
        
        if(lastFigure == 3){ // FOV
            
            if (lastMode == 1){
                fov = fov - 0.25; // El fov sólo se actualiza en la perspectiva
                camara.SetPerspective(fov, framebuffer.width/framebuffer.height, near_plane, far_plane);
            }
        }
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
