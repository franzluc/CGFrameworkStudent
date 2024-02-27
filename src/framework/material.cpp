#include "material.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "entity.h"
#include "light.h"

Material::Material(){}

Material::Material(Shader* shader, Texture* textura, float Ka, float Kd, float Ks) {
	this->shader = shader;
	this->textura = textura;
	this->Ka = Ka;   //Ambience
	this->Kd = Kd;   //Diffuse
	this->Ks = Ks;   //Specular
}

void Material::Enable(Camera* camera, Entity entity, float ambientIntensity, Light lights) {
	shader->Enable();

    //Variables que se pasamos a los shaders
    shader->SetMatrix44("u_model", entity.matrixModel);
    shader->SetMatrix44("u_viewprojection", camera->viewprojection_matrix);

    //Se establece la textura a renderizar
    shader->SetTexture("u_textureEntity", textura);

	shader->SetFloat("u_Ia", ambientIntensity);

	shader->SetFloat("u_ka", Ka);
	shader->SetFloat("u_kd", Kd);
	shader->SetFloat("u_ks", Ks);

	shader->SetVector3("u_lightPosition", lights.posicion);

	shader->SetFloat("u_Id", lights.intensidadId);
	shader->SetFloat("u_Is", lights.intensidadIs);

	shader->SetFloat("u_shininess", Shininess);

}

void Material::Disable() {
	shader->Disable();
}