#include "material.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "light.h"

Material::Material(){}

Material::Material(Shader* shader, Texture* textura, float Ka, float Kd, float Ks) {
	this->shader = shader;
	this->textura = textura;
	this->Ka = Ka;   //Ambience
	this->Kd = Kd;   //Diffuse
	this->Ks = Ks;   //Specular
}

void Material::Enable(const sUniformData& sUniform) {
	
    shader->Enable();

    //Variables que se pasamos a los shaders
    shader->SetMatrix44("u_model", sUniform.modelMatrix);
    shader->SetMatrix44("u_viewprojection", sUniform.view_projection_matrix);

    //Se establece la textura a renderizar
    shader->SetTexture("u_textureEntity", textura);

	shader->SetFloat("u_Ia", sUniform.Ia);
    shader->SetFloat("u_Id", sUniform.Id);
    shader->SetFloat("u_Is", sUniform.Is);

	shader->SetFloat("u_ka", sUniform.Ka);
	shader->SetFloat("u_kd", sUniform.Kd);
	shader->SetFloat("u_ks", sUniform.Ks);

	shader->SetVector3("u_lightPosition", sUniform.lightPosition);

    shader->SetFloat("u_shininess", sUniform.brillo);

}

void Material::Disable() {
	shader->Disable();
}
