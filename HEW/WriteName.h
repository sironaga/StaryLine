#pragma once

#include "DirectWriteCustomFont.h"

class CWriteName
{
public:
	CWriteName();
	~CWriteName();
	void Update();
	void Draw();
	static std::string GetPlayerName();
	void SetPlayerName(std::string name);
private:
	DirectWriteCustomFont* m_pWrite;
	FontData data;
	static std::string m_Name;
};