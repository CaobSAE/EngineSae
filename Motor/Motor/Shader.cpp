#include "Shader.h"
#include <SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader() :
	m_ShaderProgram(0),
	m_VertexShader(0),
	m_FragShader(0)
{
}

Shader::~Shader(){

}

bool Shader::Load(const std::string& vertName, const std::string& fragName){
	if (!CompileShader(vertName, GL_VERTEX_SHADER, m_VertexShader) || !CompileShader(fragName, GL_FRAGMENT_SHADER, m_FragShader)) {
		return false;
	}
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragShader);
	glLinkProgram(m_ShaderProgram);

	if (!IsValidProgram()) {
		return false;
	}

	return true;
}

void Shader::UnLoad(){
	glDeleteProgram(m_ShaderProgram);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragShader);
}

void Shader::SetActive(){
	glUseProgram(m_ShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix){
	GLuint loc = glGetUniformLocation(m_ShaderProgram, name);
	glUniformMatrix4fv(
		loc,					// Uniform ID
		1,						// Número de matrices
		GL_TRUE,				// TRUE si estamos utilizando vectores de fila
		matrix.GetAsFloatPtr()	//	Pointer a los datos de la matriz
	);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader){
	//Abrir archivo
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open()) {
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		//Crear el shader
		outShader = glCreateShader(shaderType);
		//Agregar el contenido e intetar compilar
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader)) {
			SDL_Log("Failed to compile shader %s", fileName.c_str());
		}
	}else {
		SDL_Log("Shader file not found %s", fileName.c_str());
		return false;
	}
	return true;
}

bool Shader::IsCompiled(GLuint shader){
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 512, nullptr, buffer);
		SDL_Log("GLSL Compile Failed: %s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram(){
	GLint status;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, buffer);
		SDL_Log("GLSL Link status: %s", buffer);
		return false;
	}

	return true;
}
