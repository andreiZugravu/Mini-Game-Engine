attribute vec3 a_posL;

uniform mat4 u_MVP;

varying vec3 v_coord;

void main()
{
	vec4 posL = u_MVP * vec4(a_posL, 1.0) ;
	gl_Position = posL;
	v_coord = a_posL;
}
   