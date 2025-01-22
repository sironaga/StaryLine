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

	void NumberDisp(void);
private:
	// --- èüîsä÷åWñ≥ÇµÇ…ïœÇÌÇÁÇ»Ç¢Ç‚Ç¬
	SpriteEx* m_pHitPoint;
	SpriteEx* m_pSelect[2];
	SpriteEx* m_pShadow;
	SpriteEx* m_pStageSelect[2];
	SpriteEx* m_pSummonData;
	SpriteEx* m_pUnderBar;
	SpriteEx* m_pBack;
	// --- ïœÇÌÇÈÇ‚Ç¬
	SpriteEx* m_pText;
	SpriteEx* m_pCharacter;
	SpriteEx* m_pLighting;
	SpriteEx* m_pTextShadow;
	SpriteEx* m_pClearTime;
	SpriteEx* m_pNextUI[2];
	SpriteEx* m_pStar;
	SpriteEx* m_pScore;
	SpriteEx* m_pBestScore;

	// --- ïœêî
	int nAnimationTimer;
	int nAnimationFrame;
	bool bAnimation;

	int nCAnimation;
	bool bCAnimation;

	bool bScore;
	bool bBestScore;

	int nSelect; // --- KeyBoardÇÃì¸óÕîªíËóp

	float CScle;
	float nCount;
	float fStarAngle;
	float CPosY;
	float StarPosY;
	float LogoAngle;
	float fTime;
	float fCTime;




	CSoundList* m_pResultSound;
	IXAudio2SourceVoice* m_pResultBGM;
	CSoundList* m_pResultSelectSound;
	IXAudio2SourceVoice* m_pResultSelectSE;
	CNumberUI* m_pNumber;

	static StageType StageLevel; 
	static ResultGameInfo ResultGameData;
	static int nBestScore[8];
};