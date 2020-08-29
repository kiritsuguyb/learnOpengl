#pragma vertex
#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
layout (location=0) in vec3 aPos;
void main(){
gl_Position=projection*view*model*vec4(aPos,1.0);
}


#pragma fragment
#version 330 core
out vec4 FragColor;

in vec2 Texcoord;

uniform vec3 lightColor;

void main(){
FragColor=vec4(lightColor,1.0f);
}