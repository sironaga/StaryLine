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

//プロトタイプ宣言
class CSceneTitle : public CScene
{
public:
	CSceneTitle();
	virtual ~CSceneTitle();
	virtual void Update()override;
	virtual void Draw()override;
private:
	IXAudio2SourceVoice* g_pSourseTitleBGM;
	CSoundList* g_TitleSound;
	CBackGround* g_pTitleBG;
	float f_SelectY;
	COption* m_pOption;
	POLYGON_BUFFER* m_vSelect;
	POLYGON_TEXTURE* m_tSelect;
};