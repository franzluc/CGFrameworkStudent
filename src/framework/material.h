#pragma once

#include "texture.h"
#include "shader.h"

class Material
{
public:
	Shader* shader = nullptr;
	Texture* textura = nullptr;
	float Ka;
	float Kd;
	float Ks;
	float Shininess = 0.5;

	Material();
	Material(Shader* shader, Texture* textura, float Ka, float Kd, float Ks);
	void Enable(Camera* camera, Entity entity, float ambientIntensity, Light lights);
	void Disable();
};