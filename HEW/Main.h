#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>
#include"DirectX.h"


HRESULT Init(HWND hWnd, UINT width, UINT height);
void Uninit();
void Update();
void Draw();
DirectX::XMMATRIX GetView();
DirectX::XMMATRIX GetProj();


//列挙型宣言
enum E_SCENE_TYPE
{
	SCENE_TITLE = 0,
	STAGE_SELECT,
	SCENE_GAME,
	SCENE_RESULT,


	SCENE_DEBUGROOM,
	SCENE_MAX
};

//プロトタイプ宣言
void ChangeScene(E_SCENE_TYPE next);	//シーン遷移を行う関数

#endif // __MAIN_H__