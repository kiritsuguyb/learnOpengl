#pragma once
#include "Shader.h"
#include "VertexLayout.h"
class VAO
{
public:
	VAO(const char* _shaderPath,const VertexLayout& _vertexlayout);
	VAO(const Shader &_shader, const VertexLayout& _vertexlayout);
	void bind();
	Shader useShader()const;

private:
	unsigned int ID;
	Shader shader;
	VertexLayout vertexlayout;//vao����layout��ʹ�õ�ʱ������ѹջ
};
