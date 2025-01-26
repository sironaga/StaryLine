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
	void TitleAnimation();
	void OptionApply();
private:
	IXAudio2SourceVoice* g_pSourseTitleBGM;
	CSoundList* g_TitleSound;
	CBackGround* g_pTitleBG;
	COption* m_pOption;
	CSoundList* m_pSelectsound;
	IXAudio2SourceVoice* m_pSourseSelectSE;
private:
	SpriteEx* m_pTitleLogo;
	SpriteEx* m_pTitleBack;
	SpriteEx* m_pLini[2];
	SpriteEx* m_pTitleFrame;
	SpriteEx* m_pTitleUnderbar;
	SpriteEx* m_pTitleStart[2];
	SpriteEx* m_pTitleContinued[2];
	SpriteEx* m_pTitleOption[2];
	SpriteEx* m_pTitleEnd[2];
	SpriteEx* m_pDecition;
	DirectX::XMFLOAT2 m_SelectPos;
	DirectX::XMFLOAT2 m_DecisionPos;
	DirectX::XMFLOAT2 m_tLiniPos;
	DirectX::XMFLOAT2 m_tTabPos[4];
	DirectX::XMFLOAT2 m_tTabSize[4];
	DirectX::XMFLOAT2 m_tStarPos;
	float m_fSelectScale;
	bool m_bSelected;
	bool m_bChange;
	int m_nAnimCount;
	int m_nLiniYCount;
	DirectX::XMFLOAT2 m_tCharaLogoTexPos[2];
	enum class Effect
	{
		Star = 0,
		Choice,

		Max
	};
	CEffectManager_sp* m_pEffect[(int)Effect::Max];
};