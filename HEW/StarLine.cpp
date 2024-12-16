#include "StarLine.h"
#include <cmath>

#define LINE_DEBUG (true)

StarLine::StarLine()
	:Pos{},Scl{},Rot{},world{}
{
	m_pModel = new CModelEx("aaaa", false);
	m_pBox = new Geometory();
	



}

StarLine::~StarLine()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

void StarLine::SetLineInfo(DirectX::XMFLOAT3 StartPosLeft, DirectX::XMFLOAT3 StartPosRight, DirectX::XMFLOAT3 NowPosLeft, DirectX::XMFLOAT3 NowPosRight)
{
	
	// --- 地点Sの中心座標を計算
	DirectX::XMFLOAT3 StartCenter =
	{ 
		(StartPosLeft.x + StartPosRight.x) / 2.0f,
		(StartPosLeft.y + StartPosRight.y) / 2.0f,
		(StartPosLeft.z + StartPosRight.z) / 2.0f 
	};
	// --- 地点Nの中心座標を計算
	DirectX::XMFLOAT3 NowCenter =
	{
		(NowPosLeft.x + NowPosRight.x) / 2.0f,
		(NowPosLeft.y + NowPosRight.y) / 2.0f,
		(NowPosLeft.z + NowPosRight.z) / 2.0f 
	};
	// ---地点Sと地点Nの中心座標を計算
	Pos.x = ((StartCenter.x + NowCenter.x) / 2.0f);
	Pos.y = ((StartCenter.y + NowCenter.y) / 2.0f);
	Pos.z = ((StartCenter.y + NowCenter.y) / 2.0f);
	// --- 座標の変換
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		Pos.x,
		Pos.y,
		Pos.z,
		0.0f
	));
	// --- 差分の計算
	DirectX::XMFLOAT3 delta;
	delta.x = StartCenter.x - NowCenter.x;
	delta.y = StartCenter.y - NowCenter.y;
	delta.z = StartCenter.z - NowCenter.z;
	// --- 差分から角度を計算
	DirectX::XMFLOAT3 Theta;
	Theta.x = std::atan2(delta.z, delta.y);
	Theta.y = std::atan2(delta.x, delta.z);
	Theta.z = std::atan2(delta.y, delta.x);
	// --- 補正
	float fRot = 0.0f;

	fRot = Theta.z;

	if (delta.x < 0.0f) delta.x *= -1.0f;
	if (delta.z < 0.0f) delta.z *= -1.0f;
	if (delta.y < 0.0f) delta.y *= -1.0f;

	if (Theta.z > TORAD(20.0f) && Theta.z < TORAD(70.0f))
	{
		fRot = TORAD(45.0f);
		delta.x -= 5.0f;
	}

	if (Theta.z > TORAD(110.0f) && Theta.z < TORAD(160.0f))
	{
		fRot = TORAD(135.0f);
		delta.x -= 5.0f;
	}

	if (Theta.z < TORAD(-20.0f) && Theta.z > TORAD(-70.0f))
	{
		fRot = TORAD(-45.0f);
		delta.x -= 5.0f;
	}

	if (Theta.z < TORAD(-110.0f) && Theta.z > TORAD(-160.f))
	{
		fRot = TORAD(-135.0f);
		delta.x -= 5.0f;
	}

	Rot.y = fRot;

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		fRot,
		0.0f
	));

	float fsize = (delta.x + delta.y) * 1.1f;

	Scl.x = fsize;

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(fsize, 2.5f, delta.z);

	DirectX::XMMATRIX mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);

	DirectX::XMStoreFloat4x4(&world, mat);
}

void StarLine::DispLine()
{
	if (LINE_DEBUG)
	{
		m_pBox->SetView(m_pCamera->GetViewMatrix());
		m_pBox->SetProjection(m_pCamera->GetProjectionMatrix());
		m_pBox->SetWorld(world);
		m_pBox->DrawBox();
	}
	else
	{
		m_pModel->SetPostion(Pos.x, Pos.y, Pos.z);
		m_pModel->SetRotation(0.0f, Rot.y, 0.0f);
		m_pModel->SetScale(Scl.x, 0.0f, 0.0f);
		m_pModel->Draw();
	}
}

 void StarLine::SetCamera(Camera* InCamera)
{
	m_pCamera = InCamera;
}
