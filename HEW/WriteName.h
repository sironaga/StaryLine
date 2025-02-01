#pragma once

#include "DirectWriteCustomFont.h"

class CWriteName
{
public:
	CWriteName();
	~CWriteName();
	void Update();
	void Draw();
private:
	DirectWriteCustomFont* m_pWrite;
	FontData data;
	std::string m_Name;
};