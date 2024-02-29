#pragma once

#include "texture.h"
#include "shader.h"
#include "camera.h"


struct sUniformData{
    Matrix44 modelMatrix;
    Camera* camara = nullptr;
    Vector3 Ia;   // Intensidad ambient light
    Vector3 Id;   // Intensidad specular light
    Vector3 Is;   // Intensidad diffuse light
    Vector3 Ka;   // Ambient reflection
    Vector3 Kd;   // Reflected diffuse light
    Vector3 Ks;   // Reflected specular light
    Vector3 lightPosition;   //Ip
    float brillo;
    int ex;
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
    Texture* nText = nullptr;
    Vector3 Ka;
    Vector3 Kd;
    Vector3 Ks;
	float Shininess = 0.5;
    
    Material();
	Material(Shader* shader, Texture* textura, Texture* nText, float Ka, float Kd, float Ks);
	void Enable(const sUniformData& sUniform);
	void Disable();
};



