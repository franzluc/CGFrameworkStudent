#pragma once
#include <framework/image.h>


class Button
{
private:
	Image imagen;
	const char* ruta;
	Vector2 posicio;
	int altura;
	int ancho;

public:
	Button(const char& _image, Vector2& _position);
	void DrawButton(Image& framebuffer);
	bool isMouseInside(Vector2 coords);
	int getAltura();
	int getAncho();
};
