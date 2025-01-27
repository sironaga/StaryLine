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
	void SetPause();
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
	//大項目
	enum SECTION_KIND
	{
		SEC_OPTION,
		SEC_RETRY,
		SEC_RETURN,
		SEC_STAGESELECT,

	};
private:
	SpriteEx* m_pPauseTex[11];//texture情報
	COption* m_pOption;//オプション
	CSoundList* m_pSoundPause[3];//音データ
	IXAudio2SourceVoice* m_pSoundPauseSE[3];//スピーカー
	bool m_bPause;//ポーズ画面判定
	bool m_bOption;//オプション画面判定
	bool m_bRetry;//リトライ判定
	bool m_bSelect;//ステータス選択判定
	bool m_bReturn;//ゲームに戻る判定
	bool m_bClose;//画面を閉じる

	FLOAT3 m_fPos[11];//ポジション
	FLOAT3 m_fSize[11];//サイズ
	float m_ftime;//計算用
	//画面のサイズ
	enum
	{
		SCREEN_800,
		SCREEN_1280,
		SCREEN_1600,
		SCREEN_1920,
	};

};

