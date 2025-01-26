#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>
#include "DirectX.h"
#include "Defines.h"
#include "_StructList.h"
#include "Option.h"
#include "CameraEvent.h"

HRESULT Init(HWND hWnd, UINT width, UINT height);
void Uninit();
void Update();
void Draw();
DirectX::XMFLOAT4X4 GetView(bool isTranspose = true);
DirectX::XMFLOAT4X4 GetProj(bool isTranspose = true);
DirectX::XMFLOAT4X4 Get2DWorld(bool isTranspose = true, DirectX::XMFLOAT3 rotate = {}, DirectX::XMFLOAT2 pos = { 0.0f,0.0f }, DirectX::XMFLOAT2 size = { 1.0f, 1.0f});
DirectX::XMFLOAT4X4 Get2DView(bool isTranspose = true);
DirectX::XMFLOAT4X4 Get2DProj(bool isTranspose = true);
DirectX::XMFLOAT3 GetCameraPos();
enum CAMERA_KIND
{
	NOMAL_CAMERA,
	EVENT_CAMERA,

	MAX_CAMERA
};
void SetCameraKind(CAMERA_KIND kind);
void CameraEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time, CCameraEvent::EASE_KIND kind);
bool GetIsEvent();
bool GetCameraEvent();

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
void InitResolusionMain();
void SetNowResolusion(int wide,int height);
int GetNowWide();
int GetNowHeight();
void StartFade();
bool IsFadeFinish();

void SpriteDebug(DirectX::XMFLOAT3* pos, DirectX::XMFLOAT3* size, DirectX::XMFLOAT3* rotate, DirectX::XMFLOAT4* color,  DirectX::XMFLOAT2* uvPos, DirectX::XMFLOAT2* uvSize,bool isModel = false);
void SpriteDebug(ObjectParam* param ,bool isModel = false);


#endif // __MAIN_H__