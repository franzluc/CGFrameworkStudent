//
//  entity.h
//  ComputerGraphics
//
//  Created by Francisco Luc on 30/1/24.
//
#pragma once

#include "mesh.h"
#include "image.h"

class Entity {
public:
    Mesh malla;
    Matrix44 matrixModel;
    
    
    Entity();
    Entity(Mesh malla);
    Entity(Matrix44 matrix);
    Entity(Mesh malla, int matrixModel[4][4]);

    void Render(Image* framebuffer, Camera* camera, const Color& c);
    void Update(float seconds_elapsed, int type);
};


