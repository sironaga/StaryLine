#include "Option.h"
#include "Main.h"
#include "InputEx.h"
#include "Defines.h"
#include "Sprite.h"

constexpr float OPTIONBACK_SIZE_X = 120.0f;
constexpr float OPTIONBACK_SIZE_Y = 50.0f;
constexpr float OPTIONPOINT_SIZE = 20.0f;

#define	DEBUG_SPEED (2.5f)

COption::COption()
	: m_nSelect(SEC_SOUND), m_nSection(MASTER)
	, m_pTexture{}, m_pParam{}
	, m_bOptionMode(false)
	, m_fVolume{ 50.0f,50.0f,50.0f }
	, m_bSetValue(false)
	, m_nValue{}, m_nTempValue{}
{
	LoadPass();
	InitParam();
	InitValue();
}

COption::~COption()
{
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		SAFE_DELETE(m_pTexture[i]);
		SAFE_DELETE(m_pParam[i]);
	}
}

void COption::Update()
{
	if (IsKeyPress(VK_RIGHT))	m_pParam[SELECT_LEFT]->pos.x+= DEBUG_SPEED;
	if (IsKeyPress(VK_LEFT))	m_pParam[SELECT_LEFT]->pos.x-=DEBUG_SPEED;
	if (IsKeyPress(VK_UP))		m_pParam[SELECT_LEFT]->pos.y+=DEBUG_SPEED;
	if (IsKeyPress(VK_DOWN))	m_pParam[SELECT_LEFT]->pos.y-=DEBUG_SPEED;
	if (IsKeyPress('D'))	m_pParam[SELECT_LEFT]->size.x+=DEBUG_SPEED;
	if (IsKeyPress('A'))	m_pParam[SELECT_LEFT]->size.x-=DEBUG_SPEED;
	if (IsKeyPress('W'))	m_pParam[SELECT_LEFT]->size.y+=DEBUG_SPEED;
	if (IsKeyPress('S'))	m_pParam[SELECT_LEFT]->size.y-=DEBUG_SPEED;
	if (IsKeyPress('Q'))
	{
		m_pParam[SELECT_LEFT]->size.x-=DEBUG_SPEED;
		m_pParam[SELECT_LEFT]->size.y-=DEBUG_SPEED;
	}
	if (IsKeyPress('E'))
	{
		m_pParam[SELECT_LEFT]->size.x+=DEBUG_SPEED;
		m_pParam[SELECT_LEFT]->size.y+=DEBUG_SPEED;
	}

	if (m_bSetValue)SetValue(m_nSelect);
	else
	{
		switch (m_nSection)
		{
		case SEC_SOUND:
			UpdateSound();
			if (IsKeyTrigger('L'))
			{
				m_nSection = SEC_SCREEN;
				m_nSelect = SCREEN_MODE;
			}
			m_pParam[SECTION_SELECT]->pos.x = -380.0f;
			break;
		case SEC_SCREEN:
			UpdateScreen();
			if (IsKeyTrigger('J'))
			{
				m_nSection = SEC_SOUND;
				m_nSelect = MASTER;
			}
			if (IsKeyTrigger('L'))
			{
				m_nSection = SEC_INPUT;
				m_nSelect = KEY_BOARD;
			}
			m_pParam[SECTION_SELECT]->pos.x = 0.0f;
			break;
		case SEC_INPUT:
			UpdateInput();
			if (IsKeyTrigger('J'))
			{
				m_nSection = SEC_SCREEN;
				m_nSelect = SCREEN_MODE;
			}
			m_pParam[SECTION_SELECT]->pos.x = 380.0f;
			break;
		default:break;
		}
		if (IsKeyTrigger(VK_ESCAPE))SetOption(false);
	}

	if (IsKeyTrigger(VK_RETURN))m_bSetValue ^= true;
}

void COption::Draw()
{
	SetRender2D();
	Sprite::SetParam(m_pParam[BACKBOARD]);
	Sprite::SetTexture(m_pTexture[BACKBOARD]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SECTION_BAR]);
	Sprite::SetTexture(m_pTexture[SECTION_BAR]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SECTION_SELECT]);
	Sprite::SetTexture(m_pTexture[SECTION_SELECT]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SECTION_LEFT]);
	Sprite::SetTexture(m_pTexture[SECTION_LEFT]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SECTION_RIGHT]);
	Sprite::SetTexture(m_pTexture[SECTION_RIGHT]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[DEFAULT]);
	Sprite::SetTexture(m_pTexture[DEFAULT]);
	Sprite::Draw();

	switch (m_nSection)
	{
	case SEC_SOUND:DrawSound(); break;
	case SEC_SCREEN:DrawScreen(); break;
	case SEC_INPUT:DrawInput(); break;
	default:break;
	}

	Sprite::SetParam(m_pParam[TAB_SELECT]);
	Sprite::SetTexture(m_pTexture[TAB_SELECT]);
	Sprite::Draw();
}

void COption::SetOption(bool isOption)
{
	m_bOptionMode = isOption;
}

bool COption::GetOption()
{
	return m_bOptionMode;
}

float COption::GetBGMVoluem()
{
	return m_fVolume[1];
}

float COption::GetSEVoluem()
{
	return m_fVolume[2];
}

void COption::LoadPass()
{
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		m_pTexture[i] = new Texture();
		m_pParam[i] = new  SpriteParam();
		m_pParam[i]->world = m_pParam[i]->operator()();
	}
	m_pTexture[BACKBOARD]->Create(TEX_PASS("Option/Option_Backboard.png"));
	m_pTexture[DEFAULT]->Create(TEX_PASS("Option/Option_Default.png"));
	m_pTexture[DEFAULT_PUSH]->Create(TEX_PASS("Option/Option_Default_Push.png"));
	m_pTexture[HELP_CONTROLLER]->Create(TEX_PASS("Option/Option_Help_Control_Controler.png"));
	m_pTexture[HELP_KEYBOARD]->Create(TEX_PASS("Option/Option_Help_Control_Keyboard.png"));
	m_pTexture[HELP_FPS]->Create(TEX_PASS("Option/Option_Help_FPS.png"));
	m_pTexture[HELP_RESOLUSION]->Create(TEX_PASS("Option/Option_Help_Resolusion.png"));
	m_pTexture[HELP_MASTERVOLUME]->Create(TEX_PASS("Option/Option_Help_Mastervolume.png"));
	m_pTexture[HELP_BGM]->Create(TEX_PASS("Option/Option_Help_BGM.png"));
	m_pTexture[HELP_SE]->Create(TEX_PASS("Option/Option_Help_SE.png"));
	m_pTexture[HELP_WINDOWMODE]->Create(TEX_PASS("Option/Option_Help_Windowmode.png"));
	m_pTexture[ICON_CONTROLLER_A]->Create(TEX_PASS("Option/Option_Icon_Controler_TypeA.png"));
	m_pTexture[ICON_CONTROLLER_B]->Create(TEX_PASS("Option/Option_Icon_Controler_TypeB.png"));
	m_pTexture[ICON_KEYBOARD_A]->Create(TEX_PASS("Option/Option_Icon_Keyboard_TypeA.png"));
	m_pTexture[ICON_KEYBOARD_B]->Create(TEX_PASS("Option/Option_Icon_Keyboard_TypeB.png"));
	m_pTexture[SELECT_LEFT]->Create(TEX_PASS("Option/Option_List_ButtonL.png"));
	m_pTexture[SELECT_RIGHT]->Create(TEX_PASS("Option/Option_List_ButtonR.png"));
	m_pTexture[BAR_FRAME]->Create(TEX_PASS("Option/Option_Control_Bar_Frame.png"));
	m_pTexture[BAR_GAGE]->Create(TEX_PASS("Option/Option_Control_Bar_Gage.png"));
	m_pTexture[BAR_HANDLE]->Create(TEX_PASS("Option/Option_Control_Bar_Handle.png"));
	m_pTexture[OFF_NOT_SELECTED]->Create(TEX_PASS("Option/Option_Mark_Off.png"));
	m_pTexture[OFF_SELECTED]->Create(TEX_PASS("Option/Option_Mark_Off_Selected.png"));
	m_pTexture[ON_NOT_SELECTED]->Create(TEX_PASS("Option/Option_Mark_On.png"));
	m_pTexture[ON_SELECTED]->Create(TEX_PASS("Option/Option_Mark_On_Selected.png"));
	m_pTexture[RESOLUSION_800x600]->Create(TEX_PASS("Option/Option_List_Resolution_800~600.png"));
	m_pTexture[RESOLUSION_1280x720]->Create(TEX_PASS("Option/Option_List_Resolution_1280~720.png"));
	m_pTexture[RESOLUSION_1600x900]->Create(TEX_PASS("Option/Option_List_Resolution_1600~900.png"));
	m_pTexture[RESOLUSION_1920x1080]->Create(TEX_PASS("Option/Option_List_Resolution_1920~1080.png"));
	m_pTexture[RESOLUSION_SELECT]->Create(TEX_PASS("Option/Option_List_Resolution_Selected.png"));
	m_pTexture[INPUT_TIPE_A]->Create(TEX_PASS("Option/Option_List_TypeA.png"));
	m_pTexture[INPUT_TIPE_B]->Create(TEX_PASS("Option/Option_List_TypeB.png"));
	m_pTexture[FULLSCREEN]->Create(TEX_PASS("Option/Option_Mark_Text_Fullscleen.png"));
	m_pTexture[WINDOW]->Create(TEX_PASS("Option/Option_Mark_Text_Window.png"));
	m_pTexture[FPS_30]->Create(TEX_PASS("Option/Option_Mark_Text_30fps.png"));
	m_pTexture[FPS_60]->Create(TEX_PASS("Option/Option_Mark_Text_60fps.png"));
	m_pTexture[SECTION_SELECT]->Create(TEX_PASS("Option/Option_Section_Frame.png"));
	m_pTexture[SECTION_BAR]->Create(TEX_PASS("Option/Option_Section_Bar.png"));
	m_pTexture[SECTION_LEFT]->Create(TEX_PASS("Option/Option_Section_L1.png"));
	m_pTexture[SECTION_RIGHT]->Create(TEX_PASS("Option/Option_Section_R1.png"));
	m_pTexture[TAB_SELECT]->Create(TEX_PASS("Option/Option_Tab_Frame.png"));
	m_pTexture[TAB_CONTROLLER]->Create(TEX_PASS("Option/Option_Tab_Control.png"));
	m_pTexture[TAB_KEYBOARD]->Create(TEX_PASS("Option/Option_Tab_Control.png"));
	m_pTexture[TAB_FPS]->Create(TEX_PASS("Option/Option_Tab_Flame.png"));
	m_pTexture[TAB_RESOLUSION]->Create(TEX_PASS("Option/Option_Tab_Resolusion.png"));
	m_pTexture[TAB_MASTERVOLUME]->Create(TEX_PASS("Option/Option_Tab_Mastervolume.png"));
	m_pTexture[TAB_BGM]->Create(TEX_PASS("Option/Option_Tab_BGM.png"));
	m_pTexture[TAB_SE]->Create(TEX_PASS("Option/Option_Tab_SE.png"));
	m_pTexture[TAB_WINDOWMODE]->Create(TEX_PASS("Option/Option_Tab_Viewmode.png"));
}

void COption::InitParam()
{
	m_pParam[BACKBOARD]->pos =			{ 0.0f,0.0f };
	m_pParam[BACKBOARD]->size =			{ 1600.0f,900.0f };
	m_pParam[DEFAULT]->pos =			{ -480.0f,-230.0f };
	m_pParam[DEFAULT]->size =			{ 430.0f,70.0f };
	m_pParam[DEFAULT_PUSH]->pos =		{ -480.0f,-230.0f };
	m_pParam[DEFAULT_PUSH]->size =		{ 430.0f,70.0f };
	m_pParam[HELP_CONTROLLER]->pos =	{ 0.0f,-345.0f };
	m_pParam[HELP_CONTROLLER]->size =	{ 1700.0f,105.0f };
	m_pParam[HELP_KEYBOARD]->pos =		{ 0.0f,-345.0f };
	m_pParam[HELP_KEYBOARD]->size =		{ 1700.0f,105.0 };
	m_pParam[HELP_FPS]->pos =			{ 0.0f,-345.0f };
	m_pParam[HELP_FPS]->size =			{ 1700.0f,105.0 };
	m_pParam[HELP_RESOLUSION]->pos =	{ 0.0f,-345.0f };
	m_pParam[HELP_RESOLUSION]->size =	{ 1700.0f,105.0 };
	m_pParam[HELP_MASTERVOLUME]->pos =	{ 0.0f,-345.0f };
	m_pParam[HELP_MASTERVOLUME]->size = { 1700.0f,105.0 };
	m_pParam[HELP_BGM]->pos =			{ 0.0f,-345.0f };
	m_pParam[HELP_BGM]->size =			{ 1700.0f,105.0 };
	m_pParam[HELP_SE]->pos =			{ 0.0f,-345.0f };
	m_pParam[HELP_SE]->size =			{ 1700.0f,105.0 };
	m_pParam[HELP_WINDOWMODE]->pos =	{ 0.0f,-345.0f };
	m_pParam[HELP_WINDOWMODE]->size =	{ 1700.0f,105.0 };
	m_pParam[ICON_CONTROLLER_A]->pos =	{ 230.0f,-80.0f };
	m_pParam[ICON_CONTROLLER_A]->size = { 910.0f,410.0f };
	m_pParam[ICON_CONTROLLER_B]->pos =	{ 230.0f,-80.0f };
	m_pParam[ICON_CONTROLLER_B]->size = { 910.0f,410.0f };
	m_pParam[ICON_KEYBOARD_A]->pos =	{ 230.0f,-80.0f };
	m_pParam[ICON_KEYBOARD_A]->size =	{ 910.0f,410.0f };
	m_pParam[ICON_KEYBOARD_B]->pos =	{ 230.0f,-80.0f };
	m_pParam[ICON_KEYBOARD_B]->size =	{ 910.0f,410.0f };
	m_pParam[SELECT_LEFT]->pos =		{ -90.0f,0.0f };
	m_pParam[SELECT_LEFT]->size =		{ 70.0f,70.0f };
	m_pParam[SELECT_RIGHT]->pos =		{ 450.0f,0.0f };
	m_pParam[SELECT_RIGHT]->size =		{ 70.0f,70.0f };
	m_pParam[BAR_FRAME]->pos =			{ 0.0f,0.0f };
	m_pParam[BAR_FRAME]->size =			{ 800.0f,600.0f };
	m_pParam[BAR_GAGE]->pos =			{ 0.0f,0.0f };
	m_pParam[BAR_GAGE]->size =			{ 800.0f,600.0f };
	m_pParam[BAR_HANDLE]->pos =			{ 0.0f,0.0f };
	m_pParam[BAR_HANDLE]->size =		{ 800.0f,600.0f };
	m_pParam[OFF_NOT_SELECTED]->pos =	{ 0.0f,0.0f };
	m_pParam[OFF_NOT_SELECTED]->size =	{ 800.0f,600.0f };
	m_pParam[OFF_SELECTED]->pos =		{ 0.0f,0.0f };
	m_pParam[OFF_SELECTED]->size =		{ 800.0f,600.0f };
	m_pParam[ON_NOT_SELECTED]->pos =	{ 0.0f,0.0f };
	m_pParam[ON_NOT_SELECTED]->size =	{ 800.0f,600.0f };
	m_pParam[ON_SELECTED]->pos =		{ 0.0f,0.0f };
	m_pParam[ON_SELECTED]->size =		{ 800.0f,600.0f };
	m_pParam[RESOLUSION_800x600]->pos = { 0.0f,0.0f };
	m_pParam[RESOLUSION_800x600]->size =	{ 800.0f,600.0f };
	m_pParam[RESOLUSION_1280x720]->pos =	{ 0.0f,0.0f };
	m_pParam[RESOLUSION_1280x720]->size =	{ 800.0f,600.0f };
	m_pParam[RESOLUSION_1600x900]->pos =	{ 0.0f,0.0f };
	m_pParam[RESOLUSION_1600x900]->size =	{ 800.0f,600.0f };
	m_pParam[RESOLUSION_1920x1080]->pos =	{ 0.0f,0.0f };
	m_pParam[RESOLUSION_1920x1080]->size =	{ 800.0f,600.0f };
	m_pParam[RESOLUSION_SELECT]->pos =	{ 380,460.0f };
	m_pParam[RESOLUSION_SELECT]->size =	{ 510,110.0f };
	m_pParam[INPUT_TIPE_A]->pos =		{ 180.0f,300.0f };
	m_pParam[INPUT_TIPE_A]->size =		{ 440.0f,70.0f };
	m_pParam[INPUT_TIPE_B]->pos =		{ 180.0f,300.0f };
	m_pParam[INPUT_TIPE_B]->size =		{ 440.0f,70.0f };
	m_pParam[FULLSCREEN]->pos =			{ 0.0f,0.0f };
	m_pParam[FULLSCREEN]->size =		{ 800.0f,600.0f };
	m_pParam[WINDOW]->pos =				{ 0.0f,0.0f };
	m_pParam[WINDOW]->size =			{ 800.0f,600.0f };
	m_pParam[FPS_30]->pos =				{ 0.0f,0.0f };
	m_pParam[FPS_30]->size =			{ 800.0f,600.0f };
	m_pParam[FPS_60]->pos =				{ 0.0f,0.0f };
	m_pParam[FPS_60]->size =			{ 800.0f,600.0f };
	m_pParam[SECTION_SELECT]->pos =		{ 380.0f,460.0f };
	m_pParam[SECTION_SELECT]->size =	{ 510.0f,110.0f };
	m_pParam[SECTION_BAR]->pos =		{ 0.0f,460.0f };
	m_pParam[SECTION_BAR]->size =		{ 1400.0f,100.0f };
	m_pParam[SECTION_LEFT]->pos =		{ -600.0f,460.0f };
	m_pParam[SECTION_LEFT]->size =		{ 90.0f,70.0f };
	m_pParam[SECTION_RIGHT]->pos =		{ 600.0f,460.0f };
	m_pParam[SECTION_RIGHT]->size =		{ 90.0f,70.0f };
	m_pParam[TAB_SELECT]->pos =			{ -485.0f,300.0f };
	m_pParam[TAB_SELECT]->size =		{ 440.0f,70.0f };
	m_pParam[TAB_CONTROLLER]->pos =		{ -480.0f,200.0f };
	m_pParam[TAB_CONTROLLER]->size =	{ 430.0f,70.0f };
	m_pParam[TAB_KEYBOARD]->pos =		{ -480.0f,300.0f };
	m_pParam[TAB_KEYBOARD]->size =		{ 430.0f,70.0f };
	m_pParam[TAB_FPS]->pos =			{ -480.0f,100.0f };
	m_pParam[TAB_FPS]->size =			{ 430.0f,70.0f };
	m_pParam[TAB_RESOLUSION]->pos =		{ -480.0f,200.0f };
	m_pParam[TAB_RESOLUSION]->size =	{ 430.0f,70.0f };
	m_pParam[TAB_MASTERVOLUME]->pos =	{ -480.0f,300.0f };
	m_pParam[TAB_MASTERVOLUME]->size =	{ 430.0f,70.0f };
	m_pParam[TAB_BGM]->pos =			{ -480.0f,200.0f };
	m_pParam[TAB_BGM]->size =			{ 430.0f,70.0f };
	m_pParam[TAB_SE]->pos =				{ -480.0f,100.0f };
	m_pParam[TAB_SE]->size =			{ 430.0f,70.0f };
	m_pParam[TAB_WINDOWMODE]->pos =		{ -480.0f,300.0f };
	m_pParam[TAB_WINDOWMODE]->size =	{ 430.0f,70.0f };
}

void COption::InitValue()
{
	m_nValue[MASTER]  = 5;
	m_nValue[BGM] = 5;
	m_nValue[SE] = 5;
	m_nValue[SCREEN_MODE] = 0;
	m_nValue[RESOLUSION] = 3;
	m_nValue[FRAME_RATE] = 1;
	m_nValue[KEY_BOARD] = 0;
	m_nValue[CONTROLLER] = 0;
	for (int i = 0; i < TAB_MAX; i++)
	{
		m_nTempValue[i] = m_nValue[i];
	}
}

void COption::InitValue(SECTION_KIND kind)
{
	switch (kind)
	{
	case SEC_SOUND:	
		m_nValue[MASTER] = 5;
		m_nValue[BGM] = 5;
		m_nValue[SE] = 5;
		break;
	case SEC_SCREEN:
		m_nValue[SCREEN_MODE] = 0;
		m_nValue[RESOLUSION] = 3;
		m_nValue[FRAME_RATE] = 1;
		break;
	case SEC_INPUT:
		m_nValue[KEY_BOARD] = 0;
		m_nValue[CONTROLLER] = 0;
		break;
	default:
		break;
	}
}

void COption::UpdateSound()
{
	switch (m_nSelect)
	{
	case MASTER:
		if (IsKeyTrigger('K'))	m_nSelect = BGM;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_MASTERVOLUME]->pos.y;
		break;
	case BGM:
		if (IsKeyTrigger('I'))	m_nSelect = MASTER;
		if (IsKeyTrigger('K'))	m_nSelect = SE;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_BGM]->pos.y;
		break;
	case SE:
		if (IsKeyTrigger('I'))	m_nSelect = BGM;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_SE]->pos.y;
		break;
	default:break;
	}

	if (IsKeyTrigger(VK_CONTROL))InitValue(SEC_SOUND);
}

void COption::UpdateScreen()
{
	switch (m_nSelect)
	{
	case SCREEN_MODE:
		if (IsKeyTrigger('K'))	m_nSelect = RESOLUSION;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_WINDOWMODE]->pos.y;
		break;
	case RESOLUSION:
		if (IsKeyTrigger('I'))	m_nSelect = SCREEN_MODE;
		if (IsKeyTrigger('K'))	m_nSelect = FRAME_RATE;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_RESOLUSION]->pos.y;
		break;
	case FRAME_RATE:
		if (IsKeyTrigger('I'))	m_nSelect = RESOLUSION;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_FPS]->pos.y;
		break;
	default:break;
	}

	if (IsKeyTrigger(VK_CONTROL))InitValue(SEC_SCREEN);
}

void COption::UpdateInput()
{
	switch (m_nSelect)
	{
	case KEY_BOARD:
		if (IsKeyTrigger('K'))	m_nSelect = CONTROLLER;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_KEYBOARD]->pos.y;
		break;
	case CONTROLLER:
		if (IsKeyTrigger('I'))	m_nSelect = KEY_BOARD;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_CONTROLLER]->pos.y;
		break;
	default:break;
	}

	if (IsKeyTrigger(VK_CONTROL))InitValue(SEC_INPUT);
}

void COption::SetValue(int kind)
{
	switch (kind)
	{
	case COption::MASTER:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[MASTER]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[MASTER]--;
		AJUST_MINMAX(m_nTempValue[MASTER], 0, 10);
		break;
	case COption::BGM:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[BGM]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[BGM]--;
		AJUST_MINMAX(m_nTempValue[BGM], 0, 10);
		break;
	case COption::SE:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[SE]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[SE]--;
		AJUST_MINMAX(m_nTempValue[SE], 0, 10);
		break;
	case COption::SCREEN_MODE:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[SCREEN_MODE]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[SCREEN_MODE]--;
		AJUST_MINMAX(m_nTempValue[SCREEN_MODE], 0, 1);
		break;
	case COption::RESOLUSION:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[RESOLUSION]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[RESOLUSION]--;
		AJUST_MINMAX(m_nTempValue[SE], 0, 3);
		break;
	case COption::FRAME_RATE:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[FRAME_RATE]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[FRAME_RATE]--;
		AJUST_MINMAX(m_nTempValue[FRAME_RATE], 0, 1);
		break;
	case COption::KEY_BOARD:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[KEY_BOARD]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[KEY_BOARD]--;
		AJUST_MINMAX(m_nTempValue[KEY_BOARD], 0, 1);
		break;
	case COption::CONTROLLER:
		if (IsKeyTrigger(VK_RIGHT))m_nTempValue[CONTROLLER]++;
		if (IsKeyTrigger(VK_LEFT))m_nTempValue[CONTROLLER]--;
		AJUST_MINMAX(m_nTempValue[CONTROLLER], 0, 1);
		break;
	default:
		break;
	}

	if (IsKeyTrigger(VK_RETURN))
	{
		m_nValue[kind] = m_nTempValue[kind];
		m_bSetValue = false;
	}
	if (IsKeyTrigger(VK_ESCAPE))
	{
		m_nTempValue[kind] = m_nValue[kind];
		m_bSetValue = false;
	}
}

void COption::DrawSound()
{
	Sprite::SetParam(m_pParam[TAB_MASTERVOLUME]);
	Sprite::SetTexture(m_pTexture[TAB_MASTERVOLUME]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[TAB_BGM]);
	Sprite::SetTexture(m_pTexture[TAB_BGM]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[TAB_SE]);
	Sprite::SetTexture(m_pTexture[TAB_SE]);
	Sprite::Draw();

	switch (m_nSelect)
	{
	case MASTER:
		Sprite::SetParam(m_pParam[HELP_MASTERVOLUME]);
		Sprite::SetTexture(m_pTexture[HELP_MASTERVOLUME]);
		Sprite::Draw();
		break;
	case BGM:
		Sprite::SetParam(m_pParam[HELP_BGM]);
		Sprite::SetTexture(m_pTexture[HELP_BGM]);
		Sprite::Draw();
		break;
	case SE:
		Sprite::SetParam(m_pParam[HELP_SE]);
		Sprite::SetTexture(m_pTexture[HELP_SE]);
		Sprite::Draw();
		break;
	default:break;
	}
}

void COption::DrawScreen()
{
	Sprite::SetParam(m_pParam[TAB_WINDOWMODE]);
	Sprite::SetTexture(m_pTexture[TAB_WINDOWMODE]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[TAB_RESOLUSION]);
	Sprite::SetTexture(m_pTexture[TAB_RESOLUSION]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[TAB_FPS]);
	Sprite::SetTexture(m_pTexture[TAB_FPS]);
	Sprite::Draw();

	switch (m_nSelect)
	{
	case SCREEN_MODE:
		Sprite::SetParam(m_pParam[HELP_WINDOWMODE]);
		Sprite::SetTexture(m_pTexture[HELP_WINDOWMODE]);
		Sprite::Draw();
		Sprite::SetParam(m_pParam[TAB_WINDOWMODE]);
		Sprite::SetTexture(m_pTexture[TAB_WINDOWMODE]);
		Sprite::Draw();
		break;
	case RESOLUSION:
		Sprite::SetParam(m_pParam[HELP_RESOLUSION]);
		Sprite::SetTexture(m_pTexture[HELP_RESOLUSION]);
		Sprite::Draw();
		Sprite::SetParam(m_pParam[TAB_WINDOWMODE]);
		Sprite::SetTexture(m_pTexture[TAB_WINDOWMODE]);
		Sprite::Draw();
		break;
	case FRAME_RATE:
		Sprite::SetParam(m_pParam[HELP_FPS]);
		Sprite::SetTexture(m_pTexture[HELP_FPS]);
		Sprite::Draw();
		break;
	default:break;
	}
}

void COption::DrawInput()
{
	Sprite::SetParam(m_pParam[TAB_KEYBOARD]);
	Sprite::SetTexture(m_pTexture[TAB_KEYBOARD]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[TAB_CONTROLLER]);
	Sprite::SetTexture(m_pTexture[TAB_CONTROLLER]);
	Sprite::Draw();
	m_pParam[SELECT_LEFT]->pos.y = 300.0f;
	m_pParam[SELECT_RIGHT]->pos.y = 300.0f;
	Sprite::SetParam(m_pParam[SELECT_LEFT]);
	Sprite::SetTexture(m_pTexture[SELECT_LEFT]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SELECT_RIGHT]);
	Sprite::SetTexture(m_pTexture[SELECT_RIGHT]);
	Sprite::Draw();
	m_pParam[SELECT_LEFT]->pos.y = 200.0f;
	m_pParam[SELECT_RIGHT]->pos.y = 200.0f;
	Sprite::SetParam(m_pParam[SELECT_LEFT]);
	Sprite::SetTexture(m_pTexture[SELECT_LEFT]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SELECT_RIGHT]);
	Sprite::SetTexture(m_pTexture[SELECT_RIGHT]);
	Sprite::Draw();

	switch (m_nSelect)
	{
	case KEY_BOARD:
		Sprite::SetParam(m_pParam[HELP_KEYBOARD]);
		Sprite::SetTexture(m_pTexture[HELP_KEYBOARD]);
		Sprite::Draw();

		Sprite::SetParam(m_pParam[ICON_KEYBOARD_A]);
		switch (m_nValue[KEY_BOARD])
		{
		case 0:Sprite::SetTexture(m_pTexture[ICON_KEYBOARD_A]); break;
		case 1:Sprite::SetTexture(m_pTexture[ICON_KEYBOARD_B]); break;
		default:
			break;
		}
		Sprite::Draw();
		break;
	case CONTROLLER:
		Sprite::SetParam(m_pParam[HELP_CONTROLLER]);
		Sprite::SetTexture(m_pTexture[HELP_CONTROLLER]);
		Sprite::Draw();

		Sprite::SetParam(m_pParam[ICON_CONTROLLER_A]);
		switch (m_nValue[CONTROLLER])
		{
		case 0:Sprite::SetTexture(m_pTexture[ICON_CONTROLLER_A]); break;
		case 1:Sprite::SetTexture(m_pTexture[ICON_CONTROLLER_B]); break;
		default:
			break;
		}
		Sprite::Draw();
		break;
	default:break;
	}

	m_pParam[INPUT_TIPE_A]->pos.y = 300.0f;
	Sprite::SetParam(m_pParam[INPUT_TIPE_A]);
	switch (m_nValue[KEY_BOARD])
	{
	case 0:Sprite::SetTexture(m_pTexture[INPUT_TIPE_A]); break;
	case 1:Sprite::SetTexture(m_pTexture[INPUT_TIPE_B]); break;
	default:
		break;
	}
	Sprite::Draw();

	m_pParam[INPUT_TIPE_A]->pos.y = 200.0f;
	Sprite::SetParam(m_pParam[INPUT_TIPE_A]);
	switch (m_nValue[CONTROLLER])
	{
	case 0:Sprite::SetTexture(m_pTexture[INPUT_TIPE_A]); break;
	case 1:Sprite::SetTexture(m_pTexture[INPUT_TIPE_B]); break;
	default:
		break;
	}
	Sprite::Draw();	
}