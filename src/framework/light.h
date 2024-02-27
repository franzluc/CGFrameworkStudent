#pragma once
#include "framework.h"

class Light
{
public:
	Vector3 posicion;
	float intensidadId;
	float intensidadIs;

	Light();
	Light(float Id, float Is);
};