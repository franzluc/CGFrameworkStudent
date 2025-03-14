#include "material.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

Material::Material(){}

Material::Material(Shader* shader, Texture* textura, Texture* nText,  float Ka, float Kd, float Ks) {
	this->shader = shader;
	this->textura = textura;
    this->nText = nText;
	this->Ka = Ka;   //Ambience
	this->Kd = Kd;   //Diffuse
	this->Ks = Ks;   //Specular
}

void Material::Enable(const sUniformData& sUniform, int index) {
	
    shader->Enable();
    
    shader->SetUniform1("ex", sUniform.ex);
    shader->SetVector3("prob", sUniform.prob);

    //Variables que se pasamos a los shaders
    shader->SetMatrix44("u_model", sUniform.modelMatrix);
    shader->SetMatrix44("u_viewprojection", sUniform.camara->viewprojection_matrix);
    shader->SetVector3("u_eye", sUniform.camara->eye);

    //Se establece la textura a renderizar
    shader->SetTexture("u_textureEntity", textura);
    shader->SetTexture("u_normalTexture", nText);
    
    
    shader->SetVector3("u_Ia", index == 0 ? sUniform.Ia : Vector3 {0,0,0});
    
	
    
    
    shader->SetVector3("u_Id", sUniform.lightsMult[index].intensidadId);
    shader->SetVector3("u_Is", sUniform.lightsMult[index].intensidadIs);
    shader->SetVector3("u_lightPosition", sUniform.lightsMult[index].posicion);
    

	shader->SetVector3("u_ka", sUniform.Ka);
	shader->SetVector3("u_kd", sUniform.Kd);
	shader->SetVector3("u_ks", sUniform.Ks);

	
    shader->SetFloat("u_shininess", sUniform.brillo);

}

void Material::Disable() {
	shader->Disable();
}
