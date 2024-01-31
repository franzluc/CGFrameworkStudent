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

Entity::Entity(){
}

Entity::Entity(Mesh m){
    this->malla = m;
   
    this->matrixModel.SetIdentity();
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
        Vector4 v1 = { vertices[i].x, vertices[i].y, vertices[i].z, 1 };
        Vector4 v2 = { vertices[i+1].x, vertices[i+1].y, vertices[i+1].z, 1 };
        Vector4 v3 = { vertices[i+2].x, vertices[i+2].y, vertices[i+2].z, 1 };

        v1 = matrixModel * v1;
        v2 = matrixModel * v2;
        v3 = matrixModel * v3;

        // Se proyectan los vertices al clip space usando la camara
        bool negZ1, negZ2, negZ3;
        Vector3 clipPos1 = camera->ProjectVector(v1.GetVector3(), negZ1);
        Vector3 clipPos2 = camera->ProjectVector(v2.GetVector3(), negZ2);
        Vector3 clipPos3 = camera->ProjectVector(v3.GetVector3(), negZ3);

        // Se comprueba que ningun vertice este fuera de la camara
        if (negZ1 || negZ2 || negZ3) {
            continue;
        }

        // Se convierten las posiciones del clip space al screen space
        float w1 = v1.w;
        float w2 = v2.w;
        float w3 = v3.w;

        v1.GetVector3() = v1.GetVector3() / w1;
        v2.GetVector3() = v2.GetVector3() / w2;
        v3.GetVector3() = v3.GetVector3() / w3;

        // Convierte el screen space usando la anchura y altura del framebuffer
        int screenWidth = framebuffer->width;
        int screenHeight = framebuffer->height;

        int x1 = static_cast<int>((clipPos1.x + 1.0f) * 0.5f * screenWidth);
        int y1 = static_cast<int>((1.0f - clipPos1.y) * 0.5f * screenHeight);

        int x2 = static_cast<int>((clipPos2.x + 1.0f) * 0.5f * screenWidth);
        int y2 = static_cast<int>((1.0f - clipPos2.y) * 0.5f * screenHeight);

        int x3 = static_cast<int>((clipPos3.x + 1.0f) * 0.5f * screenWidth);
        int y3 = static_cast<int>((1.0f - clipPos3.y) * 0.5f * screenHeight);

        // Se dibuja la malla de triangulos usando el algoritmo DDA
        framebuffer->DrawLineDDA(x1, y1, x2, y2, c);
        framebuffer->DrawLineDDA(x2, y2, x3, y3, c);
        framebuffer->DrawLineDDA(x3, y3, x1, y1, c);
    }

    
}