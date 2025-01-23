#include "EffectManager_sp.h"
#include "Sprite.h"
#include "Main.h"

CEffectManager_sp::CEffectManager_sp(const char* pass, int SplitX, int SplitY, float Speed, DirectX::XMFLOAT3 defSize)
	: m_pTexture(nullptr)
	, m_tSplit{SplitX,SplitY}, m_tParam{}
	, m_nCount(0), m_nPage(0)
	, m_nSplitNum(SplitX * SplitY),m_fSpeed(Speed)
	, m_bPlay(false), m_bLoop(false)
{
	m_tParam.pos = { 0.0f,0.0f,0.0f };
	m_tParam.size = defSize;
	m_tParam.rotate = { 0.0f,0.0f,0.0f };
	m_tParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tParam.uvPos =  { 0.0f / (float)SplitX, 0.0f / (float)SplitY };
	m_tParam.uvSize = { 1.0f / (float)SplitX, 1.0f / (float)SplitY };

	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create(pass))) MessageBox(NULL, pass, "Error:EffectManager_sp.cpp", MB_OK);
}

CEffectManager_sp::CEffectManager_sp(CEffectManager_sp* InData)
	: m_pTexture(InData->m_pTexture)
	, m_tSplit(InData->m_tSplit), m_tParam(InData->m_tParam)
	, m_nCount(0), m_nPage(0)
	, m_nSplitNum(InData->m_nSplitNum),m_fSpeed(InData->m_fSpeed)
	, m_bPlay(false),m_bLoop(false)
{

}

CEffectManager_sp::~CEffectManager_sp()
{
	SAFE_DELETE(m_pTexture);
}

void CEffectManager_sp::Update()
{
	if (m_bPlay)
	{
		m_nCount++;
		m_tParam.uvPos.x = (float)(m_nPage % m_tSplit.x) / (float)m_tSplit.x;
		m_tParam.uvPos.y = (float)(m_nPage / m_tSplit.x) / (float)m_tSplit.y;
		if (m_nCount >= m_fSpeed * 60.0f / (float)m_nSplitNum)
		{
			m_nPage++;
			m_nCount = 0;
			if (m_nPage >= m_nSplitNum)
			{
				if (m_bLoop) m_nPage = 0;
				else
				{
					m_bPlay = false;
					m_nPage = 0;
				}
			}
		}
	}
}

void CEffectManager_sp::Draw(bool is3D)
{
	if (m_bPlay)
	{
		if (is3D)
		{
			SetRender3D();
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_tParam.pos.x, m_tParam.pos.y, m_tParam.pos.z);
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tParam.size.x, m_tParam.size.y, m_tParam.size.z);
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(m_tParam.rotate.x, m_tParam.rotate.y, m_tParam.rotate.z, 0.0f));
			DirectX::XMMATRIX mWorld = S * R * T;
			mWorld = DirectX::XMMatrixTranspose(mWorld);
			DirectX::XMFLOAT4X4 world;
			DirectX::XMStoreFloat4x4(&world, mWorld);

			Sprite::SetOffset({ 0.0f,0.0f });
			Sprite::SetSize({ 1.0f,1.0f });
			Sprite::SetColor(m_tParam.color);
			Sprite::SetUVPos(m_tParam.uvPos);
			Sprite::SetUVScale(m_tParam.uvSize);
			Sprite::SetWorld(world);
			Sprite::SetView(GetView());
			Sprite::SetProjection(GetProj());
			Sprite::SetTexture(m_pTexture);
			Sprite::Draw();
		}
		else
		{
			SetRender2D();
			Sprite::SetOffset({ m_tParam.pos.x,m_tParam.pos.y });
			Sprite::SetSize({ m_tParam.size.x,m_tParam.size.y });
			Sprite::SetColor(m_tParam.color);
			Sprite::SetUVPos(m_tParam.uvPos);
			Sprite::SetUVScale(m_tParam.uvSize);
			Sprite::SetWorld(Get2DWorld(true, m_tParam.rotate));
			Sprite::SetView(Get2DView());
			Sprite::SetProjection(Get2DProj());
			Sprite::SetTexture(m_pTexture);
			Sprite::Draw();
		}
	}
}

void CEffectManager_sp::Play(bool isLoop)
{
	m_bPlay = true;
	m_bLoop = isLoop;
}

void CEffectManager_sp::SetPos(DirectX::XMFLOAT3 pos)
{
	m_tParam.pos = pos;
}

void CEffectManager_sp::SetSize(DirectX::XMFLOAT3 size)
{
	m_tParam.size = size;
}

void CEffectManager_sp::SetRotate(DirectX::XMFLOAT3 rotate)
{
	m_tParam.rotate = rotate;
}

bool CEffectManager_sp::IsPlay()
{
	return m_bPlay;
}
