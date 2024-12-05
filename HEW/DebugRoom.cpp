#include "DebugRoom.h"
#include "CameraDebug.h"
#include "Input.h"
#include "Controller.h"

#define SIZECONTROLL_DEADZONE 10000.0f
#define SIZESPEED_AJUST 1,600.0f

constexpr float LIMIT_POS_X = 100.0f;
constexpr float LIMIT_POS_Y = 100.0f;
constexpr float LIMIT_SIZE_X = 100.0f;
constexpr float LIMIT_SIZE_Y = 100.0f;

constexpr float SIZE_SPEED = 10.0f;

#define MODEL_PASS(fbx) ("Assets/Model/" fbx)

const char* ch_pModelList[CDebugRoom::MAX_DEBUGMODEL] =
{
	MODEL_PASS("Running.fbx"),
	MODEL_PASS("test.fbx"),
};

CDebugRoom::CDebugRoom()
	: m_pDebugModel{}, m_pCamera(nullptr)
	, m_nSelect(0), m_bSelect(false)
	, m_tPos{ 0.0f,0.0f,0.0f }, m_tSize{ 50.0f,50.0f,50.0f }, m_tRotate{ 0.0f,0.0f,0.0f }
{
	m_pCamera = new CameraDebug();

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
	m_pCamera->Update();

	if (IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_THUMB))m_bSelect = true;

	if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		m_nSelect++;
		m_bSelect = true;
	}
	else if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		m_nSelect--;
		m_bSelect = true;
	}
	if (m_nSelect < 0) m_nSelect = MAX_DEBUGMODEL - 1;
	else if (m_nSelect >= MAX_DEBUGMODEL) m_nSelect = 0;

	if (IsKeyPress('Q'))
	{
		m_tSize.x += SIZE_SPEED;
		m_tSize.y += SIZE_SPEED;
		m_tSize.z += SIZE_SPEED;
	}
	else if (IsKeyPress('E'))
	{
		m_tSize.x -= SIZE_SPEED;
		m_tSize.y -= SIZE_SPEED;
		m_tSize.z -= SIZE_SPEED;
	}
	else if (CGetRStick().y < -SIZECONTROLL_DEADZONE || CGetRStick().y < SIZECONTROLL_DEADZONE)
	{
		m_tSize.x += CGetRStick().y / SIZESPEED_AJUST;
		m_tSize.y += CGetRStick().y / SIZESPEED_AJUST;
		m_tSize.z += CGetRStick().y / SIZESPEED_AJUST;
	}

	if (m_bSelect)
	{
		m_tPos = { 0.0f,0.0f,0.0f };
		m_tSize = { 50.0f,50.0f,50.0f };
		m_tRotate = { 0.0f,0.0f,0.0f };
		m_bSelect = false;
	}
}

void CDebugRoom::Draw()
{
	m_pDebugModel[m_nSelect]->SetPostion(m_tPos.x, m_tPos.y, m_tPos.z);
	m_pDebugModel[m_nSelect]->SetScale(m_tSize.x, m_tSize.y, m_tSize.z);
	m_pDebugModel[m_nSelect]->SetRotation(m_tRotate.x, m_tRotate.y, m_tRotate.z);
	m_pDebugModel[m_nSelect]->SetViewMatrix(m_pCamera->GetViewMatrix());
	m_pDebugModel[m_nSelect]->SetProjection(m_pCamera->GetProjectionMatrix());
	m_pDebugModel[m_nSelect]->Draw();
}