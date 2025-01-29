#pragma once
// --- Include
#include "Scene.h"
#include"_StructList.h"
#include"SpriteEx.h"
#include"Main.h"
#include"NumberUI.h"
#include "SoundList.h"
#include"Conection_Sys.h"

class CSceneResult : public CScene
{
public:
	// --- 初期化系
	CSceneResult();
	virtual ~CSceneResult();

	// --- 更新処理
	virtual void Update()override;
	virtual void Draw()override;

	// --- データの受け渡し
	static void InResultData(ResultGameInfo InData);
	static void InStageLevel(StageType InLevel);
	static void InBestScore(int InScore, StageType InStage);
	static int OutBestScore(StageType InStage);
	static bool GetStageClear(int nStage);
	static void SetStageClear(int nStage, bool bWin);

private:
	// --- 処理
	void LoadTexture(void);
	void DefaultSetPos(void);
	void KeyProsess(void);
	void AnimationManagemer(void);
	void SetCamData(void);

	// --- 描画系
	void WinAnimation(void);
	void LoseAnimation(void);
	void WinDisp(void);
	void LoseDisp(void);
	void NumberDisp(void);

private:
	// --- 描画情報
	SpriteEx*	m_pHitPoint;
	SpriteEx*	m_pSelect;
	SpriteEx*	m_pShadow;
	SpriteEx*	m_pStageSelect[2];
	SpriteEx*	m_pSummonData;
	SpriteEx*	m_pUnderBar;
	SpriteEx*	m_pBack;
	SpriteEx*	m_pText;
	SpriteEx*	m_pCharacter;
	SpriteEx*	m_pLighting;
	SpriteEx*	m_pTextShadow;
	SpriteEx*	m_pClearTime;
	SpriteEx*	m_pNextUI[2];
	SpriteEx*	m_pStar;
	SpriteEx*	m_pScore;
	SpriteEx*	m_pBestScore;
	SpriteEx*	m_pPiyo;
	CNumberUI*	m_pNumber;
	SpriteEx*	m_pRankIco[4];

	// --- 変数
	
	// - int
	int nAnimationTimer;
	int nAnimationFrame;
	int nScore;
	int nSelect;

	int nMM = 0;
	int nSS = 0;
	int nHp = 0;
	int nSpawn = 0;
	int nAvS = 0;
	int nDisp = 0;
	int nnScore = 0;
	int nPush[2]; // ボタンを押した時
	// - float
	float fPiyoA;
	FLOAT2 fUvPos;
	// - bool
	bool bAnimation;
	bool bWorldClear;
	bool bScore;
	bool bBestScore;
	bool bEnter;

	// --- サウンド
	CSoundList* m_pResultSound;
	CSoundList* m_pResultSelectSound;
	IXAudio2SourceVoice* m_pResultBGM;
	IXAudio2SourceVoice* m_pResultSelectSE;

	// --- スタティック系
	static StageType StageLevel; 
	static ResultGameInfo ResultGameData;
	static int nBestScore[8];
	static bool bClearState[8];
};