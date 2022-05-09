#pragma once
#include "GL/glew.h"
#include <string>
#include "CMath.h"

class Shader {
public:
	Shader();
	~Shader();

	bool Load(const std::string& vertName, const std::string& fragName);
	void UnLoad();
	void SetActive();
	//Los shaders tienen un tipo de variables que se llaman Uniforms que una vez que las definimos se quedan ene memoria
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
private:
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader);
	//Preguntar si los shaders de vertices  fragmentos se enlazaron correctamente
	bool IsValidProgram();

	//ID'S de shaders
	GLuint m_VertexShader;
	GLuint m_FragShader;
	GLuint m_ShaderProgram;
};