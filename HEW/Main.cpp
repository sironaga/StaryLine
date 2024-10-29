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
		2024/10/29 秋野翔太　Modelの動作確認完了
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
#include"DirectXPlus.h"

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
FLOAT3 g_fCamAngle;/*カメラのアングル*/
CameraRay camRay;
CameraPosition camerapos;
CameraMovePosition cameramovepos;
bool first = true;
bool MoveAngle = false;



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
	f_Time.oldTime = f_Time.time = timeGetTime();

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
	CamPos_Debug();			/*カメラの移動を行っている関数*/

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
		camPos = DirectX::XMVectorSet(2.5f, 30.5f, -40.0f, 0.0f);
	}
	else {
		camPos = DirectX::XMVectorSet(camerapos.posx, camerapos.posy, camerapos.posz, 0.0f);
	}



	// ジオメトリ用カメラ初期化
	DirectX::XMFLOAT4X4 mat[2];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			camPos,
			DirectX::XMVectorSet(camRay.posx, camRay.posy, camRay.posz, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		)));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(60.0f), (float)Windows_Size_X / Windows_Size_Y, 0.1f, 100.0f)
	));
	//移動、回転
//移動行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cameramovepos.posx, cameramovepos.posy, cameramovepos.posz);
	//ビュー行列を作成
	DirectX::XMMATRIX A = DirectX::XMMatrixLookAtLH(
		camPos,
		DirectX::XMVectorSet(camRay.posx, camRay.posy, camRay.posz, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	//回転行列を作成
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(camRay.posx);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(camRay.posy);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(camRay.posz);
	DirectX::XMMATRIX matr = Rx * Ry * Rz; // それぞれの行列を掛け合わせて格納 
	//ビュー行列に回転行列を掛ける
	A = A * matr;
	//ビュー行列に移動行列を掛けて、元のビュー行列に新しいビュー行列を代入
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(A * T));
	Geometory::SetView(mat[0]);
	Geometory::SetProjection(mat[1]);
#endif
}

void CamPos_Debug()
{
	if (IsKeyPress('W'))
	{
		if (!MoveAngle)
		{
			cameramovepos.posz -= 0.1f;
		}
		else
		{
			camRay.posx += 0.01f;
		}
	}
	if (IsKeyPress('S'))
	{
		if (!MoveAngle)
		{
			cameramovepos.posz += 0.1f;
		}
		else
		{
			camRay.posx -= 0.01f;
		}
	}
	if (IsKeyPress('A'))
	{
		if (!MoveAngle)
		{
			cameramovepos.posx += 0.1f;
		}
		else
		{
			camRay.posy += 0.01f;
		}
	}
	if (IsKeyPress('D'))
	{
		if (!MoveAngle)
		{
			cameramovepos.posx -= 0.1f;
		}
		else
		{
			camRay.posy -= 0.01f;
		}
	}
	if (IsKeyTrigger(VK_SHIFT) && !MoveAngle)
	{
		MoveAngle = true;
	}
	else if (IsKeyTrigger(VK_SHIFT) && MoveAngle)
	{
		MoveAngle = false;
	}
	if (IsKeyPress('I')) camRay.posz += 0.01f;
	if (IsKeyPress('J'))camRay.posz -= 0.01f;
}