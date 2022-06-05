#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

string get_file_contents(const char* filename);

class Shader 
{
	public:
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentFile);

		void shaderActivate();
		void shaderDelete();
};

#endif SHADER_CLASS_H