#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Model.h"
#include "Camera.h"
#include"CameraDebug.h"
#include"ModelEx.h"
class SceneGame
{
public:
	SceneGame();
	~SceneGame();
	void Update();
	void Draw();

private:
	Camera* m_pCamera;
	CModelEx* pMode;
	CModelEx* pWood;
	CModelEx* pTree;
	CModelEx* pBall;
	CModelEx* pTree2;
	CModelEx* pFlag;
	DirectX::XMFLOAT4 Light;
	DirectX::XMFLOAT3 Pos;
	bool bTime;
};

#endif // __SCENE_GAME_H__