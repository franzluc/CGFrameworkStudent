//
//  entity.h
//  ComputerGraphics
//
//  Created by Francisco Luc on 30/1/24.
//
#pragma once

#include "mesh.h"
#include "image.h"
#include "texture.h"

class Entity { // Creamos la clase entidad compuesta por una malla y una matrix model
public:
    // Atributos
    Mesh malla;
    Matrix44 matrixModel;
    Image textura;
    
    // Distintos constructores
    Entity();
    Entity(Mesh malla);
    Entity(Matrix44 matrix);
    Entity(Mesh malla, Matrix44 matrix);
    Entity(Mesh malla, Image textura);
    
    
    // Métodos de la clase
    void Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
    void Update(float seconds_elapsed, int type);
};

/*
enum class eRenderMode {
    POINTCLOUD,
    WIREFRAME,
    TRIANGLES,
    TRIANGLES_INTERPOLATED
};
eRenderMode mode;
*/

