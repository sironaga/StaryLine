/*
	動作環境　		Windows DirectX 11
	プロジェクト	未定
	リードプログラマー
		中島聖羅
	システムプログラマー
		吉田京志郎
	Log
		2024/10/05 秋野翔太　基本的なDirectXに必要な処理の追加
		
*/

/* Include */
#include"DirectX.h"

/* Global */


/* prototype */
void Update();	/* 更新処理用の関数 */
void Draw();	/* 描画処理用の関数 */

/*  */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ALLInit(hInstance, nCmdShow); /* DirectX及び周辺関数の初期化 DirectX.h */

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

				KeyboardUpdate();
				SetFreamTime();
				BeginDrawDirectX();
				


			
				EndDrawDirectX();

				f_Time.oldTime = f_Time.time;
			}
		}


	}
}

void Update()
{

}

void Draw()
{

}

