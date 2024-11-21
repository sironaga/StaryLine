#pragma once

#include"ShaderList.h"
#include"Model.h"
#include "Defines.h"
class CModelEx
{
public:
	CModelEx(const char *ModelFile);
	~CModelEx();
	void Draw();
	void SetViewMatrix(DirectX::XMFLOAT4X4 Camera);
	void SetProjection(DirectX::XMFLOAT4X4 Camera);
	void SetScale(float X, float Y, float Z);
	void SetPostion(float X, float Y, float Z);
	void SetRotation(float X, float Y, float Z);
private:
	DirectX::XMMATRIX T;
	DirectX::XMMATRIX S;
	DirectX::XMMATRIX R;
	DirectX::XMMATRIX mat;
	DirectX::XMVECTOR EyePosition;
	DirectX::XMVECTOR FocusPosition;
	DirectX::XMVECTOR UpDirection;
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world, view, proj;
	float sX, sY, sZ;
	float tX, tY, tZ;
	DirectX::XMFLOAT4X4 ViewMatrix;
	DirectX::XMFLOAT4X4 ProjectionMatric;
	Model *CModel;
	


};

