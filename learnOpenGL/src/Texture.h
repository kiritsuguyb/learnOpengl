#pragma once
#include <iostream>
#include "stb_image.h"
#include <glad.h>

class Texture
{
public:
	Texture(const char* textureFilename,GLenum _textureUnit);
	~Texture();
	void Bind();
	void UnBind();
private:
	unsigned int ID;
	GLenum textureUnit;
	int width, height, nrComponent;
	stbi_uc* bytes;
};

