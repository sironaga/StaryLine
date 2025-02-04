
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
	SNOWFIELD_STAGE3,

	MAX_STAGE
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

	void LinieDraw();
	void QrackerDraw();
	void NugarDraw();
	void BoldowDraw();
	void KanneleDraw();
	void SelectAnimation();
	void ClearCheck();
	bool ClearCommand(void);

	SpriteEx* m_pGrassLand;
	SpriteEx* m_pDesert;
	SpriteEx* m_pSnowField;
	SpriteEx* m_pGrassLandStage[3];
	SpriteEx* m_pDesertStage[3];
	SpriteEx* m_pSnowFieldStage[3];
	SpriteEx* m_pRight_Select;
	SpriteEx* m_pRight_Select_Lock;
	SpriteEx* m_pLeft_Select;
	SpriteEx* m_pLeft_Select_Lock;
	SpriteEx* m_pStageSelected;
	SpriteEx* m_pStageSelect_Underbar;
	SpriteEx* m_pWorldSelect_Backboard;
	SpriteEx* m_pWorld[3];
	SpriteEx* m_pDecition;
	SpriteEx* m_pRank_C;
	SpriteEx* m_pRank_B;
	SpriteEx* m_pRank_A;
	SpriteEx* m_pRank_S;

	IXAudio2SourceVoice* m_pSourseStageSelectBGM;
	CSoundList* m_BGMSound;
	IXAudio2SourceVoice* m_pSourseStageSelectSE[4];
	CSoundList* m_pSESound[4];
	enum Sound
	{
		DECISION_SE
		, GRASS_SE
		, DESERT_SE
		, SNOW_SE
	};

	DirectX::XMFLOAT3 m_rotate;
		enum StageKind
	{
		GrassField = 0,
		DesertField,
		SnowField,

		StageKindMax
	};

	CModelEx* m_pModel[StageKindMax];
	Model* m_pStageLinie;
	Model* m_pStageQracker;
	Model* m_pStageNugar;
	Model* m_pStageBoldow;
	Model* m_pStageKannele;
	Model* m_pWorldModel;
	ObjectParam m_ModelWorldParam;
	ObjectParam m_ModelParam[StageKindMax];
	ObjectParam m_ModelSubParam[StageKindMax];
	CBackGround* m_pBackGround;

	WORD m_Direction;

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
	float LinieRotationY;
	DirectX::XMFLOAT2 m_tDecitionPos[2];
	float m_tBGRotateZ;
	bool MainStage;
	bool m_bEnd;
	bool m_bMoving;
	bool m_bClear[MAX_STAGE];
	bool m_bCantMove_Left;
	bool m_bCantMove_Right;
	float m_fTime;
	bool m_bClearCommand[5];
	float m_fClearCommandResetTimer;

	int nSlect;
};
