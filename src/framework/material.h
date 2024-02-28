#pragma once

#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "light.h"

struct sUniformData{
    Matrix44 modelMatrix;
    Matrix44 view_projection_matrix;
    float Ia;
    float Id;
    float Is;
    float Ka;
    float Kd;
    float Ks;
    Vector3 lightPosition;
    float brillo;
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



