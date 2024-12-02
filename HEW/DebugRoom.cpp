#include "DebugRoom.h"
#include "CameraDebug.h"
#include "Input.h"
#include "Controller.h"

enum DEBUG_MODEL
{
	TEST1 = 0,

	MAX_DEBUGMODEL
};

const char* ch_pModelList[MAX_DEBUGMODEL] =
{

};

constexpr float LIMIT_POS_X = 100.0f;
constexpr float LIMIT_POS_Y = 100.0f;
constexpr float LIMIT_SIZE_X = 100.0f;
constexpr float LIMIT_SIZE_Y = 100.0f;

CDebugRoom::CDebugRoom()
	: m_pDebugModel{}, m_pCamera(nullptr)
	, m_nSelect(0)
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
	if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER)) m_nSelect++;
	if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER)) m_nSelect--;

	if (m_nSelect < 0) m_nSelect = MAX_DEBUGMODEL - 1;
	else if (m_nSelect >= MAX_DEBUGMODEL) m_nSelect = 0;

	//if (IsKeyPress(VK_RIGHT) || )
}

void CDebugRoom::Draw()
{
	m_pDebugModel[m_nSelect]->SetPostion(m_tPos.x, m_tPos.y, m_tPos.z);
	m_pDebugModel[m_nSelect]->SetScale(m_tSize.x, m_tSize.y, m_tSize.z);
	m_pDebugModel[m_nSelect]->SetRotation(m_tRotate.x, m_tRotate.y, m_tRotate.z);
	m_pDebugModel[m_nSelect]->SetViewMatrix(m_pCamera);
	m_pDebugModel[m_nSelect]->SetProjectionMatrix(m_pCamera);
	m_pDebugModel[m_nSelect]->Draw();
}