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

Entity::Entity(Mesh m, Image t){
    
    this->malla = m;
    this->textura = t;
    this->matrixModel.SetIdentity();
    
}
 
// Definimos la función render que unirá los vertices de nuestra malla
void Entity::Render(Camera* camera) {

    shaderEntity = Shader::Get("shaders/simple.vs", "shaders/simple.fs");

    shaderEntity->Enable();

    // Pass necessary CPU variables to the GPU
    shaderEntity->SetMatrix44("u_model", matrixModel);
    shaderEntity->SetMatrix44("u_viewprojection", camera->viewprojection_matrix);

    // Set the texture for the entity
    entityTexture = Texture::Get("textures/lee_color_specular.tga");
    shaderEntity->SetTexture("u_textureEntity", entityTexture);

    // Render the mesh
    malla.Render(); // Assuming the mesh has a Render() method

    // Unbind the shader
    shaderEntity->Disable();
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
