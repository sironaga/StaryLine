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

#include "LibEffekseer.h"
#include "SoundList.h"

#include "Fade.h"
#include "FadeBlack.h"

#include "Scene.h"
#include "SceneTitle.h"
#include "StageSelect.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneDebug.h"

//--- グローバル変数
CStageSelect* g_pSceneSelect;
Camera* g_Camera;
IXAudio2SourceVoice* g_pSourseTitleSE;
RenderTarget* pRTV;
DepthStencil* pDSV;
CSoundList* g_mainsound;
bool IsGame = true;
CScene* g_pScene; // シーン 
CFade* g_pFade; // フェード 
HWND g_hWnd;
E_SCENE_TYPE g_SceneType;
int g_NowWide = 1920;
int g_NowHeight = 1080;

HRESULT Init(HWND hWnd, UINT width, UINT height)
{

	HRESULT hr;
	// DirectX初期化
	hr = InitDirectX(hWnd, width, height, false);
	if (FAILED(hr)) { return hr; }

	// 他機能初期化
	g_Camera = new CameraDebug();
	Geometory::Init();
	Sprite::Init();
	LibEffekseer::Init(GetDevice(), GetContext());
	InitInput();
	ShaderList::Init();
	InitSpriteDrawer(GetDevice(), GetContext(), SCREEN_WIDTH, SCREEN_HEIGHT);
	srand(timeGetTime());

	pRTV = GetDefaultRTV();
	pDSV = GetDefaultDSV();

	// フェード作成 
	g_pFade = new CFadeBlack();
	g_pFade->SetFade(1.0f, true);

	// シーン作成 
	g_pScene = new CSceneTitle();
	g_pScene->SetFade(g_pFade); // シーンに使用するフェードを設定 

	g_mainsound = new CSoundList(SE_DECISION);
	g_pSourseTitleSE = g_mainsound->GetSound(false);
	
	g_pScene->SethWnd(hWnd);
	
	return hr;
}

void Uninit()
{
	SAFE_DELETE(g_pScene);
	SAFE_DELETE(g_pFade);

	UninitSpriteDrawer();
	ShaderList::Uninit();
	UninitInput();

	//delete g_mainsound;
	//g_mainsound = nullptr;

	LibEffekseer::Uninit();
	Sprite::Uninit();
	Geometory::Uninit();
	UninitDirectX();

	//g_pSourseTitleSE = nullptr;
}

void Update()
{
	// --- 入力情報の更新
	Controller_Update();
	UpdateInput();
	// --- カメラ情報の更新
	g_Camera->Update();
	// --- ゲームモードによる分岐処理
	g_pScene->RootUpdate();

	// シーン切り替え判定 
	if (g_pScene->ChangeScene()) 
	{
		g_hWnd = g_pScene->GethWnd();
		g_mainsound->SetMasterVolume();
		g_pSourseTitleSE->FlushSourceBuffers();
		if (g_pSourseTitleSE)SetVolumeSE(g_pSourseTitleSE);
		g_pSourseTitleSE->Start();
		StageType stage = {};
		// 次のシーンの情報を取得 
		int scene = g_pScene->NextScene();
		if(scene == SCENE_GAME)stage = g_pScene->GetStage();
		g_SceneType = (E_SCENE_TYPE)scene;
		// 現在のシーンを削除 
		delete g_pScene;


		// シーンの切り替え 
		switch (scene)
		{
		case SCENE_TITLE:g_pScene = new CSceneTitle(); break; // TITLE 
		case STAGE_SELECT: g_pScene = new CStageSelect(); break;
		case SCENE_GAME:g_pScene = new CSceneGame(stage); break; // GAME 
		case SCENE_RESULT:g_pScene = new CSceneResult(); break;
		case SCENE_DEBUGROOM:g_pScene = new CSceneDebug(); break;
		}

		// 次シーンに向けて初期設定 
		g_pFade->Start(true);   // フェード開始 
		g_pScene->SetFade(g_pFade); // フェードクラスをシーンに設定 
		g_pScene->SethWnd(g_hWnd);
	}
}

void Draw()
{
	// --- 描画処理
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
	Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(size, 0, 0), DirectX::XMFLOAT4(1, 0, 0, 1));
	Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, size, 0), DirectX::XMFLOAT4(0, 1, 0, 1));
	Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, size), DirectX::XMFLOAT4(0, 0, 1, 1));
	Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(-size, 0, 0), DirectX::XMFLOAT4(0, 0, 0, 1));
	Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, -size), DirectX::XMFLOAT4(0, 0, 0, 1));

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
	DirectX::XMFLOAT4X4 mat[2];
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


	g_pScene->RootDraw();
	LibEffekseer::Draw();

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

DirectX::XMFLOAT4X4 Get2DWorld(bool isTranspose, DirectX::XMFLOAT3 rotate)
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);
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
	pRTV = GetDefaultRTV();
	pDSV = GetDefaultDSV();
	g_pFade = new CFadeBlack();
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
// EOF