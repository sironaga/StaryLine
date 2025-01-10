#pragma once

#include "Model.h"
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

		MAX_DEBUGMODEL
	};
private:
	struct param
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Size;
		DirectX::XMFLOAT3 Rotate;

	}m_Param;
	void DrawModel(Model* model,param InParam,bool isAnim = false);
	Model* m_pDebugModel[MAX_DEBUGMODEL];
	Field* m_pField;
	int m_nSelect;
	bool m_bSelect;
};