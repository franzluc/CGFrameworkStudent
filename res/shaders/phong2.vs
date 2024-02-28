// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform vec3 u_lightPosition; 
uniform vec3 u_eye;

uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;

uniform float u_shininess;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 v_N;
varying vec3 v_L;
varying vec3 v_V;
varying float v_d;


//here create uniforms for all the data we need here

void main()
{	
	v_uv = gl_MultiTexCoord0.xy;

	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;

	// Convert local normal to world space
	vec3 world_normal = (u_model * vec4(gl_Normal.xyz, 0.0)).xyz;

	// Pass them to the fragment shader interpolated
	v_world_position = world_position;
	v_world_normal = world_normal;

	vec3 N = normalize(world_normal);  
	vec3 L = normalize(u_lightPosition - world_position);
	vec3 R = reflect(-L, N); 
	vec3 V = normalize(-world_position);

	float d = (1.0 / pow(distance(world_position, u_lightPosition) ,2.0));

	v_N = N;
	v_L = L;
	v_V = V;
	v_d = d;

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader

}