#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>
#include"DirectX.h"


HRESULT Init(HWND hWnd, UINT width, UINT height);
void Uninit();
void Update();
void Draw();
DirectX::XMFLOAT4X4 GetView(bool isTranspose = true);
DirectX::XMFLOAT4X4 GetProj(bool isTranspose = true);
DirectX::XMFLOAT4X4 Get2DView(bool isTranspose = true);
DirectX::XMFLOAT4X4 Get2DProj(bool isTranspose = true);
DirectX::XMFLOAT3 GetCameraPos();


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

struct StageType
{
	int StageMainNumber;
	int StageSubNumber;
};

//プロトタイプ宣言
void ChangeScene(E_SCENE_TYPE next);	//シーン遷移を行う関数
void ChangeScene(E_SCENE_TYPE next, StageType StageType);	//シーン遷移を行う関数

E_SCENE_TYPE GetScene(void);

bool IsGaming();
void SetGameEnd();
void SetRender2D();
void SetRender3D();



#endif // __MAIN_H__