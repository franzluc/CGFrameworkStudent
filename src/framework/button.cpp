#include "button.h"
#include "framework/application.h"

Button::Button(const char& _image, Vector2& _position) {
	ruta = &_image;
	posicio = _position;
	imagen.LoadPNG(ruta, true);
	altura = imagen.height;
	ancho = imagen.width;
}

void Button::DrawButton(Image& framebuffer) {
	framebuffer.DrawImage(imagen, posicio.x, posicio.y, false);
}

bool Button::isMouseInside(Vector2 coords) {
	if (coords.x >= posicio.x && coords.x <= (posicio.x + ancho) && coords.y >= posicio.y && coords.y <= (posicio.y + altura))
	{
		return true;
	}
	else {
		return false;
	}
}

int Button::getAltura() {
	return altura;
}

int Button::getAncho() {
	return ancho;
}
