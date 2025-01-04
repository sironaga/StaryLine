#pragma once
/*
* 2025/01/04 
*/

// -- Include
#include"SpriteEx.h"



class CResult
{
public:
	CResult();
	~CResult();
	void Update();
	void Disp();
	static void InGameData(ResultGameInfo InData);
private:
	SpriteEx* m_pBackGround;
};

