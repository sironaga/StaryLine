#pragma once
#include "SpriteEx.h"
#include "Option.h"
#include "BackGround.h"
#include "SoundList.h"
class CPause
{
public:
	//コンストラクタ
	CPause();
	//デストラクタ
	~CPause();
	//更新処理
	void Update();
	//描画処理
	void Draw();

	//====セッター====
	//オプションの情報を設定
	void SetOption(COption* InOption);
	void SetRetry();
	void SetSelect();
	//====ゲッター====
	//ポーズ画面の判定
	bool GetPause();
	//オプション画面の判定
	bool GetOption();
	//リトライの判定
	bool GetRetry();
	//ステージセレクトの判定
	bool GetSelect();
	//ゲームに戻るの判定
	bool GetReturn();
	//textureの再読み込み
	void InitReload();
private:
	enum SECTION_KIND
	{
		SEC_OPTION,
		SEC_RETRY,
		SEC_RETURN,
		SEC_STAGESELECT,

	};
private:
	SpriteEx* m_pPauseTex[11];
	COption* m_pOption;
	CBackGround* m_pBackGround;
	CSoundList* m_pSoundPause[3];
	IXAudio2SourceVoice* m_pSoundPauseSE[3];
	bool m_bPause;
	bool m_bOption;
	bool m_bRetry;
	bool m_bSelect;
	bool m_bReturn;

	FLOAT3 m_fPos[11];
	FLOAT3 m_fSize[11];

	enum
	{
		SCREEN_800,
		SCREEN_1280,
		SCREEN_1600,
		SCREEN_1920,
	};

};

