#pragma vertex
#version 330 core
layout (location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexcoord;
uniform mat4 model;
uniform mat4 itmodel;
uniform mat4 view;
uniform mat4 projection;
out vec3 Normal;
out vec3 worldPos;
out vec2 Texcoord;
void main(){
gl_Position=projection*view*model*vec4(aPos,1.0);
Normal=normalize((itmodel*vec4(aNormal,0.0f)).xyz);
worldPos=(model*vec4(aPos,1.0)).xyz;
Texcoord=aTexcoord;
}


#pragma fragment
#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light_Directional{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 Texcoord;
in vec3 Normal;
in vec3 worldPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light_Directional lightDirectional;

void main(){
    vec3 ambient=lightDirectional.ambient*texture(material.diffuse,Texcoord).xyz;
    
    vec3 lightDir=normalize(-lightDirectional.direction);
    float diff=max(dot(lightDir,Normal),0.0f);
    vec3 diffuse=diff*lightDirectional.diffuse*texture(material.diffuse,Texcoord).xyz;

    vec3 reflectDir=reflect(-lightDir,Normal);
    vec3 viewDir=normalize(viewPos-worldPos);
    float spec=pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
    vec3 specular=spec*lightDirectional.specular*texture(material.specular,Texcoord).xyz;

    vec3 result=diffuse+ambient+specular;
    FragColor=vec4(result,1.0f);
}