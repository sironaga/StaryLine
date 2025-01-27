
#pragma once

#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Scene.h"
#include "SpriteEX.h"
#include "ModelEx.h"
#include  "_StructList.h"
#include "BackGround.h"
#include "SoundList.h"
#include "EffectManager_sp.h"

#define SELECT_MAX_STAR 4

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
	void SelectAnimation();

	SpriteEx* m_pGrassLand;
	SpriteEx* m_pDesert;
	SpriteEx* m_pSnowField;
	SpriteEx* m_pGrassLandStage[3];
	SpriteEx* m_pDesertStage[3];
	SpriteEx* m_pSnowFieldStage[3];
	SpriteEx* m_pLock;
	SpriteEx* m_pRight_Select;
	SpriteEx* m_pLeft_Select;
	SpriteEx* m_pStageSelected;
	SpriteEx* m_pStageSelect_Underbar;
	SpriteEx* m_pStageSelect;
	SpriteEx* m_pWorldSelect;
	SpriteEx* m_pWorld[3];
	SpriteEx* m_pDecition;

	IXAudio2SourceVoice* m_pSourseStageSelectBGM;
	CSoundList* g_StageSelectSound;

	DirectX::XMFLOAT3 m_rotate;
		enum StageKind
	{
		GrassField = 0,
		DesertField,
		SnowField,

		StageKindMax
	};

	CModelEx* m_pModel[StageKindMax];
	Model* m_pWorldModel;
	ObjectParam m_ModelWorldParam;
	ObjectParam m_ModelParam[StageKindMax];
	ObjectParam m_ModelSubParam[StageKindMax];
	CBackGround* m_pBackGround;


	enum class Effect
	{
		Star,

		Max
	};
	CEffectManager_sp* m_pEffect[(int)Effect::Max];
	CEffectManager_sp* m_pStarEfc[SELECT_MAX_STAR];

	float f_Rotation;
	float f_Rad;
	float f_SelectX;
	float f_SelectY;
	float posX[3];
	float subposX[3];
	DirectX::XMFLOAT2 m_tDecitionPos[2];
	float m_tBGRotateZ;
	bool MainStage;
	bool m_bEnd;
	bool m_bMoving;

	int nSlect;
};
