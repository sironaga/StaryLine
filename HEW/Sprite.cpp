#include "Sprite.h"
#include <cmath>

Sprite::Data Sprite::m_data;
std::shared_ptr<VertexShader> Sprite::m_defVS;
std::shared_ptr<PixelShader> Sprite::m_defPS;

void Sprite::Init()
{
	const char* VS = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
cbuffer Param : register(b1) {
	float2 offset;
	float2 size;
	float2 uvPos;
	float2 uvScale;
	float4 color;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos.xy *= size;
	vout.pos.xy += offset;
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	vout.uv *= uvScale;
	vout.uv += uvPos;
	vout.color = color;
	return vout;
})EOT";
	const char* PS = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET {
	return tex.Sample(samp, pin.uv) * pin.color;
})EOT";

	Vertex vtx[] =
	 {
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},
		{{-0.5f,-0.5f, 0.0f}, {0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.0f}, {1.0f, 1.0f}},
	};

	// メッシュ
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxSize = sizeof(Vertex);
	desc.vtxCount = _countof(vtx);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_data.mesh = std::make_shared<MeshBuffer>();
	m_data.mesh->Create(desc);

	// パラメーター
	m_data.param[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_data.param[1] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_data.param[2] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&m_data.matrix[0], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_data.matrix[1], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_data.matrix[2], DirectX::XMMatrixIdentity());

	// シェーダー
	m_defVS = std::make_shared<VertexShader>();
	m_defVS->Compile(VS);
	m_data.vs = m_defVS.get();
	m_defPS = std::make_shared <PixelShader>();
	m_defPS->Compile(PS);
	m_data.ps = m_defPS.get();
}
void Sprite::Uninit()
{
}
void Sprite::Draw()
{
	m_data.vs->WriteBuffer(0, m_data.matrix);
	m_data.vs->WriteBuffer(1, m_data.param);
	m_data.vs->Bind();
	m_data.ps->SetTexture(0, m_data.texture);
	m_data.ps->Bind();
	m_data.mesh->Draw();
}

void Sprite::SetOffset(DirectX::XMFLOAT2 offset)
{
	m_data.param[0].x = offset.x;
	m_data.param[0].y = offset.y;
}
void Sprite::SetSize(DirectX::XMFLOAT2 size)
{
	m_data.param[0].z = size.x;
	m_data.param[0].w = size.y;
}

void Sprite::SetUVPos(DirectX::XMFLOAT2 pos)
{
	m_data.param[1].x = pos.x;
	m_data.param[1].y = pos.y;
}
void Sprite::SetUVScale(DirectX::XMFLOAT2 scale)
{
	m_data.param[1].z = scale.x;
	m_data.param[1].w = scale.y;
}
void Sprite::SetColor(DirectX::XMFLOAT4 color)
{
	m_data.param[2] = color;
}
void Sprite::SetTexture(Texture* tex)
{
	m_data.texture = tex;
}
void Sprite::SetVertex(Vertex* InData, int nSize)
{
	// メッシュ
	MeshBuffer::Description desc = {};
	desc.pVtx = InData;
	desc.vtxSize = sizeof(Vertex);
	desc.vtxCount = nSize;
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_data.mesh = std::make_shared<MeshBuffer>();
	m_data.mesh->Create(desc);
}
void Sprite::SetWorld(DirectX::XMFLOAT4X4 world)
{
	m_data.matrix[0] = world;
}
void Sprite::SetView(DirectX::XMFLOAT4X4 view)
{
	m_data.matrix[1] = view;
}
void Sprite::SetProjection(DirectX::XMFLOAT4X4 proj)
{
	m_data.matrix[2] = proj;
}
void Sprite::SetVertexShader(Shader* vs)
{
	if (vs && typeid(VertexShader) == typeid(*vs))
		m_data.vs = vs;
	else
		m_data.vs = m_defVS.get();
}
void Sprite::SetPixelShader(Shader* ps)
{
	if (ps && typeid(PixelShader) == typeid(*ps))
		m_data.ps = ps;
	else
		m_data.ps = m_defPS.get();
}

void Sprite::SettingUVAnimation(Sprite* pSprite, int nSplitX, int nSplitY, int nAnimationSwap, bool IsLoop)
{
	DirectX::XMFLOAT2 tex = {};

	//int nSplit = nSplitX * nSplitY;

	if (IsLoop)
	{
		if ((nAnimeWidth >= nSplitX) && (nAnimeHeight >= nSplitY))
		{
			nAnimeWidth = 0;	// 最初のシーケンステクスチャに戻る
			nAnimeHeight = 0;
		}
	}
	else
	{
		if ((nAnimeWidth >= nSplitX) && (nAnimeHeight >= nSplitY))
		{
			nAnimeWidth = nSplitX - 1;	// 最後のシーケンステクスチャに戻る
			nAnimeHeight = nSplitY - 1;	// 最後のシーケンステクスチャに戻る
		}
	}

	// テクスチャアニメーション用
	if ((nSplitX * nSplitY) > 1)
	{
		if ((nAnimeWidth * nAnimeHeight) != ((nSplitX - 1) * (nSplitY - 1)))
		{
			if (nAnimeCount >= nAnimationSwap)
			{
				nAnimeWidth++;	// 次のシーケンステクスチャに移る
				if (nAnimeWidth == nSplitX)
				{
					if (nAnimeHeight < nSplitY)
					{
						nAnimeHeight++;
						nAnimeWidth = 0;
					}
					else
					{
						nAnimeWidth -= 1;
					}
				}
				nAnimeCount = 0;
			}
			else nAnimeCount++;
		}
	}

	tex.x = (float)nAnimeWidth / (float)nSplitX;	// 横のシーケンステクスチャの動き
	tex.y = (float)nAnimeHeight / (float)nSplitY;	// 縦のシーケンステクスチャの動き

	SetUVPos(tex);
	SetUVScale({ 1.0f / nSplitX,1.0f / nSplitY });
}

void Sprite::ReSetSprite()
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	DirectX::XMFLOAT4X4 wvp;
	DirectX::XMStoreFloat4x4(&wvp, DirectX::XMMatrixTranspose(mat));

	Sprite::SetWorld(wvp);
	Sprite::SetUVPos({ 0.0f, 0.0f });
	Sprite::SetUVScale({ 1.0f, 1.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

}

void Sprite::SetCenterPosAndRotation(DirectX::XMFLOAT3 StartPosLeft, DirectX::XMFLOAT3 StartPosRight, DirectX::XMFLOAT3 NowPosLeft, DirectX::XMFLOAT3 NowPosRight)
{
	DirectX::XMFLOAT3 Calculated;
	DirectX::XMFLOAT3 StartCenter =
	{	(StartPosLeft.x + StartPosRight.x) / 2.0f,
		(StartPosLeft.y + StartPosRight.y) / 2.0f,
		(StartPosLeft.z + StartPosRight.z) / 2.0f };
	DirectX::XMFLOAT3 NowCenter =
	{	( NowPosLeft.x + NowPosRight.x) / 2.0f,
		( NowPosLeft.y + NowPosRight.y) / 2.0f,
		( NowPosLeft.z + NowPosRight.z) / 2.0f };

	Calculated.x = ((StartCenter.x + NowCenter.x) / 2.0f);
	Calculated.y = ((StartCenter.y + NowCenter.y) / 2.0f);
	Calculated.z = 11.0f;

	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		Calculated.x,
		Calculated.y,
		Calculated.z,
		0.0f
	));

	DirectX::XMFLOAT3 delta;
	delta.x = StartCenter.x - NowCenter.x;
	delta.y = StartCenter.y - NowCenter.y;
	delta.z = StartCenter.z - NowCenter.z;

	DirectX::XMFLOAT3 Theta;
	Theta.x = std::atan2(delta.z, delta.y);
	Theta.y = std::atan2(delta.x, delta.z);
	Theta.z = std::atan2(delta.y, delta.x);

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		Theta.z,
		0.0f
	));

	if (delta.x < 0.0f) delta.x *= -1.0f;
	if (delta.z < 0.0f) delta.z *= -1.0f;
	if (delta.y < 0.0f)delta.y *= -1.0f;


	float size = delta.x + delta.y;




	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(size, 2.5f, delta.z);

	DirectX::XMMATRIX mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);

	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mat);

	m_data.matrix[0] = world;
}