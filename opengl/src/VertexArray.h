#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vbo,VertexBufferLayout& layout);
	void Bind() const;
	void UnBind() const;

};