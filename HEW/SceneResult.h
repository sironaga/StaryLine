#pragma once

#include "Scene.h"
#include"_StructList.h"
#include"SpriteEx.h"
#include"Main.h"
#include"NumberUI.h"
#include "SoundList.h"

class CSceneResult : public CScene
{
public:
	CSceneResult();
	virtual ~CSceneResult();
	virtual void Update()override;
	virtual void Draw()override;

	static void InResultData(ResultGameInfo InData);
	static void InStageLevel(StageType InLevel);
	static void InBestScore(int InScore, StageType InStage);
	static int OutBestScore(StageType InStage);
private:
	void LoadTexture(void);
	void DefaultSetPos(void);
	void KeyProsess(void);
	void AnimationManagemer(void);
	void SetCamData(void);


	void WinAnimation(void);
	void LoseAnimation(void);
	void WinDisp(void);
	void LoseDisp(void);


	void NumberDisp(void);
private:

	SpriteEx* m_pHitPoint;
	SpriteEx* m_pSelect;
	SpriteEx* m_pShadow;
	SpriteEx* m_pStageSelect[2];
	SpriteEx* m_pSummonData;
	SpriteEx* m_pUnderBar;
	SpriteEx* m_pBack;
	SpriteEx* m_pText;
	SpriteEx* m_pCharacter;
	SpriteEx* m_pLighting;
	SpriteEx* m_pTextShadow;
	SpriteEx* m_pClearTime;
	SpriteEx* m_pNextUI[2];
	SpriteEx* m_pStar;
	SpriteEx* m_pScore;
	SpriteEx* m_pBestScore;
	CNumberUI* m_pNumber;

	// --- ïœêî
	int nAnimationTimer;
	int nAnimationFrame;
	bool bWorldClear;
	bool bScore;
	bool bBestScore;
	int nSelect; 

	
	int nPush[2]; // É{É^ÉìÇâüÇµÇΩéû



	CSoundList* m_pResultSound;
	IXAudio2SourceVoice* m_pResultBGM;
	CSoundList* m_pResultSelectSound;
	IXAudio2SourceVoice* m_pResultSelectSE;


	static StageType StageLevel; 
	static ResultGameInfo ResultGameData;
	static int nBestScore[8];
	static bool bClearState[8];
};