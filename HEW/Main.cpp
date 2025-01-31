// --- Include
#include <memory>
#include "Main.h"
#include "DirectX.h"
#include "Defines.h"
#include "Geometory.h"
#include "Sprite.h"
#include "ShaderList.h"
#include "SpriteDrawer.h"
#include "Input.h"
#include "Controller.h"

#include "Camera.h"
#include "CameraDebug.h"
#include "CameraEvent.h"

#include "LibEffekseer.h"
#include "SoundList.h"

#include "Fade.h"
#include "FadeBlack.h"
#include "Transition.h"

#include "Scene.h"
#include "SceneTitle.h"
#include "StageSelect.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneDebug.h"
#include "StartDirection.h"
#include "Pause.h"
#include "Option.h"

//--- グローバル変数
CStageSelect* g_pSceneSelect;
Camera* g_Camera;
CameraDebug* g_pDebugCamera;
CCameraEvent* g_pEventCamera;
RenderTarget* pRTV;
DepthStencil* pDSV;
bool IsGame = true;
CScene* g_pScene; // シーン 
CFade* g_pFade; // フェード 
CStartDirection* g_pDirection;
COption* g_pOption;
HWND g_hWnd;
E_SCENE_TYPE g_SceneType;
int g_NowWide = 1920;
int g_NowHeight = 1080;
int g_scene=SCENE_TITLE;
int g_nEvent = 0;
bool g_bResolution = false;
StageType g_stage = {};

HRESULT Init(HWND hWnd, UINT width, UINT height)
{

	HRESULT hr;
	// DirectX初期化
	hr = InitDirectX(hWnd, width, height, false);
	if (FAILED(hr)) { return hr; }
	
	// 他機能初期化
	g_pDebugCamera = new CameraDebug();
	g_pEventCamera = new CCameraEvent();
	g_Camera = g_pDebugCamera;
	Geometory::Init();
	Sprite::Init();
	LibEffekseer::Init(GetDevice(), GetContext());
	InitInput();
	ShaderList::Init();
	CSceneResult::Init_Result();
	InitSpriteDrawer(GetDevice(), GetContext(), SCREEN_WIDTH, SCREEN_HEIGHT);
	srand(timeGetTime());

	pRTV = GetDefaultRTV();
	pDSV = GetDefaultDSV();

	//ゲームスタート演出作成
	g_pDirection = new CStartDirection();

	// フェード作成 
	g_pFade = new CTransition();
	g_pFade->SetFade(1.0f, true);

	g_pOption = new COption();
	// シーン作成 
	g_pScene = new CSceneTitle(g_pOption);
	g_pScene->SetFade(g_pFade); // シーンに使用するフェードを設定 
	g_pScene->SetGameDirection(g_pDirection);
	g_nEvent = NOMAL_CAMERA;

	// --- Staticデータの初期化


	CPause::Init();
	CPause::SetOption(g_pOption);
	g_pScene->SethWnd(hWnd);
	//g_pOption->SetAddPos(1000.0f, 0.0f);

	return hr;
}

void Uninit()
{
	SAFE_DELETE(g_Camera);
	SAFE_DELETE(g_pScene);
	SAFE_DELETE(g_pFade);
	SAFE_DELETE(g_pDirection);
	SAFE_DELETE(g_pOption);
	UninitSpriteDrawer();
	ShaderList::Uninit();
	UninitInput();

	
	CPause::Uninit();
	LibEffekseer::Uninit();
	Sprite::Uninit();
	Geometory::Uninit();
	UninitDirectX();
	ID3D11Debug *pDebug;
	pDebug = GetDebug();
	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
	//g_pSourseTitleSE = nullptr;
}

void Update()
{
	g_SceneType;
	// --- 入力情報の更新
	Controller_Update();
	UpdateInput();
	// --- カメラ情報の更新
	g_Camera->Update();
	//ポーズの更新
	if (g_scene == SCENE_GAME) CPause::Update();
	if (CPause::GetPause())
	{
		g_pScene->SetMasterVolume();
		if (!g_bResolution)
		{
			return;
		}
		g_bResolution = false;
	}
	
	// --- ゲームモードによる分岐処理
	g_pScene->RootUpdate();

	// シーン切り替え判定 
	if (g_pScene->ChangeScene()) 
	{
		
		
		
		// 次のシーンの情報を取得 
		g_scene = g_pScene->NextScene();
		if(g_scene == SCENE_GAME)g_stage = g_pScene->GetStage();
		g_SceneType = (E_SCENE_TYPE)g_scene;
		// 現在のシーンを削除 
		g_hWnd = g_pScene->GethWnd();
		delete g_pScene;


		// シーンの切り替え 
		switch (g_scene)
		{
		case SCENE_TITLE:g_pScene = new CSceneTitle(g_pOption); break; // TITLE 
		case STAGE_SELECT: 
			g_pScene = new CStageSelect(); 
			break;
		case SCENE_GAME:g_pScene = new CSceneGame(g_stage);g_pDirection->SetTimer(4.5f); break; // GAME 
		case SCENE_RESULT:g_pScene = new CSceneResult(); break;
		case SCENE_DEBUGROOM:g_pScene = new CSceneDebug(); break;
		}

		// 次シーンに向けて初期設定 
		g_pFade->Start(true);   // フェード開始 
		g_pFade->SetFade(1.0f, true);
		g_pScene->SetFade(g_pFade); // フェードクラスをシーンに設定 
		g_pScene->SethWnd(g_hWnd);
		
		g_pScene->SetGameDirection(g_pDirection);
	}
	if (g_pFade->IsFinish())
	{
		g_pDirection->Update();
	}
}

void Draw()
{
	// --- 描画処理
	BeginDrawDirectX();

	// 軸線の表示
#ifdef _DEBUG
	//// グリッド
	//DirectX::XMFLOAT4 lineColor(0.5f, 0.5f, 0.5f, 1.0f);
	//float size = DEBUG_GRID_NUM * DEBUG_GRID_MARGIN;
	//for (int i = 1; i <= DEBUG_GRID_NUM; ++i)
	//{
	//	float grid = i * DEBUG_GRID_MARGIN;
	//	DirectX::XMFLOAT3 pos[2] = {
	//		DirectX::XMFLOAT3(grid, 0.0f, size),
	//		DirectX::XMFLOAT3(grid, 0.0f,-size),
	//	};
	//	Geometory::AddLine(pos[0], pos[1], lineColor);
	//	pos[0].x = pos[1].x = -grid;
	//	Geometory::AddLine(pos[0], pos[1], lineColor);
	//	pos[0].x = size;
	//	pos[1].x = -size;
	//	pos[0].z = pos[1].z = grid;
	//	Geometory::AddLine(pos[0], pos[1], lineColor);
	//	pos[0].z = pos[1].z = -grid;
	//	Geometory::AddLine(pos[0], pos[1], lineColor);
	//}
	//// 軸
	//Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(size, 0, 0), DirectX::XMFLOAT4(1, 0, 0, 1));
	//Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, size, 0), DirectX::XMFLOAT4(0, 1, 0, 1));
	//Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, size), DirectX::XMFLOAT4(0, 0, 1, 1));
	//Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(-size, 0, 0), DirectX::XMFLOAT4(0, 0, 0, 1));
	//Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, -size), DirectX::XMFLOAT4(0, 0, 0, 1));

	//Geometory::DrawLines();

	//// カメラの値
	//static bool camPosSwitch = false;
	//if (IsKeyTrigger(VK_RETURN)) {
	//	camPosSwitch ^= true;
	//}

	//DirectX::XMVECTOR camPos;
	//if (camPosSwitch) {
	//	camPos = DirectX::XMVectorSet(2.5f, 30.5f, -40.0f, 0.0f);
	//}
	//else {
	//	camPos = DirectX::XMVectorSet(2.5f, 3.5f, -4.0f, 0.0f);
	//}

	//// ジオメトリ用カメラ初期化
	//DirectX::XMFLOAT4X4 mat[2];
	//DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(
	//	DirectX::XMMatrixLookAtLH(
	//		camPos,
	//		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
	//		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	//	)));
	//DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(
	//	DirectX::XMMatrixPerspectiveFovLH(
	//		DirectX::XMConvertToRadians(60.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f)
	//));
	//Geometory::SetView(mat[0]);
	//Geometory::SetProjection(mat[1]);
#endif



	g_pScene->RootDraw();
	LibEffekseer::Draw();
	if (g_pFade->IsFinish())
	{
		g_pDirection->Draw();
	}

	if (CPause::GetPause())
	{
		CPause::Draw();
	}
	EndDrawDirectX();
}
// --- View情報のゲッター　XMFLOAT4*4
DirectX::XMFLOAT4X4 GetView(bool isTranspose)
{
	return g_Camera->GetViewMatrix(isTranspose);
}
// --- rojectionMatrix情報のゲッター　XMFLOAT4*4
DirectX::XMFLOAT4X4 GetProj(bool isTranspose)
{
	return g_Camera->GetProjectionMatrix(isTranspose);
}

DirectX::XMFLOAT4X4 Get2DWorld(bool isTranspose, DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size)
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f + pos.x, SCREEN_HEIGHT / 2.0f + pos.y, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(size.x, -size.y, 1.0f);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rotate.x, rotate.y, rotate.z, 0.0f));
	DirectX::XMMATRIX mat = S * R * T;
	if (isTranspose)mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMStoreFloat4x4(&world, mat);
	return world;
}

DirectX::XMFLOAT4X4 Get2DView(bool isTranspose)
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	if(isTranspose)mView = DirectX::XMMatrixTranspose(mView);
	DirectX::XMStoreFloat4x4(&view, mView);
	return view;
}

DirectX::XMFLOAT4X4 Get2DProj(bool isTranspose)
{
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 100.0f);
	mProj = DirectX::XMMatrixTranspose(mProj);
	DirectX::XMStoreFloat4x4(&proj, mProj);
	return proj;
}

DirectX::XMFLOAT3 GetCameraPos()
{
	return g_Camera->GetPos();
}

void SetCameraPos(DirectX::XMFLOAT3 look)
{
	g_Camera->SetCameraPos(look);
}

void SetCameraKind(CAMERA_KIND kind)
{
	SAFE_DELETE(g_Camera);
	switch (kind)
	{
	case NOMAL_CAMERA:
		g_Camera = g_pDebugCamera;
		g_nEvent = NOMAL_CAMERA;
		break;
	case EVENT_CAMERA:
		g_Camera = g_pEventCamera;
		g_nEvent = EVENT_CAMERA;
		break;
	default:
		MessageBox(NULL, "SetCameraKind out of range", "Main.cpp", MB_OK);
		break;
	}
}

void CameraEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time, CCameraEvent::EASE_KIND kind)
{
	g_pEventCamera->SetEvent(startPos, endPos, time, kind);
}

bool GetIsEvent()
{
	return g_pEventCamera->IsEvent();
}

bool GetCameraEvent()
{
	return g_nEvent == EVENT_CAMERA;
}


//void ChangeScene(E_SCENE_TYPE next, StageType StageType)
//{
//	g_pSourseTitleSE->FlushSourceBuffers();
//	if (g_pSourseTitleSE)SetVolumeSE(g_pSourseTitleSE);
//	g_pSourseTitleSE->Start();
//	//現在のシーンの終了
//	switch (g_SceneType)
//	{
//	case(SCENE_TITLE):UninitSceneTitle();	break;
//	case(STAGE_SELECT):delete g_pSceneSelect;
//		g_pSceneSelect = nullptr;
//		break;
//	case(SCENE_GAME):UninitSceneGame();		break;
//	case(SCENE_RESULT):UninitResult();		break;
//	case SCENE_DEBUGROOM:UninitSceneDebug(); break;
//	default:break;
//	}
//
//	//現在のシーンの更新
//	g_SceneType = next;
//
//	//次のシーンの初期化
//	switch (g_SceneType)
//	{
//	case(SCENE_TITLE):InitSceneTitle();				break;
//	case(STAGE_SELECT):g_pSceneSelect = new CStageSelect();			break;
//	case(SCENE_GAME):InitSceneGame(StageType);	break;
//	case(SCENE_RESULT):InitResult();				break;
//	case SCENE_DEBUGROOM:InitSceneDebug(); break;
//	default:break;
//	}
//}

E_SCENE_TYPE GetScene(void)
{
	return g_SceneType;
}

bool IsGaming()
{
	return IsGame;
}

void SetGameEnd()
{
	IsGame = false;
}

void SetRender2D()
{
	SetRenderTargets(1, &pRTV, nullptr);
}

void SetRender3D()
{
	SetRenderTargets(1, &pRTV, pDSV);
}
void InitResolusionMain()
{
	static int b = g_pOption->GetResolusion();
	if (g_scene == SCENE_GAME && b != g_pOption->GetResolusion())
	{
		bool fullscreen;
		if (g_pOption->GetIsFullScreen() == 1) fullscreen = false;
		else fullscreen = true;

		g_pScene->InitResolusion(g_NowWide, g_NowHeight, fullscreen);
		b = g_pOption->GetResolusion();
	}
	pRTV = GetDefaultRTV();
	pDSV = GetDefaultDSV();
	g_pFade = new CTransition();
	/*SAFE_DELETE(g_pDirection);
	g_pDirection = new CStartDirection();*/
	g_pDirection->ReloadTexture();
	g_pScene->SetGameDirection(g_pDirection);
	CPause::InitReload();
	g_pOption->InitResolusion();
	CPause::SetOption(g_pOption);
	if(g_scene == SCENE_GAME)g_bResolution = true;
}
void SetNowResolusion(int wide, int height)
{
	g_NowHeight = height;
	g_NowWide = wide;
}
int GetNowWide()
{
	return g_NowWide;
}
int GetNowHeight()
{
	return g_NowHeight;
}
void StartFade()
{
	g_pFade->Start(true);   // フェード開始 
	g_pFade->SetFade(1.0f, true);
}
bool IsFadeFinish()
{
	return g_pFade->IsFinish();
}
void SpriteDebug(DirectX::XMFLOAT3* pos, DirectX::XMFLOAT3* size, DirectX::XMFLOAT3* rotate, DirectX::XMFLOAT4* color, DirectX::XMFLOAT2* uvPos, DirectX::XMFLOAT2* uvSize, bool isModel)
{
	if (IsKeyPress(VK_LEFT))	pos->x++;
	if (IsKeyPress(VK_RIGHT))	pos->x--;
	if (IsKeyPress(VK_UP))		pos->y++;
	if (IsKeyPress(VK_DOWN))	pos->y--;
	if (IsKeyPress(VK_SPACE))	pos->z++;
	if (IsKeyPress(VK_SHIFT))	pos->z--;

	if (IsKeyPress('J'))		size->x++;
	if (IsKeyPress('L'))		size->x--;
	if (IsKeyPress('I'))		size->y++;
	if (IsKeyPress('K'))		size->y--;
	if (IsKeyPress('U'))		size->z++;
	if (IsKeyPress('O'))		size->z--;
	if (IsKeyPress('Z'))
	{
		size->x++;
		size->y++;
		size->z++;
	}
	if (IsKeyPress('X'))
	{
		size->x--;
		size->y--;
		size->z--;
	}

	if (IsKeyPress('E'))		rotate->z -= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('A'))		rotate->x += DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('D'))		rotate->x -= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('W'))		rotate->y += DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('S'))		rotate->y -= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('Q'))		rotate->z += DirectX::XMConvertToRadians(1.0f);

	if (IsKeyPress('1') && IsKeyPress(VK_SHIFT))		color->x -= 5;
	else if (IsKeyPress('1'))		color->x += 5;
	if (IsKeyPress('2') && IsKeyPress(VK_SHIFT))		color->y -= 5;
	else if (IsKeyPress('2'))		color->y += 5;
	if (IsKeyPress('3') && IsKeyPress(VK_SHIFT))		color->z -= 5;
	else if (IsKeyPress('3'))		color->z += 5;
	if (IsKeyPress('4') && IsKeyPress(VK_SHIFT))		color->w -= 5;
	else if (IsKeyPress('4'))		color->w += 5;

	if (IsKeyPress('6') && IsKeyPress(VK_SHIFT))		uvPos->x -= 0.05;
	else if (IsKeyPress('6'))		uvPos->x += 0.05;
	if (IsKeyPress('7') && IsKeyPress(VK_SHIFT))		uvPos->y -= 0.05;
	else if (IsKeyPress('7'))		uvPos->y += 0.05;
	if (IsKeyPress('8') && IsKeyPress(VK_SHIFT))		uvSize->x -= 0.05;
	else if (IsKeyPress('8'))		uvSize->x += 0.05;
	if (IsKeyPress('9') && IsKeyPress(VK_SHIFT))		uvSize->y -= 0.05;
	else if (IsKeyPress('9'))		uvSize->y += 0.05;

	static int a = 0;

	if (IsKeyPress('0'))
	{
		a++;
		if (a >= 120)
		{
			pos->x = 0.0f;
			pos->y = 0.0f;
			pos->z = 0.0f;
			size->x = 1.0f;
			size->y = 1.0f;
			size->z = 1.0f;
			rotate->x = 0.0f;
			rotate->y = 0.0f;
			rotate->z = 0.0f;
			color->x = 1.0f;
			color->y = 1.0f;
			color->z = 1.0f;
			color->w = 1.0f;
			uvPos->x = 0.0f;
			uvPos->y = 0.0f;
			uvSize->x = 1.0f;
			uvSize->y = 1.0f;
			a = 0;
		}
	}
	else a = 0;
}

void SpriteDebug(ObjectParam* param, bool isModel)
{
	if (IsKeyPress(VK_LEFT))	param->pos.x++;
	if (IsKeyPress(VK_RIGHT))	param->pos.x--;
	if (IsKeyPress(VK_UP))		param->pos.y++;
	if (IsKeyPress(VK_DOWN))	param->pos.y--;
	if (IsKeyPress(VK_SPACE))	param->pos.z++;
	if (IsKeyPress(VK_SHIFT))	param->pos.z--;

	if (!isModel)
	{
		if (IsKeyPress('J'))		param->size.x++;
		if (IsKeyPress('L'))		param->size.x--;
		if (IsKeyPress('I'))		param->size.y++;
		if (IsKeyPress('K'))		param->size.y--;
		if (IsKeyPress('U'))		param->size.z++;
		if (IsKeyPress('O'))		param->size.z--;
		if (IsKeyPress('Z'))
		{
			param->size.x++;
			param->size.y++;
			param->size.z++;
		}
		if (IsKeyPress('X'))
		{
			param->size.x--;
			param->size.y--;
			param->size.z--;
		}
	}
	else
	{
		if (IsKeyPress('J'))		param->size.x+= 0.1f;
		if (IsKeyPress('L'))		param->size.x-= 0.1f;
		if (IsKeyPress('I'))		param->size.y+= 0.1f;
		if (IsKeyPress('K'))		param->size.y-= 0.1f;
		if (IsKeyPress('U'))		param->size.z+= 0.1f;
		if (IsKeyPress('O'))		param->size.z-= 0.1f;
		if (IsKeyPress('Z'))
		{
			param->size.x+= 0.1f;
			param->size.y+= 0.1f;
			param->size.z+= 0.1f;
		}
		if (IsKeyPress('X'))
		{
			param->size.x-= 0.1f;
			param->size.y-= 0.1f;
			param->size.z-= 0.1f;
		}
	}

	if (IsKeyPress('A'))		param->rotate.x += DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('D'))		param->rotate.x -= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('W'))		param->rotate.y += DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('S'))		param->rotate.y -= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('Q'))		param->rotate.z += DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('E'))		param->rotate.z -= DirectX::XMConvertToRadians(1.0f);

	if (IsKeyPress('1') && IsKeyPress(VK_SHIFT))		param->color.x -= 5;
	else if (IsKeyPress('1'))							param->color.x += 5;
	if (IsKeyPress('2') && IsKeyPress(VK_SHIFT))		param->color.y -= 5;
	else if (IsKeyPress('2'))							param->color.y += 5;
	if (IsKeyPress('3') && IsKeyPress(VK_SHIFT))		param->color.z -= 5;
	else if (IsKeyPress('3'))							param->color.z += 5;
	if (IsKeyPress('4') && IsKeyPress(VK_SHIFT))		param->color.w -= 5;
	else if (IsKeyPress('4'))							param->color.w += 5;

	if (IsKeyPress('6') && IsKeyPress(VK_SHIFT))		param->uvPos.x -= 0.05;
	else if (IsKeyPress('6'))							param->uvPos.x += 0.05;
	if (IsKeyPress('7') && IsKeyPress(VK_SHIFT))		param->uvPos.y -= 0.05;
	else if (IsKeyPress('7'))							param->uvPos.y += 0.05;
	if (IsKeyPress('8') && IsKeyPress(VK_SHIFT))		param->uvSize.x -= 0.05;
	else if (IsKeyPress('8'))							param->uvSize.x += 0.05;
	if (IsKeyPress('9') && IsKeyPress(VK_SHIFT))		param->uvSize.y -= 0.05;
	else if (IsKeyPress('9'))							param->uvSize.y += 0.05;

	static int a = 0;

	if (IsKeyPress('0'))
	{
		a++;
		if (a >= 120)
		{
			param->pos.x = 0.0f;
			param->pos.y = 0.0f;
			param->pos.z = 0.0f;
			param->size.x = 1.0f;
			param->size.y = 1.0f;
			param->size.z = 1.0f;
			param->rotate.x = 0.0f;
			param->rotate.y = 0.0f;
			param->rotate.z = 0.0f;
			param->color.x = 1.0f;
			param->color.y = 1.0f;
			param->color.z = 1.0f;
			param->color.w = 1.0f;
			param->uvPos.x = 0.0f;
			param->uvPos.y = 0.0f;
			param->uvSize.x = 1.0f;
			param->uvSize.y = 1.0f;
			a = 0;
		}
	}
	else a = 0;
}


// EOF