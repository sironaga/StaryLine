#pragma once
#include "SpriteEx.h"
#include "Option.h"
#include "BackGround.h"
#include "SoundList.h"
class CPause
{
public:
	//初期化
	static void Init();
	//終了処理
	static void Uninit();
	//更新処理
	static void Update();
	//描画処理
	static void Draw();

	//====セッター====
	//オプションの情報を設定
	static void SetOption(COption* InOption);
	static void SetRetry();
	static void SetPause();
	static void SetSelect();
	//====ゲッター====
	//ポーズ画面の判定
	static bool GetPause();
	//オプション画面の判定
	static bool GetOption();
	//リトライの判定
	static bool GetRetry();
	//ステージセレクトの判定
	static bool GetSelect();
	//ゲームに戻るの判定
	static bool GetReturn();
	//textureの再読み込み
	static void InitReload();
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
	static SpriteEx* m_pPauseTex[11];//texture情報
	static COption* m_pOption;//オプション
	static CSoundList* m_pSoundPause[3];//音データ
	static IXAudio2SourceVoice* m_pSoundPauseSE[3];//スピーカー
	static bool m_bPause;//ポーズ画面判定
	static bool m_bOption;//オプション画面判定
	static bool m_bRetry;//リトライ判定
	static bool m_bSelect;//ステータス選択判定
	static bool m_bReturn;//ゲームに戻る判定
	static bool m_bClose;//画面を閉じる
	static bool m_bSwitch;//アニメーションの切り替え

	static FLOAT3 m_fPos[11];//ポジション
	static FLOAT3 m_fSize[11];//サイズ
	static float m_ftime;//計算用
	static float m_fAnimeTime;//計算用
	//画面のサイズ
	enum
	{
		SCREEN_800,
		SCREEN_1280,
		SCREEN_1600,
		SCREEN_1920,
	};

};

