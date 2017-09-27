attribute vec3 a_posL;

uniform mat4 u_MVP;

attribute vec2 a_uv;
attribute vec2 a_uv_map;

varying vec2 v_uv;
varying vec2 v_uv_map;

uniform sampler2D u_blend_map;
uniform vec2 u_offset;

uniform mat4 u_Model;

varying vec3 v_position;

void main()
{
	v_uv = a_uv;
	v_uv_map = a_uv_map + u_offset;

	vec4 model_space_position = vec4(a_posL, 1.0);
	vec3 color = texture2D(u_blend_map, v_uv_map).rgb;

	model_space_position.y += color.r * 100.0 + color.g * 5.0 + color.b * 1.0;
	
	gl_Position = u_MVP * model_space_position;
	v_position = vec3(u_Model * vec4(a_posL, 1.0));
}
   