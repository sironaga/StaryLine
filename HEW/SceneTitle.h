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

//プロトタイプ宣言
class CSceneTitle : public CScene
{
public:
	CSceneTitle();
	virtual ~CSceneTitle();
	virtual void Update()override;
	virtual void Draw()override;
private:
	void SetResolusion(float wide,float height,bool fullscreen);
private:
	IXAudio2SourceVoice* g_pSourseTitleBGM;
	CSoundList* g_TitleSound;
	CBackGround* g_pTitleBG;
	COption* m_pOption;

private:
	SpriteEx* m_pTitleLogo;
	SpriteEx* m_pTitleBack;
	SpriteEx* m_pLini;
	SpriteEx* m_pTitleFrame;
	SpriteEx* m_pTitleUnderbar;
	SpriteEx* m_pTitleStart[2];
	SpriteEx* m_pTitleContinued[2];
	SpriteEx* m_pTitleOption[2];
	SpriteEx* m_pTitleEnd[2];
	DirectX::XMFLOAT2 m_SelectPos;
	bool m_bSelected;
};