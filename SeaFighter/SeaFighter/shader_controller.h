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
		GLuint vertex_shader, fragment_shader, shaderProgram;
		shaderObj(GLuint vshader, GLuint fshader, GLuint shaderProg) : 
			vertex_shader(vshader), fragment_shader(fshader), shaderProgram(shaderProg),usedBy(1)
		{}

		shaderObj() :
			usedBy(0)
		{}
	};

	static std::unordered_map<std::string, shaderObj> shaders;
	static std::string currentlyUsedName;

	std::string name;


	std::string readFile(std::string fileName)
	{
		fileName = "shaders/" + fileName;
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
		glShaderSource(shader, 1, &c_source, nullptr);
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::string log(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &log[0]);
			std::cout <<fileName << " " << log;
			throw std::logic_error(log);
		}
		return shader;
	}
public:
	ShaderController(std::string fileName_V, std::string fileName_F)
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL errorbara1: " << err << gluErrorString(err) << std::endl;
		}
		name = fileName_V + fileName_F;
		if (shaders.find(name) == shaders.end())
		{
			GLuint prog = glCreateProgram();
			GLuint vertex_shader = compile(GL_VERTEX_SHADER, fileName_V);
			GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, fileName_F);
			glAttachShader(prog, vertex_shader);
			glAttachShader(prog, fragment_shader);
			glLinkProgram(prog);
			GLint linkSuccessful;
			glGetProgramiv(prog, GL_LINK_STATUS, &linkSuccessful);

			// If it didn't, then read and print the link log
			if (!linkSuccessful) {
				GLint logLength;
				glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);

				std::vector<GLchar> logBuffer(logLength);
				glGetProgramInfoLog(prog, logLength, nullptr, logBuffer.data());

				std::cout << logBuffer.data();
				throw std::logic_error(logBuffer.data());
			}
			shaders.insert({ name, shaderObj(vertex_shader,fragment_shader,prog) });
		}
		else
			shaders[name].usedBy++;
	}

	operator GLuint() { return shaders[name].shaderProgram; }
	void operator()() { glUseProgram(shaders[name].shaderProgram);}

	~ShaderController() 
	{
		if (shaders[name].usedBy == 1)
		{
			glDeleteProgram(shaders[name].shaderProgram);
			glDeleteShader(shaders[name].vertex_shader);
			glDeleteShader(shaders[name].fragment_shader);
			shaders.erase(name);
		}
		else
			shaders[name].usedBy--;
	}
};


std::unordered_map<std::string, ShaderController::shaderObj> ShaderController::shaders;