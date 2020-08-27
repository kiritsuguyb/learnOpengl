#pragma once
#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader
{
public:
	unsigned int ID;
	mutable std::unordered_map<std::string, GLint> locationmap;

	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* shaderPath);
	Shader() {};

	void use()const;

	void setBool(const std::string &name,bool value)const;
	void setInt(const std::string &name, int value)const;
	void setFloat(const std::string &name, float value)const;
	void setMatrix(const std::string &name, const GLfloat* value)const;

	GLint getUniformLocation(const std::string &name)const;
private:
	void build_from_source(const char* &vertexshadersource, const char* &fragmentshadersource);
};
