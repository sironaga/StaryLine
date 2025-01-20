
#pragma once

#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Scene.h"
#include "SpriteEX.h"
#include "ModelEx.h"
#include  "_StructList.h"
#include "BackGround.h"

enum E_SELECT_STAGETYPE
{
	GRASSLAND = 0,
	DESERT,
	SNOWFIELD
};
enum E_SELECT_STAGENUMBER
{
	GRASSLAND_STAGE1 = 0,
	GRASSLAND_STAGE2,
	GRASSLAND_STAGE3,
	DESERT_STAGE1,
	DESERT_STAGE2,
	DESERT_STAGE3,
	SNOWFIELD_STAGE1,
	SNOWFIELD_STAGE2,
	SNOWFIELD_STAGE3
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

	enum StageKind
	{
		GrassField = 0,
		DesertField,
		SnowField,
		WorldField,

		StageKindMax
	};

	CModelEx* m_pModel[StageKindMax];
	ObjectParam m_ModelParam[StageKindMax];
	ObjectParam m_ModelSubParam[StageKindMax];
	CBackGround* m_pBackGround;

	float f_Rotation;
	float f_Rad;
	float f_SelectX;
	float f_SelectY;
	float posX[3];
	bool MainStage;
	bool m_bEnd;
	bool m_bMoving;

	int nSlect;
};
