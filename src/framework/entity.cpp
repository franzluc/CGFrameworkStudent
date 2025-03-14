//
//  entity.cpp
//  ComputerGraphics
//
//  Created by Francisco Luc on 30/1/24.
//

#include <stdio.h>
#include "entity.h"
#include "mesh.h"
#include "image.h"
#include "texture.h"
#include "material.h"

// Definimos los distintos constructores posibles

Entity::Entity(){
}

Entity::Entity(Mesh m){
    this->malla = m;
   
    this->matrixModel.SetIdentity();
}

Entity::Entity(Matrix44 matrix) {
    for(int i=0;i<4;i++){
        for(int j = 0; j<4; j++){
            this->matrixModel.M[i][j] = matrix.M[i][j];
        }
    }
}

Entity::Entity(Mesh m, Matrix44 matrix){
    this->malla = m;
    for(int i=0;i<4;i++){
        for(int j = 0; j<4; j++){
            this->matrixModel.M[i][j] = matrix.M[i][j];
        }
    }

}



Entity::Entity(Mesh malla, Material *material) {
    this->malla = malla;
    this->material = material;
}
 
// Definimos la función render que unirá los vertices de nuestra malla
void Entity::Render(sUniformData uniformData) {
    
    uniformData.modelMatrix = matrixModel;
    
    glDisable( GL_BLEND );
    material->Enable(uniformData, 0);
    malla.Render();
    material->Disable();
    
    

    for (int i=1; i < uniformData.lightsMult.size(); i++){
        glEnable( GL_BLEND );
        glBlendFunc( GL_ONE, GL_ONE );
        material->Enable(uniformData, i);
        malla.Render();
        material->Disable();
        
    }
}

// Definimos la función update de actualizará nuestra entidad
void Entity::Update(float seconds_elapsed, int type) {
    
    // Rotación
    for (int i= 0; i < seconds_elapsed; i++){
        if (type == 0) {
            for (float radians = 0; radians <= 1; radians += 0.01) {
                matrixModel.RotateLocal(radians, { 0, 1, 0 });
                if (radians == 1) {
                    radians = 0;
                }
            }
        }
        // Escala (No funciona)
        else if (type == 1) {
            for (float i = 1; i <= 2; i += 0.01) {
                matrixModel.ScaleLocal(i, i, i);
                if (i == 2) {
                    i = 1;
                }
            }
        }
        // Traslación (No funciona)
        else if (type == 2) {
            for (float i = -0.4; i > 1; i += 0.01) {
                matrixModel.Translate(0, i, 0);
                if (i == 1) {
                    i = -0.4;
                }
            }
        }
    }
}
