// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_normal;
varying vec3 v_cv;
uniform sampler2D u_textureEntity;

varying vec3 v_N;
varying vec3 v_L;
varying vec3 v_V;
varying float v_d;

uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;

uniform float u_shininess;

int ex = 2;

void main()
{
	if  (ex == 1){
		// Set the ouput color per pixel
		vec3 color = normalize(v_world_normal);
		
		vec3 R = reflect(-v_L, v_N);

		vec3 ambient = u_ka * u_Ia;
		vec3 dif = u_kd * u_Id * clamp(dot(v_N,v_L), 0.0, 1.0);
		vec3 spec = u_ks * u_Is * pow(clamp(dot(R,v_V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  v_d*(dif + spec);
		
		gl_FragColor = vec4(cv, 1.0);

	}
	if  (ex == 2) {
		
		vec4 t0 = texture2D(u_textureEntity,  v_uv);
		

		vec3 R = reflect(-v_L, v_N);

		vec3 ambient = (u_ka+t0.xyz) * u_Ia;
		vec3 dif = t0.xyz * u_Id * clamp(dot(v_N,v_L), 0.0, 1.0);
		vec3 spec = u_ks * u_Is * pow(clamp(dot(R,v_V), 0.0, 1.0), u_shininess);
		
		vec3 cv = ambient +  v_d*(dif + spec);
		
		gl_FragColor = vec4(cv, 1.0);

	}

		
}
