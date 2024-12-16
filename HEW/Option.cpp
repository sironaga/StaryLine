#include "Option.h"
#include "Main.h"
#include "InputEx.h"
#include "Defines.h"

constexpr float OPTIONBACK_SIZE_X = 120.0f;
constexpr float OPTIONBACK_SIZE_Y = 50.0f;
constexpr float OPTIONPOINT_SIZE = 20.0f;

#define	OPTIONBAR_SPEED (1.0f)

COption::COption()
	: m_pOptions{}, m_bOption(false)
	, m_nSelect(0), m_nOptionValue{ 50,50 }
{
	m_pOptions[IMAGE_BACKGROUND] = new SpriteEx(TEX_PASS("Option/OptionBackGround.png"));
	m_pOptions[IMAGE_OPTION_BACK] = new SpriteEx(TEX_PASS("Option/OptionBar.png"));
	m_pOptions[IMAGE_POINT] = new SpriteEx(TEX_PASS("Option/OptionPoint.png"));

	m_tBarPos[KIND_BGM] = { 0.0f,60.0f,0.0f };
	m_tBarPos[KIND_SE] = { 0.0f, 40.0f,0.0f };
	m_tOptionPos[KIND_BGM] = { 0.0f,59.0f,0.0f };
	m_tOptionPos[KIND_SE] = { 0.0f,39.0f,0.0f };
}

COption::~COption()
{
	for (int i = 0; i < IMAGE_MAX; i++)
	{
		SAFE_DELETE(m_pOptions[i]);
	}
}

void COption::Update()
{
	if (m_bOption)
	{
		if (IsKeyTrigger(VK_DOWN) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			m_nSelect++;
			if (m_nSelect >= KIND_MAX) m_nSelect = 0;
		}
		if (IsKeyTrigger(VK_UP) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_UP))
		{
			m_nSelect--;
			if (m_nSelect < 0) m_nSelect = KIND_MAX - 1;
		}

		switch (m_nSelect)
		{
		case COption::KIND_BGM:
			if (IsKeyPress(VK_LEFT) || CGetButtons(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				if (m_nOptionValue[KIND_BGM] <= 0) m_nOptionValue[KIND_BGM] = 0;
				else 
				{
					m_tOptionPos[KIND_BGM].x -= OPTIONBAR_SPEED;
					m_nOptionValue[KIND_BGM]--;
				}
			}
			if (IsKeyPress(VK_RIGHT) || CGetButtons(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				if (m_nOptionValue[KIND_BGM] >= 100) m_nOptionValue[KIND_BGM] = 100;
				else
				{
					m_tOptionPos[KIND_BGM].x += OPTIONBAR_SPEED;
					m_nOptionValue[KIND_BGM]++;
				}
			}
			break;
		case COption::KIND_SE:
			if (IsKeyPress(VK_LEFT) || CGetButtons(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				if (m_nOptionValue[KIND_SE] <= 0) m_nOptionValue[KIND_SE] = 0;
				else
				{
					m_tOptionPos[KIND_SE].x -= OPTIONBAR_SPEED;
					m_nOptionValue[KIND_SE]--;
				}
			}
			if (IsKeyPress(VK_RIGHT) || CGetButtons(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				if (m_nOptionValue[KIND_SE] >= 100) m_nOptionValue[KIND_SE] = 100;
				else
				{
					m_tOptionPos[KIND_SE].x += OPTIONBAR_SPEED;
					m_nOptionValue[KIND_SE]++;
				}
			}
			break;
		default:
			break;
		}

		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B))SetOption();
	}
}

void COption::Draw()
{
	if (m_bOption)
	{
		SetRender2D();

		m_pOptions[IMAGE_BACKGROUND]->SetPositon(GetCameraPos().x, GetCameraPos().y, GetCameraPos().z + 10);
		m_pOptions[IMAGE_BACKGROUND]->SetSize(20, 13, 0.0f);
		m_pOptions[IMAGE_BACKGROUND]->SetRotation(0.0f, 0.0f, 0.0f);
		m_pOptions[IMAGE_BACKGROUND]->SetUvPos(0.0f, 0.0f);
		m_pOptions[IMAGE_BACKGROUND]->SetUvSize(1.0f, 1.0f);
		m_pOptions[IMAGE_BACKGROUND]->SetView(GetView());
		m_pOptions[IMAGE_BACKGROUND]->SetProjection(GetProj());
		m_pOptions[IMAGE_BACKGROUND]->SetTexture();
		m_pOptions[IMAGE_BACKGROUND]->Disp();

		m_pOptions[IMAGE_OPTION_BACK]->SetPositon(m_tBarPos[KIND_BGM].x, m_tBarPos[KIND_BGM].y, m_tBarPos[KIND_BGM].z);
		m_pOptions[IMAGE_OPTION_BACK]->SetSize(OPTIONBACK_SIZE_X, OPTIONBACK_SIZE_Y, 0.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetRotation(0.0f, 0.0f, 0.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetUvPos(0.0f, 0.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetUvSize(1.0f, 1.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetView(GetView());
		m_pOptions[IMAGE_OPTION_BACK]->SetProjection(GetProj());
		m_pOptions[IMAGE_OPTION_BACK]->SetTexture();
		m_pOptions[IMAGE_OPTION_BACK]->Disp();

		m_pOptions[IMAGE_OPTION_BACK]->SetPositon(m_tBarPos[KIND_SE].x, m_tBarPos[KIND_SE].y, m_tBarPos[KIND_SE].z);
		m_pOptions[IMAGE_OPTION_BACK]->SetSize(OPTIONBACK_SIZE_X, OPTIONBACK_SIZE_Y, 0.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetRotation(0.0f, 0.0f, 0.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetUvPos(0.0f, 0.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetUvSize(1.0f, 1.0f);
		m_pOptions[IMAGE_OPTION_BACK]->SetView(GetView());
		m_pOptions[IMAGE_OPTION_BACK]->SetProjection(GetProj());
		m_pOptions[IMAGE_OPTION_BACK]->SetTexture();
		m_pOptions[IMAGE_OPTION_BACK]->Disp();

		m_pOptions[IMAGE_POINT]->SetPositon(m_tOptionPos[KIND_BGM].x, m_tOptionPos[KIND_BGM].y, m_tOptionPos[KIND_BGM].z);
		m_pOptions[IMAGE_POINT]->SetSize(OPTIONPOINT_SIZE, OPTIONPOINT_SIZE, 0.0f);
		m_pOptions[IMAGE_POINT]->SetRotation(0.0f, 0.0f, 0.0f);
		m_pOptions[IMAGE_POINT]->SetUvPos(0.0f, 0.0f);
		m_pOptions[IMAGE_POINT]->SetUvSize(1.0f, 1.0f);
		m_pOptions[IMAGE_POINT]->SetView(GetView());
		m_pOptions[IMAGE_POINT]->SetProjection(GetProj());
		m_pOptions[IMAGE_POINT]->SetTexture();
		m_pOptions[IMAGE_POINT]->Disp();

		m_pOptions[IMAGE_POINT]->SetPositon(m_tOptionPos[KIND_SE].x, m_tOptionPos[KIND_SE].y, m_tOptionPos[KIND_SE].z);
		m_pOptions[IMAGE_POINT]->SetSize(OPTIONPOINT_SIZE, OPTIONPOINT_SIZE, 0.0f);
		m_pOptions[IMAGE_POINT]->SetRotation(0.0f, 0.0f, 0.0f);
		m_pOptions[IMAGE_POINT]->SetUvPos(0.0f, 0.0f);
		m_pOptions[IMAGE_POINT]->SetUvSize(1.0f, 1.0f);
		m_pOptions[IMAGE_POINT]->SetView(GetView());
		m_pOptions[IMAGE_POINT]->SetProjection(GetProj());
		m_pOptions[IMAGE_POINT]->SetTexture();
		m_pOptions[IMAGE_POINT]->Disp();
	}
}

void COption::SetOption()
{
	m_bOption ^= true;
	m_nSelect = 0;
}

float COption::GetBGMVoluem()
{
	return (float)m_nOptionValue[KIND_BGM];
}

float COption::GetSEVoluem()
{
	return (float)m_nOptionValue[KIND_SE];;
}