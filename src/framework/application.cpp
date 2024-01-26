#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
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
	greenButton.DrawButton(framebuffer);
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
		triangleChecker = 3;
	}

	if (event.keysym.sym == SDLK_2) { //Si se presiona 2 se dibuja un rectangulo
		lastFigure = 2;
		triangleChecker = 3;
	}
    
    if (event.keysym.sym == SDLK_3) { //Si se presiona 3 se dibuja un circulo
		lastFigure = 3;
		triangleChecker = 3;
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
		triangleChecker = 0;
	}

	if (event.keysym.sym == SDLK_5) {
		cursorPaintTool = true;
	}
    
    if (event.keysym.sym == SDLK_6){
        float total_time = 10000;
        float current_time = 0;
        float dt = 1;
        
        ParticleSystem ps; // Creamos el sistema de particulas
        ps.Init(); // Inciamos sus valores
        ps.Render(&framebuffer); // Introducimos los valores iniciales en el framebuffer
        
        // Bucle que actualiza los valores mientras no llegue el tiempo total
        while (current_time < total_time){
            ps.Update(dt); // Actualizamos los valores
            ps.Render(&framebuffer); // Introducimos los valores en el framebuffer
            
            //std::this_thread::sleep_for(std::chrono::milliseconds(15)); Tiempo real entre cada loop (idealmente debería ser mayor pero no ejecuta el ordenador bien (no se si esta es la manera correcta de hacer este paso)
           
            current_time = current_time + dt; // Actualizamos el tiempo total
        }
    }

	if (event.keysym.sym == SDLK_f) {
		isFigureFilled = true;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		if (mouse_position.y >= toolbarButton.getAltura()) {
			// Deteccion de coordenadas para la linea, el rectangulo y el circulo
			if (!drawingInCourse && modeSelected && !cursorPaintTool && !isDrawing) {
				initPosX = mouse_position.x;
				initPosY = mouse_position.y;
				drawingInCourse = true;
			}
			else if (drawingInCourse && modeSelected && !cursorPaintTool && !isDrawing) {
				endPosX = mouse_position.x;
				endPosY = mouse_position.y;
				if (lastFigure == 1) {
					framebuffer.DrawLineDDA(initPosX, initPosY, endPosX, endPosY, borderFill);
				}
				else if (lastFigure == 2) {
					width = endPosX - initPosX;
					height = endPosY - initPosY;
					if (isFigureFilled) {
						framebuffer.DrawRect(initPosX, initPosY, width, height, borderFill, borderWidth, true, colorFill);
					}
					else {
						framebuffer.DrawRect(initPosX, initPosY, width, height, borderFill, borderWidth, false, colorFill);
					}
				}
				else if (lastFigure == 3) {
					radio = sqrt((endPosX - initPosX) * (endPosX - initPosX) + (endPosY - initPosY) * (endPosY - initPosY));
					if (isFigureFilled) {
						framebuffer.DrawCircle(initPosX, initPosY, radio, borderFill, borderWidth, true, colorFill);
					}
					else {
						framebuffer.DrawCircle(initPosX, initPosY, radio, borderFill, borderWidth, false, colorFill);
					}
				}
				drawingInCourse = false;
			}

			if (cursorPaintTool) {
				isDrawing = true;
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
					if (isFigureFilled) {
						framebuffer.DrawTriangle(vector0, vector1, vector2, Color::WHITE, true, borderFill);
					}
					else {
						framebuffer.DrawTriangle(vector0, vector1, vector2, Color::WHITE, false, borderFill);
					}
					
				}
			}
		}
		
		if (clearButton.isMouseInside(mouse_position) == true) {
			framebuffer.DrawRect(0, toolbarButton.getAltura(), window_width, (window_height - toolbarButton.getAltura()), Color::BLACK, 1, true, Color::BLACK);
			lastFigure = 5;
			modeSelected = false;
			drawingInCourse = false;
			cursorPaintTool = false;
			isDrawing = false;
		}
		else if (loadButton.isMouseInside(mouse_position) == true) {
			Image imagenCargada;
			imagenCargada.LoadPNG("images/fruits.png", true);
			imagenCargada.Resize(1080, 520);
			framebuffer.DrawImage(imagenCargada, 100, 100, false);
		}
		else if (saveButton.isMouseInside(mouse_position) == true) {
			Image imagenGuardada;
			imagenGuardada.SaveTGA("IMAGEN_GUARDADA_(saveTGA)");
		}
		else if (lineButton.isMouseInside(mouse_position) == true) {
			lastFigure = 1;
			modeSelected = true;
			cursorPaintTool = false;
			isDrawing = false;
		} 
		else if (rectangleButton.isMouseInside(mouse_position) == true) {
			lastFigure = 2;
			modeSelected = true;
			cursorPaintTool = false;
			isDrawing = false;
		}
		else if (circleButton.isMouseInside(mouse_position) == true) {
			lastFigure = 3;
			modeSelected = true;
			cursorPaintTool = false;
			isDrawing = false;
		}
		else if (triangleButton.isMouseInside(mouse_position) == true) {
			lastFigure = 4;
			modeSelected = true;
			triangleChecker = 0;
			cursorPaintTool = false;
			isDrawing = false;
		}
		else if (blackButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::BLACK;
		}
		else if (redButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::RED;
		}
		else if (greenButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::GREEN;
		}
		else if (blueButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::BLUE;
		}
		else if (yellowButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::YELLOW;
		}
		else if (pinkButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::PURPLE;
		}
		else if (cyanButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::CYAN;
		}
		else if (whiteButton.isMouseInside(mouse_position) == true) {
			colorFill = Color::WHITE;
		}
	}

	if (event.button == SDL_BUTTON_RIGHT) {
		if (blackButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::BLACK;
		}
		else if (redButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::RED;
		}
		else if (greenButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::GREEN;
		}
		else if (blueButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::BLUE;
		}
		else if (yellowButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::YELLOW;
		}
		else if (pinkButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::PURPLE;
		}
		else if (cyanButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::CYAN;
		}
		else if (whiteButton.isMouseInside(mouse_position) == true) {
			borderFill = Color::WHITE;
		}
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		isDrawing = false;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (isDrawing) {
		framebuffer.SetPixel(mouse_position.x, mouse_position.y, colorFill);
		framebuffer.SetPixel(mouse_position.x + 1, mouse_position.y, colorFill);
		framebuffer.SetPixel(mouse_position.x - 1, mouse_position.y, colorFill);
		framebuffer.SetPixel(mouse_position.x, mouse_position.y + 1, colorFill);
		framebuffer.SetPixel(mouse_position.x, mouse_position.y - 1, colorFill);
		framebuffer.SetPixel(mouse_position.x + 1, mouse_position.y + 1, colorFill);
		framebuffer.SetPixel(mouse_position.x - 1, mouse_position.y + 1, colorFill);
		framebuffer.SetPixel(mouse_position.x + 1, mouse_position.y - 1, colorFill);
		framebuffer.SetPixel(mouse_position.x - 1, mouse_position.y - 1, colorFill);
	}
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
