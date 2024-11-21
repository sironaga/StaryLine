#include "SceneGame.h"
#include "Geometory.h"
#include "Defines.h"
#include "ShaderList.h"
#include "Camera.h"
#include"CameraDebug.h"

Camera* m_pCamera;
char MapData[19][19];
float MoveX, MoveY;


SceneGame::SceneGame()
{
	
	RenderTarget* pRTV = GetDefaultRTV(); // デフォルトのRenderTargetViewを取得 
	DepthStencil* pDSV = GetDefaultDSV(); // デフォルトのDepthStencilViewを取得 
	SetRenderTargets(1, &pRTV, pDSV);  // 第3引数がnullの場合、2D表示となる

}

SceneGame::~SceneGame()
{


}

void SceneGame::Update()
{
	
}

void SceneGame::Draw()
{
	
}
