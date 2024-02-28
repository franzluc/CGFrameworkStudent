// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform vec3 u_eye;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 u_lightPosition;

varying vec3 N1;
varying vec3 L1;
varying vec3 V1;

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

    vec3 N1 = normalize(v_world_normal);
    vec3 L1 = u_lightPosition - world_position;
    vec3 V1 = normalize(-v_world_position);

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
}