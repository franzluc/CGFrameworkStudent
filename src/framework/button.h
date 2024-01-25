#pragma once
#include <framework/image.h>

/*
class Button
{
private:
	Image imagen;
	const char* ruta;
	Vector2 posicio;

public:
<<<<<<< Updated upstream
	Button(const char& _imatge, const Vector2& _posicio);
};

*/
=======
	Button(const char& _image, Vector2& _position);
	void DrawButton(Image& framebuffer);
	bool isMouseInside(Vector2 coords);
};
>>>>>>> Stashed changes
