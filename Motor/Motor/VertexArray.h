#pragma once

class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// Activar este Vertex Array para poder dibujar sobre él
	void SetActive();

	unsigned int GetNumIndices() const { return m_NumIndices; }
	unsigned int GetNumVerts() const { return m_NumVerts; }
private:
	// Número de vértices en el búfer de vértices
	unsigned int m_NumVerts;
	// Número de índices en el búfer de índices
	unsigned int m_NumIndices;
	// OpenGL IDs (OpenGL no regresa pointers, sino IDs de objetos)
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;
	unsigned int m_VertexArray;
};