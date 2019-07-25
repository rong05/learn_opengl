
//#include<glad/glad.h>
// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm-master/glm/glm.hpp>

#include<string>
#include<sstream>
#include<fstream>
#include<iostream>

#ifndef _SHADER_H_
#define _SHADER_H_

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath,const char* fragmentPath,const char* geometryPath = nullptr);
	~Shader();


	//activate the shader
	void use();
	//utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name,int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec2(const std::string& name, const glm::vec2 &value) const;
	void setVec3(const std::string& name, const glm::vec3 &value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4 &value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2 &mat) const;
	void setMat3(const std::string& name, const glm::mat3 &mat) const;
	void setMat4(const std::string& name, const glm::mat4 &mat) const;
private:
	void checkCompileErrors(unsigned int shader,std::string type);
};
#endif