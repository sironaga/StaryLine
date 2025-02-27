#pragma once

#include "Texture.h"
#include "_StructList.h"
#include "NumberUI.h"

/// <summary>
/// セクションを選択してからタブを選択する二段階式に
/// それに伴い減ったテクスチャの再設定
/// </summary>

	// 小項目の種類



class COption
{
public:
	COption();
	~COption();
	void Update();
	void Draw();

	/// @brief
	/// オプション画面を開く
	/// escボタン・Bボタンで閉じる
	void SetOption();
	/// @brief
	///ポジションに足す
	void SetAddPos(float x, float y);
	/// @brief
	/// サイズに掛ける
	void SetMulSize(float mul);
	/// @brief
	/// オプション画面の状態取得
	/// @return
	/// true:オプション画面を開いている
	/// @return
	/// false:オプション画面を閉じている
	bool GetOption();

	/// @brief
	/// マスターボリュームの音量を取得する
	/// @return
	/// 全体音量を0.0f～10.0fまで1.0f刻み
	float GetMasterVoluem();
	/// @brief
	/// BGMの音量を取得する
	/// @return
	/// 全体音量を0.0f～10.0fまで1.0f刻み
	float GetBGMVoluem();
	/// @brief
	/// SEの音量を取得する
	/// @return
	/// 全体音量を0.0f～10.0fまで1.0f刻み
	float GetSEVoluem();
	/// @brief
	/// フルスクリーンかどうかを取得する
	/// @return
	/// 0:ウィンドウモード	1:フルスクリーンモード
	int GetIsFullScreen();
	/// @brief
	/// FPSを取得する
	/// @return
	/// 30.0fもしくは60.0f
	static float GetFPS();
	/// @brief
	/// 解像度を取得する
	/// @return
	/// 解像度が変更出来ることを確認次第改善
	int GetResolusion();
	/// @brief
	/// キーボードでの操作方法を取得する
	/// @return
	/// 0:タイプA	1:タイプB
	static int GetKeyboardSetting();
	/// @brief
	/// コントローラーでの操作方法を取得する
	/// @return
	/// 0:タイプA	1:タイプB
	static int GetControllerSetting();

	static WORD GetTypeAB(int Type,WORD isTypeA);
	/// @brief
	// 解像度変更時に必要な初期化の呼び出し
	void InitResolusion();
	/// @brief
	// サイズの初期化
	void ResetSize();
	/// @brief
	// ポジションの初期化
	void ResetPos();
private:
	// ボタンテクスチャの種類
	enum OPTION_TEXTURE_KIND
	{
		// 背景
		BACKBOARD,
		BACKSCROLL,
		
		// デフォルト設定に戻す
		DEFAULT,
		DEFAULT_PUSH,
		DEFAULT_OK,
		DEFAULT_CANSEL,
		DEFAULT_CONFIRMATION,
		DEFAULT_SELECTED,
		
		// ボタンヘルプA
		HELP_CONTROLLER_A,
		HELP_KEYBOARD_A,
		HELP_FPS_A,
		HELP_RESOLUSION_A,
		HELP_MASTERVOLUME_A,
		HELP_BGM_A,
		HELP_SE_A,
		HELP_WINDOWMODE_A,
		HELP_DEFAULT_A,

		// ボタンヘルプB
		HELP_CONTROLLER_B,
		HELP_KEYBOARD_B,
		HELP_FPS_B,
		HELP_RESOLUSION_B,
		HELP_MASTERVOLUME_B,
		HELP_BGM_B,
		HELP_SE_B,
		HELP_WINDOWMODE_B,
		HELP_DEFAULT_B,
		
		// 操作方法アイコン
		ICON_CONTROLLER_A,
		ICON_CONTROLLER_B,
		ICON_KEYBOARD_A,
		ICON_KEYBOARD_B,
		
		// オプション選択肢
		SELECT_LEFT,
		SELECT_RIGHT,
		BAR_FRAME,
		BAR_GAGE,
		BAR_HANDLE,
		OFF_NOT_SELECTED,
		OFF_SELECTED,
		ON_NOT_SELECTED,
		ON_SELECTED,
		
		// 解像度選択
		RESOLUSION_800x600,
		RESOLUSION_1280x720,
		RESOLUSION_1600x900,
		RESOLUSION_1920x1080,
		RESOLUSION_SELECT,

		// 操作方法選択
		INPUT_TIPE_A,
		INPUT_TIPE_A2,
		INPUT_TIPE_B,
		INPUT_TIPE_B2,

		// FPS選択
		FPS_30,
		FPS_60,

		// スクリーンモード選択
		FULLSCREEN,
		WINDOW,

		// 大項目
		SECTION_SELECT,
		SECTION_LEFT,
		SECTION_RIGHT,

		// 小項目
		TAB_SELECT,
		TAB_CONTROLLER,
		TAB_KEYBOARD,
		TAB_FPS,
		TAB_RESOLUSION,
		TAB_MASTERVOLUME,
		TAB_BGM,
		TAB_SE,
		TAB_WINDOWMODE,
		TAB_DEFAULT,

		BACK_SWAP,

		KINDMAX_OPTION
	};

	// 大項目の種類
	enum SECTION_KIND
	{
		SEC_SOUND = 0,
		SEC_SCREEN,
		SEC_INPUT,
	};

	enum TAB_KIND
	{
		MASTER = 0,
		BGM,
		SE,
		SCREEN_MODE,
		RESOLUSION,
		FRAME_RATE,
		KEY_BOARD,
		CONTROLLER,
		DEFAULTSOUND,
		DEFAULTSCREEN,
		DEFAULTINPUT,

		TAB_MAX
	};


	Texture* m_pTexture[KINDMAX_OPTION];	// テクスチャの読み込み
	SpriteParam* m_pParam[KINDMAX_OPTION];	// テクスチャのパラメータ
	

	CNumberUI* m_pNumberUI;
	FLOAT2 OldPos[KINDMAX_OPTION];
	FLOAT2 MovePos;
	WORD m_Direction; 
	float m_fMul;
	int m_nSection;							// 大項目の選択
	int m_nSelect;							// 小項目の選択
	static int m_nValue[TAB_MAX];					// 各小項目の物量(確定)
	int m_nTempValue[TAB_MAX];				// 各小項目の物量(推定) 

	bool m_bOptionMode;						// オプション画面を開いているかどうか
	bool m_bSetValue;						// 小項目を操作しているかどうか

	int NowResolusion;						//現在の解像度

	void LoadPass();						// テクスチャのパス読み込み
	void InitParam();						// テクスチャのパラメータ設定
	void InitValue();						// 初期値設定
	void InitValue(SECTION_KIND kind);		// 大項目ごとにデフォルトに戻す

	void UpdateSound();						// サウンド設定
	void UpdateScreen();					// 画面設定
	void UpdateInput();						// 操作方法設定
	void SetValue(int kind);				// 各小項目の物量の設定

	void DrawSound();						// サウンド設定の描画
	void DrawScreen();						// 画面設定の描画
	void DrawInput();						// 操作方法設定の描画


};