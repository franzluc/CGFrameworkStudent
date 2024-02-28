// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 u_lightPosition;

varying vec3 N1;
varying vec3 L1;
varying vec3 V1;

// Global variables from the CPU
uniform sampler2D u_textureEntity;
uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_shininess;

void main()
{
    vec3 N = normalize(N1);
    vec3 L = normalize(L1);
    vec3 V = normalize(V1);
    vec3 R = reflect(-L, N);

	// Cargamos los colores de la textura de la entidad en función de las coordenadas.
	vec4 t0 = texture2D(u_textureEntity,  v_uv);

    u_kd = t0.xyz;

    u_ka += t0.xyz;

    float alpha = t0.z;
    vec3 greyscale = vec3(alpha);
    u_ks = greyscale;

    vec3 ambient = u_ka * u_Ia;
    vec3 dif = u_kd * u_Id * clamp(dot(N,L), 0.0, 1.0);
    vec3 spec = u_ks * u_Is * pow(clamp(dot(R, V), 0.0, 1.0), u_shininess);
    vec3 cv = ambient + (1.0 / pow(distance(v_world_position, u_lightPosition) ,2.0)) * (dif + spec);

    // Se pinta el pixel
	gl_FragColor = vec4(cv, 1.0);
}
