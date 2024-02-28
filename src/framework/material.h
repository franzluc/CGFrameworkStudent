#pragma once

#include "texture.h"
#include "shader.h"
#include "camera.h"


struct sUniformData{
    Matrix44 modelMatrix;
    Camera* camara = nullptr;
    Vector3 Ia;
    Vector3 Id;
    Vector3 Is;
    Vector3 Ka;
    Vector3 Kd;
    Vector3 Ks;
    Vector3 lightPosition;
    float brillo;
};

struct Light {
    Vector3 posicion;
    Vector3 intensidadId;
    Vector3 intensidadIs;
};


class Material
{
public:
	Shader* shader = nullptr;
	Texture* textura = nullptr;
    Vector3 Ka;
    Vector3 Kd;
    Vector3 Ks;
	float Shininess = 0.5;
    
    Material();
	Material(Shader* shader, Texture* textura, float Ka, float Kd, float Ks);
	void Enable(const sUniformData& sUniform);
	void Disable();
};



