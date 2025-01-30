#include "Option.h"
#include "Main.h"
#include "InputEx.h"
#include "Defines.h"
#include "Sprite.h"

int COption::m_nValue[TAB_MAX];

COption::COption()
	: m_nSelect(SEC_SOUND), m_nSection(MASTER)
	, m_pTexture{}, m_pParam{}
	, m_nTempValue{}
	, m_bOptionMode(false), m_bSetValue(false)
	, m_pNumberUI()
	, NowResolusion(1)
	, m_fMul(1.0f)
	, MovePos({0.0f,0.0f})
	, m_Direction(XINPUT_GAMEPAD_START)
{
	LoadPass();
	InitParam();
	InitValue();
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		OldPos[i].X = m_pParam[i]->pos.x;
		OldPos[i].Y = m_pParam[i]->pos.y;
	}
}

COption::~COption()
{
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		SAFE_DELETE(m_pTexture[i]);
		SAFE_DELETE(m_pParam[i]);
	}
	if (m_pNumberUI)
	{
		SAFE_DELETE(m_pNumberUI);
	}

}

void COption::Update()
{
	m_Direction = GetControllerLStickTriggerForeDirection();

	if (m_bSetValue)SetValue(m_nSelect);
	else
	{
		switch (m_nSection)
		{
		case SEC_SOUND:
			UpdateSound();
			if (WithGetKeyTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER, VK_RIGHT) || IsKeyTrigger('D'))
			{
				m_nSection = SEC_SCREEN;
				m_nSelect = SCREEN_MODE;
			}
			m_pParam[SECTION_SELECT]->pos.x = ( - 380.0f * m_fMul)+MovePos.X;
			break;
		case SEC_SCREEN:
			UpdateScreen();
			if (WithGetKeyTriger(XINPUT_GAMEPAD_LEFT_SHOULDER, VK_LEFT) || IsKeyTrigger('A'))
			{
				m_nSection = SEC_SOUND;
				m_nSelect = MASTER;
			}
			if (WithGetKeyTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER, VK_RIGHT) || IsKeyTrigger('D'))
			{
				m_nSection = SEC_INPUT;
				m_nSelect = KEY_BOARD;
			}
			m_pParam[SECTION_SELECT]->pos.x = 0.0f + MovePos.X;
			break;
		case SEC_INPUT:
			UpdateInput();
			if (WithGetKeyTriger(XINPUT_GAMEPAD_LEFT_SHOULDER, VK_LEFT) || IsKeyTrigger('A'))
			{
				m_nSection = SEC_SCREEN;
				m_nSelect = SCREEN_MODE;
			}
			m_pParam[SECTION_SELECT]->pos.x = (380.0f * m_fMul) + MovePos.X;
			break;
		default:break;
		}
		if (WithGetKeyTriger(XINPUT_GAMEPAD_B, VK_ESCAPE))
		{
			m_nSelect = SEC_SOUND;
			m_nSection = MASTER;
			m_bOptionMode = false;
		}
		if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN))m_bSetValue = true;
	}
}

void COption::Draw()
{
	SetRender2D();

	Sprite::SetParam(m_pParam[BACKBOARD]);
	Sprite::SetTexture(m_pTexture[BACKBOARD]);
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

	Sprite::ReSetSprite();
}

void COption::SetOption()
{
	m_bOptionMode = true;
}

void COption::SetAddPos(float x,float y)
{
	MovePos = { x,y };
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		m_pParam[i]->pos.x += x;
		m_pParam[i]->pos.y += y;
	}
}

void COption::SetMulSize(float mul)
{
	m_fMul = 1 - mul;
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		OldPos[i].X = m_pParam[i]->pos.x;
		OldPos[i].Y = m_pParam[i]->pos.y;
		m_pParam[i]->size.x *= mul;
		m_pParam[i]->size.y *= mul;
		if (m_pParam[i]->pos.x < SCREEN_WIDTH)m_pParam[i]->pos.x = m_pParam[i]->pos.x - m_pParam[i]->pos.x * m_fMul;
		else if(m_pParam[i]->pos.x > SCREEN_WIDTH)m_pParam[i]->pos.x = m_pParam[i]->pos.x + m_pParam[i]->pos.x * m_fMul;
		if (m_pParam[i]->pos.y <SCREEN_HEIGHT)m_pParam[i]->pos.y = m_pParam[i]->pos.y - m_pParam[i]->pos.y * m_fMul;
		else if (m_pParam[i]->pos.y > SCREEN_HEIGHT)m_pParam[i]->pos.y = m_pParam[i]->pos.y + m_pParam[i]->pos.y * m_fMul;
	}
	m_fMul = 1 - m_fMul;
}

bool COption::GetOption()
{
	return m_bOptionMode;
}

float COption::GetMasterVoluem()
{
	return (float)m_nValue[MASTER];
}

float COption::GetBGMVoluem()
{
	return (float)m_nValue[BGM];
}

float COption::GetSEVoluem()
{
	return (float)m_nValue[SE];
}

int COption::GetIsFullScreen()
{
	return m_nValue[SCREEN_MODE];
}

float COption::GetFPS()
{
	return 30.0f * (float)(m_nValue[FRAME_RATE] + 1);
}

int COption::GetResolusion()
{
	return  m_nValue[RESOLUSION];
}

int COption::GetKeyboardSetting()
{
	return  m_nValue[KEY_BOARD];
}

int COption::GetControllerSetting()
{
	return  m_nValue[CONTROLLER];
}

WORD COption::GetTypeAB(int Type, WORD isTypeA)
{
	WORD out;
	switch (Type)
	{
	case 0: out = isTypeA; break;
	case 1:
		if (isTypeA == XINPUT_GAMEPAD_A) out = XINPUT_GAMEPAD_B;
		else if (isTypeA == XINPUT_GAMEPAD_B) out = XINPUT_GAMEPAD_A;
		break;
	default:
		break;
	}
	return out;
}

void COption::InitResolusion()
{
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		delete m_pTexture[i];
		delete m_pParam[i];
	}
	LoadPass();
	InitParam();
}

void COption::ResetSize()
{
	if (m_fMul == 1.0f)return;
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		m_pParam[i]->size.x /= m_fMul;
		m_pParam[i]->size.y /= m_fMul;
		m_pParam[i]->pos.x = OldPos[i].X;
		m_pParam[i]->pos.y = OldPos[i].Y;
	}
	m_fMul = 1.0f;
}

void COption::ResetPos()
{
	for (int i = 0; i < KINDMAX_OPTION; i++)
	{
		m_pParam[i]->pos.x -= MovePos.X;
		m_pParam[i]->pos.y -= MovePos.Y;
	}
	MovePos = { 0.0f,0.0f };
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
	//m_pTexture[SECTION_BAR]->Create(TEX_PASS("Option/Option_Section_Bar.png"));
	m_pTexture[SECTION_LEFT]->Create(TEX_PASS("Option/Option_Section_L1.png"));
	m_pTexture[SECTION_RIGHT]->Create(TEX_PASS("Option/Option_Section_R1.png"));
	m_pTexture[TAB_SELECT]->Create(TEX_PASS("Option/Option_Tab_Frame.png"));
	m_pTexture[TAB_CONTROLLER]->Create(TEX_PASS("Option/Option_Tab_Control.png"));
	m_pTexture[TAB_KEYBOARD]->Create(TEX_PASS("Option/Option_Tab_Keyboard.png"));
	m_pTexture[TAB_FPS]->Create(TEX_PASS("Option/Option_Tab_Flame.png"));
	m_pTexture[TAB_RESOLUSION]->Create(TEX_PASS("Option/Option_Tab_Resolusion.png"));
	m_pTexture[TAB_MASTERVOLUME]->Create(TEX_PASS("Option/Option_Tab_Mastervolume.png"));
	m_pTexture[TAB_BGM]->Create(TEX_PASS("Option/Option_Tab_BGM.png"));
	m_pTexture[TAB_SE]->Create(TEX_PASS("Option/Option_Tab_SE.png"));
	m_pTexture[TAB_WINDOWMODE]->Create(TEX_PASS("Option/Option_Tab_Viewmode.png"));
	m_pNumberUI = new CNumberUI();
}

void COption::InitParam()
{
	m_pParam[BACKBOARD]->pos =			{ -5.0f,0.0f };
	m_pParam[BACKBOARD]->size =			{ 1500.0f,1000.0f };
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
	m_pParam[SELECT_LEFT]->pos =		{ -90.0f,300.0f };
	m_pParam[SELECT_LEFT]->size =		{ 70.0f,70.0f };
	m_pParam[SELECT_RIGHT]->pos =		{ 450.0f,0.0f };
	m_pParam[SELECT_RIGHT]->size =		{ 70.0f,70.0f };
	m_pParam[BAR_FRAME]->pos =			{ 300.0f,300.0f };
	m_pParam[BAR_FRAME]->size =			{ 700.0f,60.0f };
	m_pParam[BAR_GAGE]->pos =			{ 300.0f,300.0f };
	m_pParam[BAR_GAGE]->size =			{ 700.0f,30.0f };
	m_pParam[BAR_HANDLE]->pos =			{ 300.0f,300.0f };
	m_pParam[BAR_HANDLE]->size =		{ 30.0f,50.0f };
	m_pParam[OFF_NOT_SELECTED]->pos =	{ -90.0f,300.0f };
	m_pParam[OFF_NOT_SELECTED]->size =	{ 70.0f,70.0f };
	m_pParam[OFF_SELECTED]->pos =		{ -90.0f,300.0f };
	m_pParam[OFF_SELECTED]->size =		{ 70.0f,70.0f };
	m_pParam[ON_NOT_SELECTED]->pos =	{ -90.0f,0.0f };
	m_pParam[ON_NOT_SELECTED]->size =	{ 70.0f,70.0f };
	m_pParam[ON_SELECTED]->pos =		{ -90.0f,0.0f };
	m_pParam[ON_SELECTED]->size =		{ 70.0f,70.0f };
	m_pParam[RESOLUSION_800x600]->pos =		{ 180.0f,100.0f };
	m_pParam[RESOLUSION_800x600]->size =	{ 440.0f,70.0f };
	m_pParam[RESOLUSION_1280x720]->pos =	{ 180.0f,100.0f };
	m_pParam[RESOLUSION_1280x720]->size =	{ 440.0f,70.0f };
	m_pParam[RESOLUSION_1600x900]->pos =	{ 180.0f,100.0f };
	m_pParam[RESOLUSION_1600x900]->size =	{ 440.0f,70.0f };
	m_pParam[RESOLUSION_1920x1080]->pos =	{ 180.0f,100.0f };
	m_pParam[RESOLUSION_1920x1080]->size =	{ 440.0f,70.0f };
	m_pParam[RESOLUSION_SELECT]->pos =	{ 180.0f,70.0f };
	m_pParam[RESOLUSION_SELECT]->size =	{ 440.0f,10.0f };
	m_pParam[FULLSCREEN]->pos =			{ 120.0f, 290.0f };
	m_pParam[FULLSCREEN]->size =		{ 320.0f,70.0f };
	m_pParam[WINDOW]->pos =				{ 520.0f,290.0f };
	m_pParam[WINDOW]->size =			{ 230.0f,70.0f };
	m_pParam[FPS_30]->pos =				{ 70.0f,190.0f };
	m_pParam[FPS_30]->size =			{ 210.0f,80.0f };
	m_pParam[FPS_60]->pos =				{ 515.0f,190.0f };
	m_pParam[FPS_60]->size =			{ 210.0f,80.0f };
	m_pParam[INPUT_TIPE_A]->pos =		{ 180.0f,300.0f };
	m_pParam[INPUT_TIPE_A]->size =		{ 440.0f,70.0f };
	m_pParam[INPUT_TIPE_B]->pos =		{ 180.0f,300.0f };
	m_pParam[INPUT_TIPE_B]->size =		{ 440.0f,70.0f };
	m_pParam[SECTION_SELECT]->pos =		{ 380.0f,450.0f };
	m_pParam[SECTION_SELECT]->size = { 510.0f,110.0f };
	m_pParam[SECTION_LEFT]->pos =		{ -570.0f,450.0f };
	m_pParam[SECTION_LEFT]->size =		{ 90.0f,70.0f };
	m_pParam[SECTION_RIGHT]->pos =		{ 570.0f,450.0f };
	m_pParam[SECTION_RIGHT]->size =		{ 90.0f,70.0f };
	m_pParam[TAB_SELECT]->pos =			{ -485.0f,300.0f };
	m_pParam[TAB_SELECT]->size =		{ 440.0f,70.0f };
	m_pParam[TAB_CONTROLLER]->pos =		{ -480.0f,200.0f };
	m_pParam[TAB_CONTROLLER]->size =	{ 430.0f,70.0f };
	m_pParam[TAB_KEYBOARD]->pos =		{ -480.0f,300.0f };
	m_pParam[TAB_KEYBOARD]->size =		{ 430.0f,70.0f };
	m_pParam[TAB_FPS]->pos =			{ -480.0f,200.0f };
	m_pParam[TAB_FPS]->size =			{ 430.0f,70.0f };
	m_pParam[TAB_RESOLUSION]->pos =		{ -480.0f,100.0f };
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
	m_nValue[SCREEN_MODE] = 1;
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
		m_nValue[MASTER] = m_nTempValue[MASTER] = 5;
		m_nValue[BGM] = m_nTempValue[BGM] = 5;
		m_nValue[SE] = m_nTempValue[SE] = 5;
		break;
	case SEC_SCREEN:
		m_nValue[SCREEN_MODE] = m_nTempValue[SCREEN_MODE] = 1;
		m_nValue[RESOLUSION] = m_nTempValue[RESOLUSION] = 3;
		m_nValue[FRAME_RATE] = m_nTempValue[FRAME_RATE] = 1;
		break;
	case SEC_INPUT:
		m_nValue[KEY_BOARD] = m_nTempValue[KEY_BOARD] = 0;
		m_nValue[CONTROLLER] = m_nTempValue[CONTROLLER] = 0;
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
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_nSelect = BGM;
		}
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_MASTERVOLUME]->pos.y;
		break;
	case BGM:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)	m_nSelect = MASTER;
		else if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) ||IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)	m_nSelect = SE;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_BGM]->pos.y;
		break;
	case SE:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)	m_nSelect = BGM;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_SE]->pos.y;
		break;
	default:break;
	}

	if (WithGetKeyTriger(XINPUT_GAMEPAD_Y, VK_CONTROL))InitValue(SEC_SOUND);
}

void COption::UpdateScreen()
{
	switch (m_nSelect)
	{
	case SCREEN_MODE:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)	m_nSelect = FRAME_RATE;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_WINDOWMODE]->pos.y;
		break;
	case FRAME_RATE:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)	m_nSelect = SCREEN_MODE;
		else if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)	m_nSelect = RESOLUSION;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_FPS]->pos.y;
		break;
	case RESOLUSION:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)	m_nSelect = FRAME_RATE;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_RESOLUSION]->pos.y;
		break;
	default:break;
	}

	if (WithGetKeyTriger(XINPUT_GAMEPAD_Y, VK_CONTROL))InitValue(SEC_SCREEN);
}

void COption::UpdateInput()
{
	switch (m_nSelect)
	{
	case KEY_BOARD:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)	m_nSelect = CONTROLLER;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_KEYBOARD]->pos.y;
		break;
	case CONTROLLER:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)	m_nSelect = KEY_BOARD;
		m_pParam[TAB_SELECT]->pos.y = m_pParam[TAB_CONTROLLER]->pos.y;
		break;
	default:break;
	}

	if (WithGetKeyTriger(XINPUT_GAMEPAD_Y, VK_CONTROL))InitValue(SEC_INPUT);
}

void COption::SetValue(int kind)
{
	for (int i = 0; i < TAB_MAX; i++)
	{
		if (i == kind)continue;
		switch (i)
		{
		case COption::MASTER:
			m_pParam[TAB_MASTERVOLUME]->color.w = 0.5f;
			break;
		case COption::BGM:
			m_pParam[TAB_BGM]->color.w = 0.5f;
			break;
		case COption::SE:
			m_pParam[TAB_SE]->color.w = 0.5f;
			break;
		case COption::SCREEN_MODE:
			m_pParam[TAB_WINDOWMODE]->color.w = 0.5f;
			break;
		case COption::RESOLUSION:
			m_pParam[TAB_RESOLUSION]->color.w = 0.5f;
			break;
		case COption::FRAME_RATE:
			m_pParam[TAB_FPS]->color.w = 0.5f;
			break;
		case COption::KEY_BOARD:
			m_pParam[TAB_KEYBOARD]->color.w = 0.5f;
			break;
		case COption::CONTROLLER:
			m_pParam[TAB_CONTROLLER]->color.w = 0.5f;
			break;
		case COption::TAB_MAX:
			break;
		default:
			break;
		}
	}

	switch (kind)
	{
	case COption::MASTER:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[MASTER]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[MASTER]--;
		AJUST_MINMAX(m_nTempValue[MASTER], 0, 10);
		break;
	case COption::BGM:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[BGM]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[BGM]--;
		AJUST_MINMAX(m_nTempValue[BGM], 0, 10);
		break;
	case COption::SE:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[SE]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[SE]--;
		AJUST_MINMAX(m_nTempValue[SE], 0, 10);
		break;
	case COption::SCREEN_MODE:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[SCREEN_MODE]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[SCREEN_MODE]--;
		AJUST_MINMAX(m_nTempValue[SCREEN_MODE], 0, 1);
		break;
	case COption::RESOLUSION:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[RESOLUSION]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[RESOLUSION]--;
		AJUST_MINMAX(m_nTempValue[RESOLUSION], 0, 3);
		break;
	case COption::FRAME_RATE:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[FRAME_RATE]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[FRAME_RATE]--;
		AJUST_MINMAX(m_nTempValue[FRAME_RATE], 0, 1);
		break;
	case COption::KEY_BOARD:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[KEY_BOARD]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[KEY_BOARD]--;
		AJUST_MINMAX(m_nTempValue[KEY_BOARD], 0, 1);
		break;
	case COption::CONTROLLER:
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)m_nTempValue[CONTROLLER]++;
		if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A') || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)m_nTempValue[CONTROLLER]--;
		AJUST_MINMAX(m_nTempValue[CONTROLLER], 0, 1);
		break;
	default:
		break;
	}

	if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_SPACE))
	{
		m_nValue[kind] = m_nTempValue[kind];
		m_bSetValue = false;

		m_pParam[TAB_MASTERVOLUME]->color.w = 1.0f;
		m_pParam[TAB_BGM]->color.w = 1.0f;
		m_pParam[TAB_SE]->color.w = 1.0f;
		m_pParam[TAB_WINDOWMODE]->color.w = 1.0f;
		m_pParam[TAB_RESOLUSION]->color.w = 1.0f;
		m_pParam[TAB_FPS]->color.w = 1.0f;
		m_pParam[TAB_KEYBOARD]->color.w = 1.0f;
		m_pParam[TAB_CONTROLLER]->color.w = 1.0f;
	}
	if (WithGetKeyTriger(XINPUT_GAMEPAD_B, VK_ESCAPE))
	{
		m_nTempValue[kind] = m_nValue[kind];
		m_bSetValue = false;

		m_pParam[TAB_MASTERVOLUME]->color.w = 1.0f;
		m_pParam[TAB_BGM]->color.w = 1.0f;
		m_pParam[TAB_SE]->color.w = 1.0f;
		m_pParam[TAB_WINDOWMODE]->color.w = 1.0f;
		m_pParam[TAB_RESOLUSION]->color.w = 1.0f;
		m_pParam[TAB_FPS]->color.w = 1.0f;
		m_pParam[TAB_KEYBOARD]->color.w = 1.0f;
		m_pParam[TAB_CONTROLLER]->color.w = 1.0f;
	}
}

void COption::DrawSound()
{
	Sprite::SetParam(m_pParam[TAB_MASTERVOLUME]);
	Sprite::SetTexture(m_pTexture[TAB_MASTERVOLUME]);
	Sprite::Draw();	
	m_pParam[BAR_GAGE]->size.x = 60.0f * m_nTempValue[MASTER] * m_fMul;
	m_pParam[BAR_GAGE]->pos = { (263.0f * m_fMul - (m_pParam[BAR_FRAME]->size.x/2.0f) + m_pParam[BAR_GAGE]->size.x / 2.0f) + MovePos.X,(300.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_GAGE]);
	Sprite::SetTexture(m_pTexture[BAR_GAGE]);
	Sprite::Draw();
	m_pParam[BAR_FRAME]->pos = { (150.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_FRAME]);
	Sprite::SetTexture(m_pTexture[BAR_FRAME]);
	Sprite::Draw();
	m_pParam[BAR_HANDLE]->pos = { (((60.0f*m_nTempValue[MASTER] - 60.0f)-30.0f) * m_fMul) + MovePos.X ,(300.0f * m_fMul) + MovePos.Y};
	Sprite::SetParam(m_pParam[BAR_HANDLE]);
	Sprite::SetTexture(m_pTexture[BAR_HANDLE]);
	Sprite::Draw();

	

	Sprite::SetParam(m_pParam[TAB_BGM]);
	Sprite::SetTexture(m_pTexture[TAB_BGM]);
	Sprite::Draw();
	m_pParam[BAR_GAGE]->size.x = m_nTempValue[BGM] * 60.0f * m_fMul;
	m_pParam[BAR_GAGE]->pos = { ((263.0f * m_fMul - m_pParam[BAR_FRAME]->size.x / 2.0f) + m_pParam[BAR_GAGE]->size.x / 2.0f) + MovePos.X,(200.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_GAGE]);
	Sprite::SetTexture(m_pTexture[BAR_GAGE]);
	Sprite::Draw();
	m_pParam[BAR_FRAME]->pos = { (150.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_FRAME]);
	Sprite::SetTexture(m_pTexture[BAR_FRAME]);
	Sprite::Draw();
	m_pParam[BAR_HANDLE]->pos = { (((m_nTempValue[BGM] * 60.0f - 60.0f)-30.0f) * m_fMul) + MovePos.X ,(200.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_HANDLE]);
	Sprite::SetTexture(m_pTexture[BAR_HANDLE]);
	Sprite::Draw();

	Sprite::SetParam(m_pParam[TAB_SE]);
	Sprite::SetTexture(m_pTexture[TAB_SE]);
	Sprite::Draw();
	m_pParam[BAR_GAGE]->size.x = m_nTempValue[SE] * 60.0f * m_fMul;
	m_pParam[BAR_GAGE]->pos = { ((263.0f * m_fMul - m_pParam[BAR_FRAME]->size.x / 2.0f) + m_pParam[BAR_GAGE]->size.x / 2.0f) + MovePos.X,(100.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_GAGE]);
	Sprite::SetTexture(m_pTexture[BAR_GAGE]);
	Sprite::Draw();
	m_pParam[BAR_FRAME]->pos = { (150.0f * m_fMul) + MovePos.X,(100.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_FRAME]);
	Sprite::SetTexture(m_pTexture[BAR_FRAME]);
	Sprite::Draw();
	m_pParam[BAR_HANDLE]->pos = { (((m_nTempValue[SE] * 60.0f - 60.0f)-30.0f) * m_fMul) + MovePos.X,(100.0f * m_fMul) + MovePos.Y };
	Sprite::SetParam(m_pParam[BAR_HANDLE]);
	Sprite::SetTexture(m_pTexture[BAR_HANDLE]);
	Sprite::Draw();
	Sprite::ReSetSprite();
	float pos[3] = { -165.0f ,-165.0f ,-165.0f };
	for (int i = MASTER; i <= SE; i++)
	{
		if (m_nTempValue[i] == 10)
		{
			pos[i] = -180.0f;
		}
	}

	m_pNumberUI->SetNumber(m_nTempValue[MASTER]);
	m_pNumberUI->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumberUI->SetPos({ SCREEN_CENTER.x + pos[MASTER],SCREEN_CENTER.y - 300,0.0f });
	m_pNumberUI->Draw();

	m_pNumberUI->SetNumber(m_nTempValue[BGM]);
	m_pNumberUI->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumberUI->SetPos({ SCREEN_CENTER.x + pos[BGM],SCREEN_CENTER.y - 200,0.0f });
	m_pNumberUI->Draw();

	m_pNumberUI->SetNumber(m_nTempValue[SE]);
	m_pNumberUI->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumberUI->SetPos({ SCREEN_CENTER.x + pos[SE],SCREEN_CENTER.y - 100,0.0f });
	m_pNumberUI->Draw();



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
	Sprite::SetParam(m_pParam[FULLSCREEN]);
	Sprite::SetTexture(m_pTexture[FULLSCREEN]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[WINDOW]);
	Sprite::SetTexture(m_pTexture[WINDOW]);
	Sprite::Draw();

	
	switch (m_nValue[SCREEN_MODE])
	{
	case 0:	
		switch (m_nTempValue[SCREEN_MODE])
		{
		case 0:
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y};
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_NOT_SELECTED]);
			Sprite::Draw();
			break;
		case 1: 
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y};
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_NOT_SELECTED]);
			Sprite::Draw();
			break;
		}
		break;
	case 1:
		switch (m_nTempValue[SCREEN_MODE])
		{
		case 0:
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y};
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_NOT_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_SELECTED]);
			Sprite::Draw();
			break;
		case 1:
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y};
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_NOT_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(300.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_SELECTED]);
			Sprite::Draw();
			break;
		}
		break;
	default:
		break;
	}


	
	Sprite::SetParam(m_pParam[FPS_30]);
	Sprite::SetTexture(m_pTexture[FPS_30]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[FPS_60]);
	Sprite::SetTexture(m_pTexture[FPS_60]);
	Sprite::Draw();
	switch (m_nValue[FRAME_RATE])
	{
	case 0:
		switch (m_nTempValue[FRAME_RATE])
		{
		case 0:
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y};
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_NOT_SELECTED]);
			Sprite::Draw();
			break;
		case 1:
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y};
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_NOT_SELECTED]);
			Sprite::Draw();
			break;
		}
		break;
	case 1:
		switch (m_nTempValue[FRAME_RATE])
		{
		case 0:
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y};
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_NOT_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_SELECTED]);
			Sprite::Draw();
			break;
		case 1:
			m_pParam[OFF_NOT_SELECTED]->pos = { ( - 90.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[OFF_NOT_SELECTED]);
			Sprite::Draw();
			m_pParam[OFF_NOT_SELECTED]->pos = { (350.0f * m_fMul) + MovePos.X,(200.0f * m_fMul) + MovePos.Y };
			Sprite::SetParam(m_pParam[OFF_NOT_SELECTED]);
			Sprite::SetTexture(m_pTexture[ON_SELECTED]);
			Sprite::Draw();
			break;
		}
		break;
	default:
		break;
	}
	Sprite::SetParam(m_pParam[FPS_60]);
	Sprite::SetTexture(m_pTexture[FPS_60]);
	Sprite::Draw();

	Sprite::SetParam(m_pParam[RESOLUSION_1920x1080]);
	switch (m_nTempValue[RESOLUSION])
	{
	case 0:	Sprite::SetTexture(m_pTexture[RESOLUSION_800x600]);break;
	case 1:	Sprite::SetTexture(m_pTexture[RESOLUSION_1280x720]);break;
	case 2:	Sprite::SetTexture(m_pTexture[RESOLUSION_1600x900]);break;
	case 3:	Sprite::SetTexture(m_pTexture[RESOLUSION_1920x1080]);break;
	default:break;
	}
	Sprite::Draw();
	m_pParam[SELECT_LEFT]->pos.y = (100.0f *  m_fMul)+MovePos.Y;
	m_pParam[SELECT_RIGHT]->pos.y =( 100.0f * m_fMul)+MovePos.Y;
	Sprite::SetParam(m_pParam[SELECT_LEFT]);
	Sprite::SetTexture(m_pTexture[SELECT_LEFT]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SELECT_RIGHT]);
	Sprite::SetTexture(m_pTexture[SELECT_RIGHT]);
	Sprite::Draw();

	if (m_nValue[RESOLUSION] == m_nTempValue[RESOLUSION])
	{
		Sprite::SetParam(m_pParam[RESOLUSION_SELECT]);
		Sprite::SetTexture(m_pTexture[RESOLUSION_SELECT]);
		Sprite::Draw();
	}

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
	m_pParam[SELECT_LEFT]->pos.y =  (300.0f*m_fMul)+MovePos.Y;
	m_pParam[SELECT_RIGHT]->pos.y = (300.0f*m_fMul)+MovePos.Y;
	Sprite::SetParam(m_pParam[SELECT_LEFT]);
	Sprite::SetTexture(m_pTexture[SELECT_LEFT]);
	Sprite::Draw();
	Sprite::SetParam(m_pParam[SELECT_RIGHT]);
	Sprite::SetTexture(m_pTexture[SELECT_RIGHT]);
	Sprite::Draw();
	m_pParam[SELECT_LEFT]->pos.y =  (200.0f * m_fMul) + MovePos.Y;
	m_pParam[SELECT_RIGHT]->pos.y = (200.0f * m_fMul) + MovePos.Y;
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
		switch (m_nTempValue[KEY_BOARD])
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
		switch (m_nTempValue[CONTROLLER])
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

	m_pParam[INPUT_TIPE_A]->pos.y = (300.0f * m_fMul) + MovePos.Y;
	Sprite::SetParam(m_pParam[INPUT_TIPE_A]);
	switch (m_nTempValue[KEY_BOARD])
	{
	case 0:Sprite::SetTexture(m_pTexture[INPUT_TIPE_A]); break;
	case 1:Sprite::SetTexture(m_pTexture[INPUT_TIPE_B]); break;
	default:
		break;
	}
	Sprite::Draw();

	m_pParam[INPUT_TIPE_A]->pos.y = (200.0f * m_fMul) + MovePos.Y;
	Sprite::SetParam(m_pParam[INPUT_TIPE_A]);
	switch (m_nTempValue[CONTROLLER])
	{
	case 0:Sprite::SetTexture(m_pTexture[INPUT_TIPE_A]); break;
	case 1:Sprite::SetTexture(m_pTexture[INPUT_TIPE_B]); break;
	default:
		break;
	}
	Sprite::Draw();	
}


