#include "button.h"
<<<<<<< Updated upstream
#include "framework/application.h"
/*
Button::Button(const char& _imatge, const Vector2& _posicio)
{
	Image imatge;
	imatge.LoadPNG(&_imatge, true);
	posicio.x = _posicio.x;
	posicio.y = _posicio.y;
}

 */
=======

Button::Button(const char& _image, Vector2& _position) {
	ruta = &_image;
	posicio = _position;
	imagen.LoadPNG(ruta, true);
}

void Button::DrawButton(Image& framebuffer) {
	framebuffer.DrawImage(imagen, posicio.x, posicio.y, false);
}

bool Button::isMouseInside(Vector2 coords) {
	if (coords.x >= posicio.x && coords.x <= (posicio.x + 32) && coords.y >= posicio.y && coords.y <= (posicio.y + 32))
	{
		return true;
	}
	else {
		return false;
	}
}
>>>>>>> Stashed changes
