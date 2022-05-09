#pragma once

class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// Activar este Vertex Array para poder dibujar sobre �l
	void SetActive();

	unsigned int GetNumIndices() const { return m_NumIndices; }
	unsigned int GetNumVerts() const { return m_NumVerts; }
private:
	// N�mero de v�rtices en el b�fer de v�rtices
	unsigned int m_NumVerts;
	// N�mero de �ndices en el b�fer de �ndices
	unsigned int m_NumIndices;
	// OpenGL IDs (OpenGL no regresa pointers, sino IDs de objetos)
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;
	unsigned int m_VertexArray;
};