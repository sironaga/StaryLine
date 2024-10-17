/*
	動作環境　		Windows DirectX 11
	プロジェクト	未定
	リードプログラマー
		中島聖羅
	システムプログラマー
		吉田京志郎
	Log
		2024/10/05 秋野翔太　基本的なDirectXに必要な処理の追加
		2024/10/07 秋野翔太　g_SoundVolume変数の追加それに伴いセッターゲッターの追加
		2024/10/16 秋野翔太　DirectXを3Dに対応　プロジェクト一部変更
*/

/* Include */
#include"Main.h"
#include<memory>
#include"DirectX.h"
#include"Geometory.h"
#include"Sprite.h"
#include"Input.h"
#include"ShaderList.h"
#include"Controller.h"

/* define */
#define Windows_Size_X (1920)
#define Windows_Size_Y (1080)
#define Windows_FullScreen (false)
#define Windows_Name "HalEventWeek"

/* Global */	
int g_nSoundVolume; /* ゲーム全体の音量変数 */
Screen g_Screen;	/* モード管理用変数 */
HWND hWnd;
WNDCLASSEX wcex;;
FLOAT3 g_fCamPos; /* カメラのポジション */


/* prototype */
void Init(HINSTANCE InhInstance, int InCmd);	/* システムの初期化 */
void Update();	/* 更新処理用の関数 */
void Draw();	/* 描画処理用の関数 */
void UnInit(HINSTANCE InhInstance);	/* 終了処理 */
void Draw_Debug(); /* Draw Debug用 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* ウィンドウプロシージャー */
void CreatWindows(HINSTANCE InhInstance, int InCmd);								/* ウインドウ制作 */
void CamPos_Debug();


/*  */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Init(hInstance,nCmdShow);

	g_Screen = TITLE;

	FPSTIMER f_Time; /* フレーム制御用構造体 */
	f_Time.FPS = 0;
	f_Time.fpsCount = 0;
	f_Time.fpsTime = 0;
	f_Time.oldTime = 0;
	f_Time.time = 0;

	MSG message;

	while (1)
	{

		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&message, NULL, 0, 0)) {
				break;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			f_Time.time = timeGetTime();

			if (f_Time.time - f_Time.oldTime >= 1000 / 60)
			{

				
				
				Controller_Update();

				Update();	/* 更新処理 */
				Draw();		/* 描画処理 */
			
			

				f_Time.oldTime = f_Time.time;
			}
		}


	}
	UnInit(hInstance);
	return 0;
}

void Init(HINSTANCE InhInstance,int InCmd)
{
	CreatWindows(InhInstance, InCmd);
	InitDirectX(hWnd, Windows_Size_X, Windows_Size_Y, Windows_FullScreen);
	InitInput();	/*キーボードの初期化*/
	Geometory::Init();
	Sprite::Init();
	ShaderList::Init();
}

void Update()
{
	Controller_Update();	/*コントローラーの更新*/
	UpdateInput();			/*キーボードの更新*/
	CamPos_Debug();

	switch (g_Screen)
	{
	case TITLE:
		break;
	default:
		break;
	}
}

void Draw()
{
	BeginDrawDirectX();

#ifdef _DEBUG
	Draw_Debug();
#endif
	Geometory::DrawBox();
	switch (g_Screen)
	{
	case TITLE:
		break;
	default:
		break;
	}

	EndDrawDirectX();
}

void UnInit(HINSTANCE InhInstance)
{
	timeEndPeriod(1);
	XInputEnable(false); // XInputの無効化
	UninitInput();
	UninitDirectX();
	UnregisterClass(wcex.lpszMenuName, InhInstance);
}

/* === seting === */

void SetVolume(int InVolume)
{
	g_nSoundVolume = InVolume;
}

int GetVolume()
{
	return g_nSoundVolume;
}

void SetScreen(Screen InScreen)
{
	/* 現在使用しているクラスを閉じる用Switch文 */
	switch (g_Screen)
	{
	case TITLE:
		break;
	default:
		/* クラスを使用していないモードの場合Defaultが呼び出されるようにしてください */
		break;
	}

	g_Screen = InScreen;

	/* 変更先のモードのクラスをnewする用Switch文 */
	switch (g_Screen)
	{
	case TITLE:
		break;
	default:
		/* クラスを使用していないモードの場合Defaultが呼び出されるようにしてください */
		break;
	}

}

/* === system === */

/* DirectXの初期化 */
void CreatWindows(HINSTANCE InhInstance , int InCmd)
{

	// ウィンドクラス情報の設定
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = InhInstance;
	wcex.lpszClassName = "Class Name";
	wcex.lpfnWndProc = WndProc;
	wcex.style = CS_CLASSDC | CS_DBLCLKS;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// ウィンドウクラス情報の登録
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to RegisterClassEx", "Error", MB_OK);
		return ;
	}

	// ウィンドウの作成
	RECT rect = { 0, 0, Windows_Size_X, Windows_Size_Y };
	DWORD style = WS_CAPTION | WS_SYSMENU;
	DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&rect, style, false, exStyle);
	hWnd = CreateWindowEx(
		exStyle, wcex.lpszClassName,
		Windows_Name, style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		HWND_DESKTOP,
		NULL, InhInstance, NULL
	);

	// ウィンドウの表示
	ShowWindow(hWnd, InCmd);
	UpdateWindow(hWnd);
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/* DEBUG */

void Draw_Debug()
{
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
		camPos = DirectX::XMVectorSet(2.5f + g_fCamPos.X, 30.5f + g_fCamPos.Y, -40.0f + g_fCamPos.Z, 0.0f);
	}
	else {
		camPos = DirectX::XMVectorSet(2.5f + g_fCamPos.X, 3.5f + g_fCamPos.Y, -4.0f + g_fCamPos.Z, 0.0f);
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
			DirectX::XMConvertToRadians(60.0f), (float)Windows_Size_X / Windows_Size_Y, 0.1f, 100.0f)
	));
	Geometory::SetView(mat[0]);
	Geometory::SetProjection(mat[1]);
#endif
}

void CamPos_Debug()
{
	if (IsKeyPress('A'))
	{
		g_fCamPos.X -= 0.1f;
	}
	if (IsKeyPress('D'))
	{
		g_fCamPos.X += 0.1f;
	}
	if (IsKeyPress('W'))
	{
		g_fCamPos.Z += 0.1f;
	}
	if (IsKeyPress('S'))
	{
		g_fCamPos.Z -= 0.1f;
	}
	if (IsKeyPress(VK_SPACE))
	{
		g_fCamPos.Y += 0.1f;
	}
	if (IsKeyPress(VK_SHIFT))
	{
		g_fCamPos.Y -= 0.1f;
	}
}