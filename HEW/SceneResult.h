#pragma once
// --- Include
#include "Scene.h"
#include"_StructList.h"
#include"SpriteEx.h"
#include"Main.h"
#include"NumberUI.h"
#include "SoundList.h"
#include"Conection_Sys.h"
#include"StageSelect.h"


enum Rank
{
	S_RANK,
	A_RANK,
	B_RANK,
	C_RANK,
	NONE_RANK,
};

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
	static void Init_Result();
	static void InResultData(ResultGameInfo InData);
	static void InStageLevel(StageType InLevel);
	static void InBestScore(int InScore, StageType InStage);
	static int OutBestScore(StageType InStage);
	static bool GetStageClear(int nStage);
	static void SetStageClear(int nStage, bool bWin);
	static Rank GetRankData(int nStage);
	static void SetRankData(int nStage, Rank SetRank);
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
	SpriteEx* m_pBackGround;		// --- 背景
	SpriteEx* m_pUnberUI;			// --- ボタンUI
	SpriteEx* m_pStageSelectUI[2];	// --- ステージセレクトUI
	SpriteEx* m_pNextUI[2];			// --- リトライ or 次のステージ
	SpriteEx* m_pSelectUI;			// --- 選択中UI
	SpriteEx* m_pResultData;		// --- 勝利時に表示されるリザルト結果
	SpriteEx* m_pLighting;			// --- 光
	SpriteEx* m_pText;				// --- テキスト
	SpriteEx* m_pRank[NONE_RANK];   // --- Rank
	SpriteEx* m_pCharacter;			// --- リリー
	SpriteEx* m_pBestScore;			// --- ベストスコア
	SpriteEx* m_pTextShadow;		// --- テキスト影
	SpriteEx* m_pCharacterShadow;	// --- キャラクター影
	CNumberUI* m_pNumber;			// --- 数字の描画

	// --- 変数
	
	// - int
	int nAnimationTimer;
	int nAnimationFrame;
	int nSelect;
	int nPush[2];			// ボタンを押した時
	int nLoopAnimeCount;	// --- キャラクター用ループカウンター
	int nStage;				 // --- 現在のステージ番号の保管

	// - Number用
	int nMinutes;
	int nSeconds;
	int nHitPoint;
	int nSpawn;
	int nAverage;
	int nDispCount;
	
	int nScore;

	// - float
	FLOAT2 fUvPos;	// UVのposition

	// - WinGroup
	FLOAT2 Group;	  // --- 全体のposition管理に使用

	// - bool
	bool bWorldClear; // --- ? - 3をClearしたか判定
	bool bAnimation;  // --- アニメーションを実行するかしないか
	bool bEnter;	  // --- Enterが押されたか判定
	bool bLoopAnime;  // --- キャラクター用ループAnimation
	bool bBestScore;


	// - Number用Bool
	bool bTimeProsess;
	bool bHPProsess;
	bool bSpwanProsess;

	// --- サウンド
	CSoundList* m_pResultSound;
	CSoundList* m_pResultSelectSound;
	IXAudio2SourceVoice* m_pResultBGM;
	IXAudio2SourceVoice* m_pResultSelectSE;

	// --- スタティック系
	static StageType StageLevel; 
	static ResultGameInfo ResultGameData;
	static int nBestScore[MAX_STAGE];
	static bool bClearState[MAX_STAGE];
	static Rank StageRank[MAX_STAGE];
};