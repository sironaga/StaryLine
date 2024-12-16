#pragma once

#define _DIRECTX_
#include"DirectX.h"

enum E_TITLE_TYPE
{
	GAMESTART = 0,
	GAMECONTINUE,
	GAMEOPTION,
	GAMEEND,
};

class CStartTitle
{
public:
	CStartTitle();
	~CStartTitle();

	void UpdateStartTitle();
	void DrawStartTitle();



private:
	POLYGON_BUFFER*		m_vBackGround;
	POLYGON_TEXTURE*	m_tBackGround;
	POLYGON_BUFFER*		m_vLogo;
	POLYGON_TEXTURE*	m_tLogo;
	POLYGON_BUFFER*		m_vE;
	POLYGON_BUFFER*		m_vACS;
	POLYGON_TEXTURE*	m_tAccessories[10];
	POLYGON_BUFFER*		m_vSelect;
	POLYGON_TEXTURE*	m_tSelect;
	float f_Rotation;
	float f_Rad;
	float f_SelectY;

};


