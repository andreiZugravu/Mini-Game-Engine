attribute vec3 a_posL;

uniform mat4 u_MVP;

attribute vec2 a_uv;

varying vec2 v_uv;

uniform mat4 u_Model;

varying vec3 v_position;

void main()
{
	vec4 posL = u_MVP * vec4(a_posL, 1.0) ;
	gl_Position = posL;
	v_uv = a_uv;
	v_position = vec3(u_Model * vec4(a_posL, 1.0));
}
