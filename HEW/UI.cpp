#include "UI.h"

CUI::CUI()
	:m_pTex()
	,m_rate(0.0f)
{
	m_pTex = new Texture();
	/*if (FAILED(m_pTex->Create("Asset/TitleBackGround/B_1.png")))
	{
		MessageBox(NULL, "Texture load failed.\nUI.cpp", "Error" , MB_OK);
	}*/
}

CUI::~CUI()
{
	
	if (m_pTex)
	{
		delete m_pTex;
		m_pTex = nullptr;
	}
	
}

void CUI::Update()
{
}

void CUI::Draw()
{
    DirectX::XMFLOAT4X4 world, view, proj;
    DirectX::XMMATRIX mView =
        DirectX::XMMatrixLookAtLH(
            DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
            DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        );
    DirectX::XMMATRIX mProj =
        DirectX::XMMatrixOrthographicOffCenterLH(
            0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f
        );

    mView = DirectX::XMMatrixTranspose(mView);
    mProj = DirectX::XMMatrixTranspose(mProj);
    DirectX::XMStoreFloat4x4(&view, mView);
    DirectX::XMStoreFloat4x4(&proj, mProj);
    Sprite::SetView(view);
    Sprite::SetProjection(proj);

    // フレームとゲージの描画 
    DirectX::XMFLOAT2 pos = { 640.0f,650.0f };
    DirectX::XMFLOAT2 size[] = {
     {210.0f, 40.0f}  // フレームを大きめに設定 
    };
    Texture* pTex[] = { m_pTex };
    for (int i = 0; i < 1; ++i)
    {
        DirectX::XMMATRIX T =
            DirectX::XMMatrixTranslation(pos.x - size[i].x * 0.5f, pos.y, 0.0f);
        DirectX::XMMATRIX S;
        if (i == 0) // フレームは倍率を変更せずに表示 
            S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);
        else   // ゲージは横方向をm_rateに応じて拡縮 
            S = DirectX::XMMatrixScaling(m_rate, -1.0f, 1.0f);
        DirectX::XMMATRIX mWorld = S * T;
        mWorld = DirectX::XMMatrixTranspose(mWorld);
        DirectX::XMStoreFloat4x4(&world, mWorld);


        Sprite::SetWorld(world);       // スプライトのワールド行列を設定 
        Sprite::SetSize(size[i]);      // スプライトのサイズを設定 
        Sprite::SetOffset({ size[i].x * 0.5f, 0.0f }); // スプライトの原点を変更 
        Sprite::SetTexture(pTex[i]);      // テクスチャを設定 
        Sprite::SetColor({ 1.0f,1.0f,1.0f,1.0f });
        Sprite::Draw();
    }

    Sprite::SetOffset({ 0.0f,0.0f });
}
