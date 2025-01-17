
#pragma once

#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Scene.h"
#include "SpriteEX.h"

enum E_SELECT_STAGETYPE
{
	GRASSLAND = 0,
	DESERT,
	SNOWFIELD
};
enum E_SELECT_STAGENUMBER
{
	STAGE1 = 0,
	STAGE2,
	STAGE3
};

class CStageSelect : public CScene
{
public:
	CStageSelect();
	virtual ~CStageSelect();
	virtual void Update() override;
	virtual void Draw()override;

	//int GetStageNum();
private:
	SpriteEx* m_pGrassLand;
	SpriteEx* m_pDesert;
	SpriteEx* m_pSnowField;
	SpriteEx* m_pGrassLandStage[3];
	SpriteEx* m_pDesertStage[3];
	SpriteEx* m_pSnowFieldStage[3];
	SpriteEx* m_pRight_Select;
	SpriteEx* m_pLeft_Select;
	float f_Rotation;
	float f_Rad;
	float f_SelectX;
	float f_SelectY;
	bool MainStage;

	int nSlect;
};
