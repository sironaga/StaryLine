#pragma once

#include "ModelEx.h"
#include "Defines.h"
#include "Camera.h"

class CDebugRoom
{
public:
	CDebugRoom();
	~CDebugRoom();
	void Update();
	void Draw();

	enum DEBUG_MODEL
	{
		TEST1 = 0,

		MAX_DEBUGMODEL
	};
private:
	CModelEx* m_pDebugModel[MAX_DEBUGMODEL];
	Camera* m_pCamera;
	int m_nSelect;
	DirectX::XMFLOAT3 m_tPos;
	DirectX::XMFLOAT3 m_tSize;
	DirectX::XMFLOAT3 m_tRotate;
};