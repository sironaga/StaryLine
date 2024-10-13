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
*/

/* Include */
#include"DirectX.h"
#include"Main.h"
#include"Controller.h"
/* define */


/* Global */	
int g_nSoundVolume; /* ゲーム全体の音量変数 */
Screen g_Screen;	/* モード管理用変数 */

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
				
				Controller_Update();

				Update();	/* 更新処理 */
				Draw();		/* 描画処理 */
			
				EndDrawDirectX();

				f_Time.oldTime = f_Time.time;
			}
		}


	}
}

void Update()
{
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
	switch (g_Screen)
	{
	case TITLE:
		break;
	default:
		break;
	}
}

/*  */

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