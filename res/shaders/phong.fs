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

uniform vec3 prob;

uniform float u_shininess;

uniform vec3 u_lightPosition2;
uniform vec3 u_Id2;
uniform vec3 u_Is2;

vec3 ka_u;
vec3 ks_u;
vec3 kd_u;

vec3 Normal;

void main()
{
	
	if (ex == 1){
		vec3 N = normalize(v_world_normal);  
		vec3 L = normalize(u_lightPosition - v_world_position);
		vec3 R = reflect(- L, N); 
		vec3 V = normalize(u_eye - v_world_position);

		float d = (1.0 / pow(distance(v_world_position, u_lightPosition) , 2.0));

	
		vec3 t0 = texture2D(u_textureEntity,  v_uv).xyz;
		float alpha = texture2D(u_textureEntity,  v_uv).w;
		vec3 t1 = vec3(alpha, alpha, alpha);

		vec3 NNN = texture2D(u_normalTexture,  v_uv).xyz;
		NNN = NNN * 2.0 - 1.0;
		vec3 normalInverse = normalize((u_model * vec4(NNN, 1.0)).xyz);

		
		Normal = N;
		
		if(prob.x == 1.0){
			ka_u = u_ka*t0;
			kd_u = t0;
		} else {
			ka_u = u_ka;
			kd_u = u_kd;
		}

		if(prob.y == 1.0){
			ks_u = t1;
		} else {
			ks_u = u_ks;

		}

		if(prob.z == 1.0){
			N = normalInverse;
		}else{
			N = Normal;
		}
		

		vec3 ambient = ka_u * u_Ia;
		vec3 dif = kd_u * u_Id * clamp(dot(L, N), 0.0, 1.0);
		vec3 spec = ks_u * u_Is * pow(clamp(dot(R, V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  d*(dif + spec);
		
		gl_FragColor = vec4(cv, 1.0);
	
	}

	if (ex == 2){
		vec3 N = normalize(v_world_normal);  
		vec3 L = normalize(u_lightPosition - v_world_position);
		vec3 L2 = normalize(u_lightPosition2 - v_world_position);
		vec3 R = reflect(- L, N);
		vec3 R2 = reflect(- L2, N);
		vec3 V = normalize(u_eye - v_world_position);

		float d = (1.0 / pow(distance(v_world_position, u_lightPosition) , 2.0));
		float d2 = (1.0 / pow(distance(v_world_position, u_lightPosition2) , 2.0));

	
		vec3 t0 = texture2D(u_textureEntity,  v_uv).xyz;
		float alpha = texture2D(u_textureEntity,  v_uv).w;
		vec3 t1 = vec3(alpha, alpha, alpha);

		vec3 NNN = texture2D(u_normalTexture,  v_uv).xyz;
		NNN = NNN * 2.0 - 1.0;
		vec3 normalInverse = normalize((u_model * vec4(NNN, 1.0)).xyz);

		
		Normal = N;
		
		if(prob.x == 1.0){
			ka_u = u_ka*t0;
			kd_u = t0;
		} else {
			ka_u = u_ka;
			kd_u = u_kd;
		}

		if(prob.y == 1.0){
			ks_u = t1;
		} else {
			ks_u = u_ks;

		}

		if(prob.z == 1.0){
			N = normalInverse;
		}else{
			N = Normal;
		}
		

		vec3 ambient = ka_u * u_Ia;
		vec3 dif = kd_u * u_Id * clamp(dot(L, N), 0.0, 1.0);
		vec3 spec = ks_u * u_Is * pow(clamp(dot(R, V), 0.0, 1.0), u_shininess);

		vec3 dif2 = kd_u * u_Id2 * clamp(dot(L2, N), 0.0, 1.0);
		vec3 spec2 = ks_u * u_Is2 * pow(clamp(dot(R2, V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  d*(dif + spec) + d2*(dif2 + spec2);
		
		gl_FragColor = vec4(cv, 1.0);
	}
	
	
}
