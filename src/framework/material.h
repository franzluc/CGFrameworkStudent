#pragma once

#include "texture.h"
#include "shader.h"
#include "camera.h"


struct sUniformData{
    Matrix44 modelMatrix;
    Camera* camara = nullptr;
    float Ia;
    float Id;
    float Is;
    float Ka;
    float Kd;
    float Ks;
    Vector3 lightPosition;
    float brillo;
};

struct Light {
    Vector3 posicion;
    float intensidadId;
    float intensidadIs;
};


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
	void Enable(const sUniformData& sUniform);
	void Disable();
};



