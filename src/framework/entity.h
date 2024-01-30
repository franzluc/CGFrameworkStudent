//
//  entity.h
//  ComputerGraphics
//
//  Created by Francisco Luc on 30/1/24.
//
#pragma once

#include "mesh.h"

#ifndef entity_h
#define entity_h

class Entity {
public:
    Mesh malla;
    int matrixModel[4][4];
    
    
    Entity();
    Entity(Mesh malla);
    Entity(int matrix[4][4]);
    Entity(Mesh malla, int matrixModel[4][4]);
    
    ~Entity();
};



#endif /* entity_h */
