#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//1.获取shader数据
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
#define vin vShaderFile
	std::ifstream fShaderFile;
#define fin fShaderFile

	// ensure ifstream objects can throw exceptions:
	vin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vin.open(vertexPath);
		fin.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vin.rdbuf();
		fShaderStream << fin.rdbuf();

		vin.close();
		fin.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	build_from_source(vShaderCode, fShaderCode);
}

Shader::Shader(const char * shaderPath)
{
	//获取两个文件的字符串
	//获取完整字符串
	//打开文件
	std::ifstream shaderStream(shaderPath);
#define sin shaderStream
	std::string lineString;
	std::string finalString[2];
	int Itype = -1;
	while (getline(sin, lineString)) {
		if (lineString.find("#pragma") != std::string::npos)
		{
			Itype = (int)lineString.find("vertex") == std::string::npos;
		}
		else {
			finalString[Itype] += lineString;
			finalString[Itype] += "\n";
		}
	}

	std::cout << "Vertex!!!!!\n" << finalString[0] << std::endl;
	std::cout << "Fragment!!!!!\n" << finalString[1] << std::endl;

	const char* vertexshadersource = finalString[0].c_str();
	const char* fragmentshadersource = finalString[1].c_str();
	build_from_source(vertexshadersource, fragmentshadersource);
}

void Shader::use()const
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(getUniformLocation(name),value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setMatrix(const std::string & name, const GLfloat * value) const
{
	glUniformMatrix4fv(getUniformLocation(name),1,GL_FALSE,value);
}

void Shader::setVec3(const std::string & name, float x, float y, float z) const
{
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setVec3(const std::string & name, const GLfloat * value) const
{
	glUniform3fv(getUniformLocation(name),1,value);
}

void Shader::setVec4(const std::string & name, float x, float y, float z, float w) const
{
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setVec4(const std::string & name, const GLfloat * value) const
{
	glUniform3fv(getUniformLocation(name),1,value);
}

GLint Shader::getUniformLocation(const std::string &name)const
{
	if (locationmap.find(name) != locationmap.end())
		return locationmap[name];
	return locationmap[name] = glGetUniformLocation(ID, name.c_str());
}

void Shader::build_from_source(const char * &vertexshadersource, const char * &fragmentshadersource)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexshadersource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentshadersource, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	GLint success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cerr << "ERROR::COMPILE::SHADER::VERTEX: " << infolog << std::endl;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cerr << "ERROR::COMPILE::SHADER::FRAGMENT: " << infolog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infolog);
		std::cerr << "ERROR::LINKING::SHADERPROGRAM: " << infolog << std::endl;
	}
}
