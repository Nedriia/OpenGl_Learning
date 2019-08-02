#version 330 core
out vec4 FragColor;

in vec3 vertex_color;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
//Here we don't have to assign a location value to tehe texture sampler2D
//We have only one texture so the program render the square with that texture
//-> default texture

void main()
{
	FragColor = mix(texture(texture1,TexCoord), texture(texture2,TexCoord),0.3);
};