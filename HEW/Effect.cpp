#include "Effect.h"
#include "CameraDebug.h"
#include "CameraUIControl.h"
#include "SpriteDrawer.h"

constexpr int SWAP_SPEED = 10;

CEffect::CEffect(const char* EffectFile, int nSplitX, int nSplitY)
	: m_tSplit{ 0.0f,0.0f }
	, m_tPos{ 0.0f,0.0f,0.0f }
	, m_tSize{ 1.0f,1.0f,1.0f }
	, m_tRotate{ 0.0f,0.0f,0.0f }
	, m_tColor{ 1.0f,1.0f,1.0f,1.0f }
	, m_fSpeed(1.0f)
	, m_nSplitX(nSplitX),m_nSplitY(nSplitY)
	, m_bPlay(false)
	, m_pEffect(nullptr), m_pTexture(nullptr)
{
	m_pEffect = new Sprite();
	m_pEffect->Init();
	m_pTexture = new Texture();
	m_pCamera = new CameraDebug();

	m_pTexture->Create(EffectFile);
	m_pEffect->SetTexture(m_pTexture);
}

CEffect::~CEffect()
{
	m_pEffect->Uninit();
	m_pTexture->GetResource()->Release();
}

void CEffect::Update()
{
	if (m_bPlay)
	{
		m_tSplit = UpdatePosTex(m_nSplitX, m_nSplitY, SWAP_SPEED);
		
		m_pEffect->SetUVPos(m_tSplit);
	}
}

void CEffect::Draw()
{
	if (m_bPlay)m_pEffect->Draw();
}

void CEffect::SetEffectPos(DirectX::XMFLOAT2 pos)
{

	DirectX::XMFLOAT2 pos2D = { pos.x,pos.y };
	m_pEffect->SetOffset(pos2D);
}

void CEffect::SetEffectScale(DirectX::XMFLOAT2 size)
{
	DirectX::XMFLOAT2 size2D = { size.x,size.y };
	m_pEffect->SetSize(size2D);

}

void CEffect::SetEffectRotate(DirectX::XMFLOAT3 rotate)
{
	/*回転2Dの処理*/
}

void CEffect::SetEffectColor(DirectX::XMFLOAT4 color)
{
	m_pEffect->SetColor(color);
}

void CEffect::SetEffectTexture()
{
	m_pEffect->SetTexture(m_pTexture);
}

void CEffect::SetEffect3D(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate)
{
	DirectX::XMMATRIX pos3D = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMMATRIX rotateX3D = DirectX::XMMatrixRotationX(rotate.x);
	DirectX::XMMATRIX rotateY3D = DirectX::XMMatrixRotationY(rotate.y);
	DirectX::XMMATRIX rotateZ3D = DirectX::XMMatrixRotationZ(rotate.z);
	DirectX::XMMATRIX size3D = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	DirectX::XMMATRIX mat = size3D * rotateX3D * rotateY3D * rotateZ3D * pos3D;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mat));
	m_pEffect->SetWorld(world);
	m_pEffect->SetView(m_pCamera->GetViewMatrix());
	m_pEffect->SetProjection(m_pCamera->GetProjectionMatrix());
}

void CEffect::SetEffectState(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT4 color,bool b3D)
{
	if (b3D)
	{
		DirectX::XMMATRIX pos3D = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		DirectX::XMMATRIX rotateX3D = DirectX::XMMatrixRotationX(rotate.x);
		DirectX::XMMATRIX rotateY3D = DirectX::XMMatrixRotationY(rotate.y);
		DirectX::XMMATRIX rotateZ3D = DirectX::XMMatrixRotationZ(rotate.z);
		DirectX::XMMATRIX size3D = DirectX::XMMatrixScaling(size.x, size.y, size.z);
		DirectX::XMMATRIX mat = size3D * rotateX3D * rotateY3D * rotateZ3D * pos3D;
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mat));
		m_pEffect->SetWorld(world);
	}
	else
	{
		DirectX::XMFLOAT2 pos2D = { pos.x,pos.y };
		m_pEffect->SetOffset(pos2D);

		DirectX::XMFLOAT2 size2D = { size.x,size.y };
		m_pEffect->SetOffset(size2D);

		/*回転2Dの処理*/
	}
	m_pEffect->SetColor(color);
}

void CEffect::Play()
{
	m_bPlay = true;
}

bool CEffect::IsPlay()
{
	return m_bPlay;
}

DirectX::XMFLOAT2 CEffect::UpdatePosTex(int nSplitX, int nSplitY, int nAnimationSwap)
{
	DirectX::XMFLOAT2 tex = {};
	static int nSplit = nSplitX * nSplitY;
	static int nAnimePage = 0;
	static int nAnimeCount = 0;

	// テクスチャアニメーション用
	if (nAnimeCount >= nAnimationSwap)
	{
		nAnimePage++;	// 次のシーケンステクスチャに移る
		nAnimeCount = 0;
	}
	else nAnimeCount++;

	if (nAnimePage >= nSplit)
	{
		nAnimePage = 0;		// エフェクトを描画し切ったら最初のシーケンステクスチャに戻る
		m_bPlay = false;	// プレイを止める
	}
	// 横のシーケンステクスチャの動き
	switch (nAnimePage % nSplitX)
	{
	default:break;
	case 0: tex.x = 0.0 / (float)nSplitX; break;
	case 1: tex.x = 1.0 / (float)nSplitX; break;
	case 2: tex.x = 2.0 / (float)nSplitX; break;
	case 3: tex.x = 3.0 / (float)nSplitX; break;
	}
	// 縦のシーケンステクスチャの動き
	switch (nAnimePage / nSplitY)
	{
	default:break;
	case 0: tex.y = 0.0 / (float)nSplitY; break;
	case 1: tex.y = 1.0 / (float)nSplitY; break;
	case 2: tex.y = 2.0 / (float)nSplitY; break;
	case 3: tex.y = 3.0 / (float)nSplitY; break;
	}

	return tex;
}