#include "Main.h"
#include <memory>
#include "DirectX.h"
#include "Geometory.h"
#include "Sprite.h"
#include "Input.h"
#include "SceneGame.h"
#include "Defines.h"
#include "ShaderList.h"
#include "SpriteDrawer.h"
#include "Controller.h"
#include "StageSelect.h"
#include "SceneTitle.h"
#include "SoundList.h"
//--- グローバル変数
E_SCENE_TYPE g_SceneType;


HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr;
	// DirectX初期化
	hr = InitDirectX(hWnd, width, height, false);
	if (FAILED(hr)) { return hr; }

	// 他機能初期化
	Geometory::Init();
	Sprite::Init();
	InitInput();
	InitSound();
	ShaderList::Init();
	InitSpriteDrawer(GetDevice(), GetContext(), SCREEN_WIDTH, SCREEN_HEIGHT);


	// シーン作成
	g_SceneType = SCENE_TITLE;
	InitSceneTitle();

	return hr;
}

void Uninit()
{
	UninitSceneGame();
	UninitSpriteDrawer();
	ShaderList::Uninit();
	UninitInput();
	UnInitSound();
	Sprite::Uninit();
	Geometory::Uninit();
	UninitDirectX();

}

void Update()
{
    Controller_Update();
	UpdateInput();
	switch (g_SceneType)
	{
	case SCENE_TITLE:UpdateSceneTitle();
		break;
	case STAGE_SELECT:UpdateStageSelect();
		break;
	case SCENE_GAME:UpdateSceneGame();
		break;
	case SCENE_MAX:
		break;
	default:
		break;
	}
}

void Draw()
{
	BeginDrawDirectX();

	// 軸線の表示
#ifdef _DEBUG
	// グリッド
	DirectX::XMFLOAT4 lineColor(0.5f, 0.5f, 0.5f, 1.0f);
	float size = DEBUG_GRID_NUM * DEBUG_GRID_MARGIN;
	for (int i = 1; i <= DEBUG_GRID_NUM; ++i)
	{
		float grid = i * DEBUG_GRID_MARGIN;
		DirectX::XMFLOAT3 pos[2] = {
			DirectX::XMFLOAT3(grid, 0.0f, size),
			DirectX::XMFLOAT3(grid, 0.0f,-size),
		};
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].x = pos[1].x = -grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].x = size;
		pos[1].x = -size;
		pos[0].z = pos[1].z = grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].z = pos[1].z = -grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
	}
	// 軸
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(size,0,0), DirectX::XMFLOAT4(1,0,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,size,0), DirectX::XMFLOAT4(0,1,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,size), DirectX::XMFLOAT4(0,0,1,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(-size,0,0),  DirectX::XMFLOAT4(0,0,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,-size),  DirectX::XMFLOAT4(0,0,0,1));

	Geometory::DrawLines();

	// カメラの値
	static bool camPosSwitch = false;
	if (IsKeyTrigger(VK_RETURN)) {
		camPosSwitch ^= true;
	}

	DirectX::XMVECTOR camPos;
	if (camPosSwitch) {
		camPos = DirectX::XMVectorSet(2.5f, 30.5f, -40.0f, 0.0f);
	}
	else {
		camPos = DirectX::XMVectorSet(2.5f, 3.5f, -4.0f, 0.0f);
	}

	// ジオメトリ用カメラ初期化
	DirectX::XMFLOAT4X4 mat[2] = {};
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			camPos,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		)));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(60.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f)
	));
	Geometory::SetView(mat[0]);
	Geometory::SetProjection(mat[1]);
#endif

	switch (g_SceneType)
	{
	case SCENE_TITLE:DrawSceneTitle();
		break;
	case STAGE_SELECT:DrawStageSelect();
		break;
	case SCENE_GAME:DrawSceneGame();
		break;
	case SCENE_MAX:
		break;
	default:
		break;
	}
	EndDrawDirectX();
}

void ChangeScene(E_SCENE_TYPE next)
{
	//現在のシーンの終了
	switch (g_SceneType)
	{
	case(SCENE_TITLE):UninitSceneTitle();	break;
	case(STAGE_SELECT):UninitStageSelect(); break;
	case(SCENE_GAME):UninitSceneGame();		break;
	default:break;
	}

	//現在のシーンの更新
	g_SceneType = next;

	//次のシーンの初期化
	switch (g_SceneType)
	{
	case(SCENE_TITLE):InitSceneTitle();	break;
	case(STAGE_SELECT):InitStageSelect();	break;
	case(SCENE_GAME):InitSceneGame(GetStageNum());		break;
	default:break;
	}
}

// EOF