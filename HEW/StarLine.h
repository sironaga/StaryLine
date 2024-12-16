#pragma once

#include"ModelEx.h"
#include"Geometory.h"
#include "Camera.h"


class StarLine
{
public:
	StarLine();
	~StarLine();
	void SetLineInfo(DirectX::XMFLOAT3 StartPosLeft, DirectX::XMFLOAT3 StartPosRight, DirectX::XMFLOAT3 NowPosLeft, DirectX::XMFLOAT3 NowPosRight);
	void DispLine();


private:
	DirectX::XMFLOAT3 Pos; // --- position
	DirectX::XMFLOAT3 Scl; // --- Scale
	DirectX::XMFLOAT3 Rot; // --- Rotation
	DirectX::XMFLOAT4X4 world; // --- World Position

	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Proj;
private:
	Geometory* m_pBox;
	CModelEx *m_pModel;
};

