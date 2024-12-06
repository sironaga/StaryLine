#pragma once

#include "ModelEx.h"
#include "Defines.h"
#include "Camera.h"
#include "Field.h"

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
		TEST2,

		MAX_DEBUGMODEL
	};
private:
	CModelEx* m_pDebugModel[MAX_DEBUGMODEL];
	Field* m_pField;
	int m_nSelect;
	bool m_bSelect;
	DirectX::XMFLOAT3 m_tPos;
	DirectX::XMFLOAT3 m_tSize;
	DirectX::XMFLOAT3 m_tRotate;
};