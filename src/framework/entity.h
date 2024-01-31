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

    void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);
};
