#include "Texture.h"

Texture::Texture(const char * textureFilename, GLenum _textureUnit)
{
	glGenTextures(1, &ID);
	textureUnit = _textureUnit;
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	bytes = stbi_load(textureFilename, &width, &height, &nrComponent, 0);
	if (bytes)
	{
		std::cout << "imageload success!" << std::endl;
		switch (nrComponent)
		{
		case 0:break;
		case 1:break;
		case 2:break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
			break;
		default:
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Image import failure. \nFailure reason: " << stbi_failure_reason() << std::endl;
	}
	
}

Texture::~Texture()
{
	UnBind();
	stbi_image_free(bytes);
}

void Texture::Bind()
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::UnBind()
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
