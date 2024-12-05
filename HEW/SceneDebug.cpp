#include "SceneDebug.h"
#include "Defines.h"
#include "Input.h"
#include "Controller.h"
#include "Main.h"
#include <Xinput.h>

CDebugRoom* g_pDebugRoom;

void InitSceneDebug()
{
	g_pDebugRoom = new CDebugRoom();
}

void UninitSceneDebug()
{
	SAFE_DELETE(g_pDebugRoom);
}

void UpdateSceneDebug()
{
	g_pDebugRoom->Update();

	if (IsKeyTrigger(VK_TAB) || CGetButtons(XINPUT_GAMEPAD_X))
	{
		ChangeScene(SCENE_DEBUGROOM);
	}
}

void DrawSceneDebug()
{
	g_pDebugRoom->Draw();
}