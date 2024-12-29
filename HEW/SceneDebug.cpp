#include "SceneDebug.h"
#include "Defines.h"
#include "Input.h"
#include "Controller.h"
#include "Main.h"
#include <Xinput.h>

CSceneDebug::CSceneDebug()
	:m_pDebugRoom(nullptr)
{
	m_pDebugRoom = new CDebugRoom();
}

CSceneDebug::~CSceneDebug()
{
	SAFE_DELETE(m_pDebugRoom);
}

void CSceneDebug::Update()
{
	m_pDebugRoom->Update();

	if (IsKeyTrigger(VK_TAB) || CGetButtons(XINPUT_GAMEPAD_X))
	{
		SetNext(SCENE_TITLE);
	}
}

void CSceneDebug::Draw()
{
	m_pDebugRoom->Draw();
}
