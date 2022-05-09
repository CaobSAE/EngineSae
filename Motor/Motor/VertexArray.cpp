#include "VertexArray.h"
#include "GL/glew.h"

VertexArray::VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices) :
	m_NumVerts(numVerts),
	m_NumIndices(numIndices)
{
	// Primero, genero el VertexArray (guardo el espacio en la memoria) y después le asigno un ID que significa que el VertexArray está activo
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	// Una vez creado el búfer, tenemos que copiar verts al búfer
	glBufferData(
		GL_ARRAY_BUFFER,				// El tipo de búfer al que estamos escribiendo
		numVerts * 3 * sizeof(float),	// El tamaño de los bytes que vamos a copiar
		verts,							// Pointer a la fuente de los datos
		GL_STATIC_DRAW					// Cómo vamos a utilizar estos datos
	);

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		numIndices * sizeof(unsigned int),
		indices,
		GL_STATIC_DRAW
	);

	// Vamos a definir un primer atributo que es la posición
	// Activar el primer componente de este array
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// Índice del atributo (0 para el primero)
		3,					// Cuántos componentes tiene este atributo (en este caso son 3, para la posición x, y, z)
		GL_FLOAT,			// Tipo de componentes
		GL_FALSE,			// 
		sizeof(float) * 3,	// Stride (usualmente el tamaño de cada vértice, a menos que tengamos padding)
		0					//Offset desde el principio del vertex array a este atributo
	);
}

VertexArray::~VertexArray(){
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteVertexArrays(1, &m_VertexArray);
}

void VertexArray::SetActive(){
	glBindVertexArray(m_VertexArray);
}
