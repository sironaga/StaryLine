//SceneTitle.h
//作成者：AT12B浦口龍輝

//開発履歴 
//10/31プロトタイプ宣言の追加

#pragma once

#include <d3d11.h>
#include "SoundList.h"
#include "BackGround.h"
#include "Scene.h"
#include "Option.h"
#include "Texture.h"
#include "SpriteEx.h"
#include "_StructList.h"
#include "EffectManager_sp.h"

#define MAX_STAR 4

//プロトタイプ宣言
class CSceneTitle : public CScene
{
public:
	CSceneTitle(COption*pOption);
	virtual ~CSceneTitle();
	virtual void Update()override;
	virtual void Draw()override;
private:
	void SetResolusion(float wide, float height, bool fullscreen);
private:
	void TitleAnimation();
	void EffectUpdate();
	enum class Effect
	{
		Star = 0,
		Choice,

		Max
	};
	CEffectManager_sp* m_pEffect[(int)Effect::Max];
	CEffectManager_sp* m_pStarEfc[MAX_STAR];
	DirectX::XMFLOAT2 m_tStarPos[MAX_STAR];
	DirectX::XMFLOAT3 m_tStarRotate[MAX_STAR];
	int m_nAnimCount;
	float m_fAnimTime;
	enum class TitleAnim
	{
		Hat,
		Logo,
		LinieBack,
		Linie,
		UnderBar,

		Max
	};
	SpriteEx* m_pAnimSprite[(int)TitleAnim::Max];
	ObjectParam m_AnimParam[(int)TitleAnim::Max];
private:
	void TitleInput();
	SpriteEx* m_pTitleBack;
	SpriteEx* m_pTitleFrame;
	SpriteEx* m_pTitleStart[2];
	SpriteEx* m_pTitleContinued[2];
	SpriteEx* m_pTitleOption[2];
	SpriteEx* m_pTitleEnd[2];
	SpriteEx* m_pDecition;
	DirectX::XMFLOAT2 m_SelectPos;
	DirectX::XMFLOAT2 m_DecisionPos;
	float m_fTabInitPosY;
	DirectX::XMFLOAT2 m_tTabPos[4];
	DirectX::XMFLOAT2 m_tTabSize[4];
	WORD m_Direction;
	float m_fSelectScale;
	bool m_bSelected;
	bool m_bChange;
	int m_nLiniYCount;
	DirectX::XMFLOAT2 m_tCharaLogoTexPos[2];

private:
	void OptionApply();
	COption* m_pOption;

private:
	IXAudio2SourceVoice* g_pSourseTitleBGM;
	CSoundList* g_TitleSound;
	CSoundList* m_pSelectsound;
	IXAudio2SourceVoice* m_pSourseSelectSE;

private:
	CBackGround* g_pTitleBG;

private:
	static bool m_bFirstPlay;
public:
	static bool IsFirstPlay();
	static void SetPlayed();
	
private:
	bool CheckRankingCommand();
	bool m_bRankingCommand[4];
	float m_fRankingCommandResetTimer;
};