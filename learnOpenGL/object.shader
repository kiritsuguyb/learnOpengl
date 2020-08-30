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
    float intensity;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Light_Point{
    float intensity;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float k;
    float minIntensity;
    float maxDistance;
};
struct Light_Spot{
    vec3 direction;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float cutoff;
    float outterCutoff;
    float k;
    float minIntensity;
    float maxDistance;
};

out vec4 FragColor;

in vec2 Texcoord;
in vec3 Normal;
in vec3 worldPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light_Spot light;

void main(){
    float lightdist=length(light.position-worldPos);
    float attenuation=1.0f/(1.0f+light.k*pow(lightdist,2.0f))/(1-light.minIntensity)-light.minIntensity;
    vec3 ambient=light.ambient*texture(material.diffuse,Texcoord).xyz;
    
    vec3 lightDir=light.position-worldPos;
    lightDir=normalize(lightDir);
    float diff=max(dot(lightDir,Normal),0.0f);
    vec3 diffuse=diff*light.diffuse*texture(material.diffuse,Texcoord).xyz;

    vec3 reflectDir=reflect(-lightDir,Normal);
    vec3 viewDir=normalize(viewPos-worldPos);
    float spec=pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
    vec3 specular=spec*light.specular*texture(material.specular,Texcoord).xyz;
    
    ambient  *= attenuation*light.intensity; 
    diffuse  *= attenuation*light.intensity;
    specular *= attenuation*light.intensity; 
    float theta=dot(normalize(light.direction),-lightDir);
    float cutoff_atten=clamp((theta-light.outterCutoff)/(light.cutoff-light.outterCutoff),0.0f,1.0f);
    vec3 result=ambient;
    result+=(diffuse+specular)*cutoff_atten;
    FragColor=vec4(result,1.0f);
}