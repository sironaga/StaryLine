//SceneGame.h
//作成者：AT12B浦口龍輝

//開発履歴 
//10/31プロトタイプ宣言の追加


#pragma once
#include"Main.h"
#include "Character.h"
#include "StageSelect.h"
#include "Field.h"
#include "SoundList.h"
#include "BackGround.h"
#include "FieldVertex.h"
#include "Player.h"
#include "Battle.h"
#include "Scene.h"
#include "StarLine.h"

class CSceneGame : public CScene
{
public:
	CSceneGame(StageType StageNum);
	virtual ~CSceneGame();
	virtual void Update()override;
	virtual void Draw()override;

    void SetMasterVolume() override;
	void SetResult(bool InWin);
	void InitResolusion(float wide, float height, bool fullscreen) override;
private:

	CFieldVertex* m_pFieldVertex;
	CPlayer* m_pPlayer;
	CBattle* m_pBattle;
	Field* m_pField;
	CBackGround* m_pBackGround;
	CEffectManager* m_pEffect;
	IXAudio2SourceVoice* m_pSourseGameBGM;
	CSoundList* g_GameSound;
	IXAudio2SourceVoice* m_pSourseFeverBGM;
	CSoundList* g_FeverSound;
	StarLine* m_pStarLine;

	bool m_bPhase;
	bool m_bEnd;
	
};

bool GetFeverMode();
bool GetTimeStart();
float GetTime();