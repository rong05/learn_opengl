#version 410
   
in vec3 VertColor;
in vec2 TextCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float mixValue;

out vec4 color;


void main()
{
	vec4 color1 = texture(tex1, vec2(TextCoord.s, 1.0 - TextCoord.t));
	vec4 color2 = texture(tex2, TextCoord);
	color = mix(color2, color1, mixValue);
}