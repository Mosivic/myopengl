#pragma once
#include "Debug.h"


class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Heigh, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeigh() const { return m_Heigh; }
};


