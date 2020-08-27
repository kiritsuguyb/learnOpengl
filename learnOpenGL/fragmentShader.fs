#version 330 core
in vec3 ourColor;
out vec4 FragColor;
void main(){
float max=ourColor.r>ourColor.g?ourColor.r:ourColor.g;
max=max>ourColor.b?max:ourColor.b;
FragColor=vec4(ourColor.r/max,ourColor.g/max,ourColor.b/max,1.0f);
}