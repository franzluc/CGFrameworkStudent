//
//  entity.cpp
//  ComputerGraphics
//
//  Created by Francisco Luc on 30/1/24.
//

#include <stdio.h>
#include "entity.h"

Entity::Entity(){
}

Entity::Entity(Mesh m){
    this->malla = m;
   
    int matrixModel[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
    };
}

Entity::Entity(int matrix[4][4]){
    for(int i=0;i<4;i++){
        for(int j = 0; j<4; j++){
            this->matrixModel[i][j] = matrix[i][j];
        }
    }
    
}

Entity::Entity(Mesh m, int matrix[4][4]){
    this->malla = m;
    for(int i=0;i<4;i++){
        for(int j = 0; j<4; j++){
            this->matrixModel[i][j] = matrix[i][j];
        }
    }

}
