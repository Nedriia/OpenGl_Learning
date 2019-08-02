#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
public:
	//Program ID
	unsigned int ID;
	//Glchar -> is a type similar to the C char, which serves to represent a narrow character
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//use/activate shader
	void Use();
	//Utility Uniform functions
	void setBool(const std::string &name, bool value)const;
	void setInt(const std::string &name, int value)const;
	void setFloat(const std::string &name, float value)const;
};

#endif