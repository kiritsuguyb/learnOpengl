#pragma once
#include <glad.h>
#include <glfw3.h>
#include <vector>

struct layoutElement {
	GLenum type;
	unsigned int size;
	unsigned int count;
};
class VertexLayout
{
public:
	VertexLayout() {
		stride = 0;
	};
	template<typename T>
	void push(unsigned int count) {
		stride += count * sizeof(T);
		layout.push_back({ GL_FLOAT, sizeof(T), count });
	}
	template<>
	void push<float>(unsigned int count) {
		stride += count * sizeof(float);
		layout.push_back({ GL_FLOAT, sizeof(float), count });
	}

	void use();
private:
	std::vector<layoutElement> layout;
	unsigned int stride;
	
};

