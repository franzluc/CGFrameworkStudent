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
void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer) {
    // Se cogen los vertices del mesh
    const std::vector<Vector3>& vertices = malla.GetVertices();
    
    const std::vector<Vector2>& mallaUV = malla.GetUVs();

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

        float x1 = static_cast<int>((clipPos1.x + 1.0f) * 0.5f * screenWidth);
        float y1 = static_cast<int>((1.0f - clipPos1.y) * 0.5f * screenHeight);

        float x2 = static_cast<int>((clipPos2.x + 1.0f) * 0.5f * screenWidth);
        float y2 = static_cast<int>((1.0f - clipPos2.y) * 0.5f * screenHeight);

        float x3 = static_cast<int>((clipPos3.x + 1.0f) * 0.5f * screenWidth);
        float y3 = static_cast<int>((1.0f - clipPos3.y) * 0.5f * screenHeight);
        
        
        Vector2 uv1 = {mallaUV[i].x, mallaUV[i].y};
        Vector2 uv2 = {mallaUV[i+1].x, mallaUV[i+1].y};
        Vector2 uv3 = {mallaUV[i+2].x, mallaUV[i+2].y};
        
        uv1.x = (mallaUV[i].x + 1.0f) * 0.5f * textura.width;
        uv1.y = (1.0f - mallaUV[i].y) * 0.5f * textura.height;
        
        uv2.x = (mallaUV[i+1].x + 1.0f) * 0.5f * textura.width;
        uv2.y = (1.0f - mallaUV[i+1].y) * 0.5f * textura.height;
        
        uv3.x = (mallaUV[i+2].x + 1.0f) * 0.5f * textura.width;
        uv3.y = (1.0f - mallaUV[i+2].y) * 0.5f * textura.height;
        
        
        // Se dibuja la malla de triangulos usando el algoritmo DDA
        if (x1 < framebuffer->width && x2 < framebuffer->width && x3 < framebuffer->width){
            if (y1 < framebuffer->height && y2 < framebuffer->height && y3 < framebuffer->height){
                
                framebuffer->DrawTriangleInterpolated({ x1, y1, clipPos1.z}, { x2, y2, clipPos2.z}, { x3, y3, clipPos3.z}, Color::RED, Color::BLUE, Color::GREEN, zBuffer, &textura, uv1, uv2, uv3);
            }
        }
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
