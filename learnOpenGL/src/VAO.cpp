#include "VAO.h"

VAO::VAO(const char* _shaderPath, const VertexLayout& _vertexlayout)
{
	shader = Shader(_shaderPath);
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
	vertexlayout = _vertexlayout;
}

VAO::VAO(const Shader & _shader, const VertexLayout& _vertexlayout)
{
	shader = _shader;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
	vertexlayout = _vertexlayout;
}

void VAO::bind() {
	glBindVertexArray(ID);
	vertexlayout.use();
	shader.use();
}


Shader VAO::useShader()const
{
	shader.use();
	return shader;
}
