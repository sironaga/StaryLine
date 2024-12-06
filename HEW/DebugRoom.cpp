#include "DebugRoom.h"
#include "Input.h"
#include "Controller.h"
#include "Main.h"

#define SIZECONTROLL_DEADZONE 10000.0f
#define SIZESPEED_AJUST 1,600.0f

constexpr float LIMIT_POS_X = 100.0f;
constexpr float LIMIT_POS_Y = 100.0f;
constexpr float LIMIT_SIZE_X = 100.0f;
constexpr float LIMIT_SIZE_Y = 100.0f;

constexpr float SIZE_SPEED = 10.0f;

const char* ch_pModelList[CDebugRoom::MAX_DEBUGMODEL] =
{
	MODEL_PASS("Running.fbx"),
	MODEL_PASS("test.fbx"),
};

CDebugRoom::CDebugRoom()
	: m_pDebugModel{}
	, m_nSelect(0), m_bSelect(false)
	, m_tPos{ 0.0f,0.0f,0.0f }, m_tSize{ 1.0f,1.0f,1.0f }, m_tRotate{ 0.0f,0.0f,0.0f }
	, m_pField(nullptr)
{

	for (int i = 0; i < MAX_DEBUGMODEL; i++)
	{
		m_pDebugModel[i] = new CModelEx(ch_pModelList[i]);
	}
}

CDebugRoom::~CDebugRoom()
{
	for (int i = 0; i < MAX_DEBUGMODEL; i++)
	{
		SAFE_DELETE_ARRAY(m_pDebugModel[i]);
	}
}

void CDebugRoom::Update()
{
	//m_pCamera->Update();

	if (IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_THUMB))m_bSelect = true;

	if (IsKeyTrigger('L') || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		m_nSelect++;
		m_bSelect = true;
	}
	else if (IsKeyTrigger('J') || CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		m_nSelect--;
		m_bSelect = true;
	}
	if (m_nSelect < 0) m_nSelect = MAX_DEBUGMODEL - 1;
	else if (m_nSelect >= MAX_DEBUGMODEL) m_nSelect = 0;

	if (m_bSelect)
	{
		m_tPos = { 0.0f,0.0f,0.0f };
		m_tSize = { 1.0f,1.0f,1.0f };
		m_tRotate = { 0.0f,0.0f,0.0f };
		m_bSelect = false;
	}
}

void CDebugRoom::Draw()
{
	m_pDebugModel[m_nSelect]->SetViewMatrix(GetView());
	m_pDebugModel[m_nSelect]->SetProjection(GetProj());
	m_pDebugModel[m_nSelect]->Draw();
}