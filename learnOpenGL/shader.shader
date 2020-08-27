#pragma vertex
#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexcoord;
out vec2 Texcoord;
void main(){
gl_Position=projection*view*model*vec4(aPos,1.0);
Texcoord=aTexcoord;
}


#pragma fragment
#version 330 core
out vec4 FragColor;

in vec2 Texcoord;

uniform sampler2D TEXTURE0;
uniform sampler2D TEXTURE1;

void main(){
vec4 TexColor0=texture(TEXTURE0,Texcoord);
vec4 TexColor1=texture(TEXTURE1,Texcoord);
FragColor=mix(TexColor0,TexColor1,0.2f);
}