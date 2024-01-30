//
//  entity.cpp
//  ComputerGraphics
//
//  Created by Francisco Luc on 30/1/24.
//

#include <stdio.h>
#include "entity.h"
#include "mesh.h"

Entity::Entity(){
}

Entity::Entity(Mesh m){
    this->malla = m;
   
    matrixModel.SetIdentity();
}

/*Entity::Entity(Matrix44 matrix) {
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

}*/

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
    // Se cogen los vertices del mesh
    const std::vector<Vector3>& vertices = malla.GetVertices();

    // Se itera por cada vertice
    for (int i = 0; i < vertices.size(); i += 3) {
        // Se transforman los vertices de local space a world space.
        Vector3 v1 = matrixModel * vertices[i];
        Vector3 v2 = matrixModel * vertices[i + 1];
        Vector3 v3 = matrixModel * vertices[i + 2];

        // Se proyectan los vertices al clip space usando la camara
        bool negZ1, negZ2, negZ3;
        Vector3 clipPos1 = camera->ProjectVector(v1, negZ1);
        Vector3 clipPos2 = camera->ProjectVector(v2, negZ2);
        Vector3 clipPos3 = camera->ProjectVector(v3, negZ3);

        // Se comprueva que ningun vertice este fuera de la camara
        if (negZ1 || negZ2 || negZ3) {
            continue;
        }

        // Se convierten las posiciones del clip space al screen space
        float w1 = clipPos1.w;
        float w2 = clipPos2.w;
        float w3 = clipPos3.w;

        clipPos1 /= w1;
        clipPos2 /= w2;
        clipPos3 /= w3;

    }

    
}