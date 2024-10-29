#include "DirectXPlus.h"

FLOAT3 POS;
FLOAT3 SCL;
FLOAT3 ROT;

DirectX::XMMATRIX Translation;
DirectX::XMMATRIX Scaling;
DirectX::XMMATRIX Rotation;

void InitDirectXPlus()
{
	/* グローバル変数の初期化 */
	POS.X = POS.Y = POS.Z = 0.0f;
	SCL.X = SCL.Y = SCL.Z = 1.0f;
	ROT.X = ROT.Y = ROT.Z = 0.0f;
}

/* 移動行列 */
void SetTranslation(float X, float Y, float Z)
{
	POS.X = X;
	POS.Y = Y;
	POS.Z = Z;
	Translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(POS.X, POS.Y, POS.Z, 0.0f));
	Scaling = DirectX::XMMatrixScaling(SCL.X, SCL.Y, SCL.Z);
	Rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(ROT.X, ROT.Y, ROT.Z, 0.0f));
	DirectX::XMMATRIX mat = Scaling * Rotation * Translation;
	mat = XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);

	
}

/* 拡大行列 */
void SetScaling(float X, float Y, float Z)
{
	SCL.X = X;
	SCL.Y = Y;
	SCL.Z = Z;
	Translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(POS.X, POS.Y, POS.Z, 0.0f));
	Scaling = DirectX::XMMatrixScaling(SCL.X, SCL.Y, SCL.Z);
	Rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(ROT.X, ROT.Y, ROT.Z, 0.0f));
	DirectX::XMMATRIX mat = Scaling * Rotation * Translation;
	mat = XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);

}

/* 回転行列 */
void SetRotation(float X, float Y, float Z)
{
	ROT.X = X;
	ROT.Y = Y;
	ROT.Z = Z;
	Translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(POS.X, POS.Y, POS.Z, 0.0f));
	Scaling = DirectX::XMMatrixScaling(SCL.X, SCL.Y, SCL.Z);
	Rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(ROT.X, ROT.Y, ROT.Z, 0.0f));
	DirectX::XMMATRIX mat = Scaling * Rotation * Translation;
	mat = XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);
}

/* 移動行列 */
void SetTranslation(FLOAT3 SetTranslation)
{
	POS.X = SetTranslation.X;
	POS.Y = SetTranslation.Y;
	POS.Z = SetTranslation.Z;
	Translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(POS.X, POS.Y, POS.Z, 0.0f));
	Scaling = DirectX::XMMatrixScaling(SCL.X, SCL.Y, SCL.Z);
	Rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(ROT.X, ROT.Y, ROT.Z, 0.0f));
	DirectX::XMMATRIX mat = Scaling * Rotation * Translation;
	mat = XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);
}

/* 拡大行列 */
void SetScaling(FLOAT3 SetScaling)
{
	SCL.X = SetScaling.X;
	SCL.Y = SetScaling.Y;
	SCL.Z = SetScaling.Z;
	Translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(POS.X, POS.Y, POS.Z, 0.0f));
	Scaling = DirectX::XMMatrixScaling(SCL.X, SCL.Y, SCL.Z);
	Rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(ROT.X, ROT.Y, ROT.Z, 0.0f));
	DirectX::XMMATRIX mat = Scaling * Rotation * Translation;
	mat = XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);
}

/* 回転行列 */
void SetRotation(FLOAT3 SetRotation)
{
	ROT.X = SetRotation.X;
	ROT.Y = SetRotation.Y;
	ROT.Z = SetRotation.Z;
	Translation = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(POS.X, POS.Y, POS.Z, 0.0f));
	Scaling = DirectX::XMMatrixScaling(SCL.X, SCL.Y, SCL.Z);
	Rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(ROT.X, ROT.Y, ROT.Z, 0.0f));
	DirectX::XMMATRIX mat = Scaling * Rotation * Translation;
	mat = XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);
}

/* リセット */
void ResetData()
{
	/* 初期値 */
	POS.X = POS.Y = POS.Z = 0.0f;
	SCL.X = SCL.Y = SCL.Z = 1.0f;
	ROT.X = ROT.Y = ROT.Z = 0.0f;
}

