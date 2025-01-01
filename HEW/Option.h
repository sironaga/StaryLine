#pragma once

#include "Texture.h"
#include "_StructList.h"

class COption
{
public:
	COption();
	~COption();
	void Update();
	void Draw();

	void SetOption(bool isOption);
	bool GetOption();
	float GetBGMVoluem();
	float GetSEVoluem();
private:
	enum OPTION_TEXTURE_KIND
	{
		// 背景
		BACKBOARD,
		
		// デフォルト設定に戻す
		DEFAULT,
		DEFAULT_PUSH,
		
		// ボタンヘルプ
		HELP_CONTROLLER,
		HELP_KEYBOARD,
		HELP_FPS,
		HELP_RESOLUSION,
		HELP_MASTERVOLUME,
		HELP_BGM,
		HELP_SE,
		HELP_WINDOWMODE,
		
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
		INPUT_TIPE_B,

		// FPS選択
		FPS_30,
		FPS_60,

		// スクリーンモード選択
		FULLSCREEN,
		WINDOW,

		// 大項目
		SECTION_SELECT,
		SECTION_BAR,
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

		KINDMAX_OPTION
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

		TAB_MAX
	};

	enum SECTION_KIND
	{
		SEC_SOUND = 0,
		SEC_SCREEN,
		SEC_INPUT,
	};

	Texture* m_pTexture[KINDMAX_OPTION];
	SpriteParam* m_pParam[KINDMAX_OPTION];

	int m_nSection;
	int m_nSelect;
	int m_nValue[TAB_MAX];
	int m_nTempValue[TAB_MAX];

	bool m_bOptionMode;
	bool m_bSetValue;

	void LoadPass();
	void InitParam();
	void InitValue();
	void InitValue(SECTION_KIND kind);

	void UpdateSound();
	void UpdateScreen();
	void UpdateInput();
	void SetValue(int kind);

	void DrawSound();
	void DrawScreen();
	void DrawInput();
};