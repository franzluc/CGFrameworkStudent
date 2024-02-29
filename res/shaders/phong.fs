// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_normal;
varying vec3 v_world_position;
uniform mat4 u_model;

uniform int ex;

uniform sampler2D u_textureEntity;
uniform sampler2D u_normalTexture;

uniform vec3 u_lightPosition; 
uniform vec3 u_eye; 

uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;

uniform float u_shininess;

//int ex = 4;

void main()
{
	
	
	vec3 N = normalize(v_world_normal);  
	vec3 L = normalize(u_lightPosition - v_world_position);
	vec3 R = reflect(- L, N); 
	vec3 V = normalize(u_eye - v_world_position);

	float d = (1.0 / pow(distance(v_world_position, u_lightPosition) , 2.0));
	
	
	if  (ex == 1){
		vec3 t0 = texture2D(u_textureEntity,  v_uv).xyz;
		
		vec3 ambient = u_ka * u_Ia;
		vec3 dif = u_kd * u_Id * clamp(dot(L,N), 0.0, 1.0);
		vec3 spec = u_ks * u_Is * pow(clamp(dot(R,V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  d*(dif + spec);
		
		gl_FragColor = vec4(cv, 1.0);

	}
	if  (ex == 2) {
		
		vec3 t0 = texture2D(u_textureEntity,  v_uv).xyz;
		
		vec3 ambient = u_ka*t0 * u_Ia;
		vec3 dif = t0 * u_Id * clamp(dot(L,N), 0.0, 1.0);
		vec3 spec = u_ks * u_Is * pow(clamp(dot(R,V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  d*(dif + spec);
		
		gl_FragColor = vec4(cv, 1.0);

	}

	if  (ex == 3) {
		
		
		vec3 t0 = texture2D(u_textureEntity,  v_uv).xyz;
		float alpha = texture2D(u_textureEntity,  v_uv).w;
		vec3 t1 = vec3(alpha, alpha, alpha);
		

		vec3 ambient = u_ka*t0 * u_Ia;
		vec3 dif = t0 * u_Id * clamp(dot(L,N), 0.0, 1.0);
		vec3 spec = t1 * u_Is * pow(clamp(dot(R,V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  d*(dif + spec);
		
		gl_FragColor = vec4(cv, 1.0);

	}

	if  (ex == 4) {
		
		vec3 NNN = texture2D(u_normalTexture,  v_uv).xyz;
		NNN = NNN * 2.0 - 1.0;
		vec3 normalInverse = normalize((u_model * vec4(NNN, 1.0)).xyz);



		vec3 t0 = texture2D(u_textureEntity,  v_uv).xyz;
		float alpha = texture2D(u_textureEntity,  v_uv).w;
		vec3 t1 = vec3(alpha, alpha, alpha);
		

		vec3 ambient = u_ka*t0 * u_Ia;
		vec3 dif = t0 * u_Id * clamp(dot(L, normalInverse), 0.0, 1.0);
		vec3 spec = t1 * u_Is * pow(clamp(dot(R, V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  d*(dif + spec);
		
		gl_FragColor = vec4(cv, 1.0);

	}
 
	if (ex == 5) {

		vec4 t0 = texture2D(u_textureEntity,  v_uv);
		gl_FragColor = t0;
	}

		
}
