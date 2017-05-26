#pragma once

#include <gl\glew.h>
#include <gl\freeglut.h>

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class ShaderController {

	struct shaderObj 
	{
		int usedBy;
		GLuint vertex_shader, fragment_shader, shaderProg;
		shaderObj(GLuint vshader, GLuint fshader, GLuint shaderProg) : 
			vertex_shader(vshader), fragment_shader(fshader), shaderProg(shaderProg),usedBy(0)
		{}

		shaderObj() :
			usedBy(0)
		{}
	};

	static std::unordered_map<std::string, shaderObj> shaders;
	std::string name;


	std::string readFile(std::string fileName)
	{
		std::ifstream file(fileName);
		if (!file.is_open())
			throw "Lol wrong filename";

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	GLuint compile(GLuint type, std::string fileName)
	{
		std::string source = readFile(fileName);
		const char * c_source = source.c_str();
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &c_source, NULL);
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::string log(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &log[0]);
			std::cout << log;
			throw std::logic_error(log);
		}
		return shader;
	}
public:
	ShaderController(std::string fileName_V, std::string fileName_F)
	{
		name = fileName_V + fileName_F;
		if (shaders.find(name) == shaders.end())
		{
			GLuint vertex_shader = compile(GL_VERTEX_SHADER, fileName_V);
			GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, fileName_F);
			GLuint prog = glCreateProgram();
			glAttachShader(prog, vertex_shader);
			glAttachShader(prog, fragment_shader);
			glLinkProgram(prog);
			shaders.insert({ name, shaderObj(vertex_shader,fragment_shader,prog)});
		}
	}

	operator GLuint() { return shaders[name].shaderProg; }
	void operator()() { glUseProgram(shaders[name].shaderProg); }

	~ShaderController() 
	{
		if (shaders[name].usedBy == 1)
		{
			glDeleteProgram(shaders[name].shaderProg);
			glDeleteShader(shaders[name].vertex_shader);
			glDeleteShader(shaders[name].fragment_shader);
			shaders.erase(name);
		}
		else
			shaders[name].usedBy--;
	}
};


std::unordered_map<std::string, ShaderController::shaderObj> ShaderController::shaders;