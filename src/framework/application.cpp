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

	Image imatge;
	bool carrega = imatge.LoadPNG("images/toolbar2.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/toolbar2.png", true);
		framebuffer.DrawImage(imatge, 0, 0, false);
	}
	carrega = imatge.LoadPNG("images/clear.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/clear.png", true);
		framebuffer.DrawImage(imatge, 10, 16, false);
	}
	carrega = imatge.LoadPNG("images/load.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/load.png", true);
		framebuffer.DrawImage(imatge, 52, 16, false);
	}
	carrega = imatge.LoadPNG("images/save.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/save.png", true);
		framebuffer.DrawImage(imatge, 94, 16, false);
	}
	carrega = imatge.LoadPNG("images/eraser.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/eraser.png", true);
		framebuffer.DrawImage(imatge, 136, 16, false);
	}
	carrega = imatge.LoadPNG("images/line.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/line.png", true);
		framebuffer.DrawImage(imatge, 178, 16, false);
	}
	carrega = imatge.LoadPNG("images/rectangle.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/rectangle.png", true);
		framebuffer.DrawImage(imatge, 220, 16, false);
	}
	carrega = imatge.LoadPNG("images/circle.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/circle.png", true);
		framebuffer.DrawImage(imatge, 262, 16, false);
	}
	carrega = imatge.LoadPNG("images/triangle.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/triangle.png", true);
		framebuffer.DrawImage(imatge, 304, 16, false);
	}
	carrega = imatge.LoadPNG("images/black.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/black.png", true);
		framebuffer.DrawImage(imatge, 346, 16, false);
	}
	carrega = imatge.LoadPNG("images/white.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/white.png", true);
		framebuffer.DrawImage(imatge, 388, 16, false);
	}
	carrega = imatge.LoadPNG("images/pink.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/pink.png", true);
		framebuffer.DrawImage(imatge, 430, 16, false);
	}
	carrega = imatge.LoadPNG("images/yellow.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/yellow.png", true);
		framebuffer.DrawImage(imatge, 472, 16, false);
	}
	carrega = imatge.LoadPNG("images/red.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/red.png", true);
		framebuffer.DrawImage(imatge, 514, 16, false);
	}
	carrega = imatge.LoadPNG("images/blue.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/blue.png", true);
		framebuffer.DrawImage(imatge, 556, 16, false);
	}
	carrega = imatge.LoadPNG("images/cyan.png");
	if (carrega == false) {
		std::cout << "Image not found!" << std::endl;
	}
	else {
		imatge.LoadPNG("images/cyan.png", true);
		framebuffer.DrawImage(imatge, 598, 16, false);
	}
	
	
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
		framebuffer.DrawLineDDA(initPosX, initPosY, endPosX, endPosY, Color::WHITE);
		lastFigure = 1;
	}

	if (event.keysym.sym == SDLK_2) { //Si se presiona 2 se dibuja un rectangulo
		width = endPosX - initPosX;
		height = endPosY - initPosY;
		framebuffer.DrawRect(initPosX, initPosY, width, height, Color::WHITE, borderWidth, true, Color::BLUE);
		lastFigure = 2;
	}
    
    if (event.keysym.sym == SDLK_3) { //Si se presiona 3 se dibuja un circulo
        radio = sqrt((endPosX-initPosX)*(endPosX-initPosX)+(endPosY-initPosY)*(endPosY-initPosY));
        framebuffer.DrawCircle(initPosX, initPosY, radio, Color::WHITE, borderWidth, true, Color::BLUE);
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
		framebuffer.DrawTriangle(vector0, vector1, vector2, Color::WHITE, false, Color::BLUE);
		lastFigure = 4;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		// Deteccion de coordenadas para la linea, el rectangulo y el circulo
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

		// Deteccion del coordenadas para el triangulo
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
