#include "LibEffekseer.h"
#include <uchar.h>

Effekseer::ManagerRef LibEffekseer::m_efkManager = nullptr;
EffekseerRendererDX11::RendererRef LibEffekseer::m_efkRenderer = nullptr;
EffekseerSound::SoundRef LibEffekseer::m_efkSound = nullptr;
Effekseer::Vector3D LibEffekseer::m_viewPosition;
Effekseer::Matrix44 LibEffekseer::m_viewMatrix;
Effekseer::Matrix44 LibEffekseer::m_projMatrix;

/*
* @brief 初期化処理
* @parma[in] pDevice リソースデバイス
* @param[in] pContext 描画デバイス
* @param[in] pAudio サウンドデバイス
*/
void LibEffekseer::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IXAudio2* pAudio)
{
	// エフェクトマネージャー作成
	m_efkManager = Effekseer::Manager::Create(8000);

	// 描画デバイス作成
	auto graphicsDevice = EffekseerRendererDX11::CreateGraphicsDevice(pDevice, pContext);
	m_efkRenderer = EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// 描画モジュールの設定
	m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());
	m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	m_efkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	m_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// サウンドモジュール設定
	if (pAudio)
	{
		m_efkSound = EffekseerSound::Sound::Create(pAudio, 16, 16);
		m_efkManager->SetSoundPlayer(m_efkSound->CreateSoundPlayer());
		m_efkManager->SetSoundLoader(m_efkSound->CreateSoundLoader());
	}
}

/*
* @brief 終了処理
*/
void LibEffekseer::Uninit()
{
	m_efkSound.Reset();
	m_efkRenderer.Reset();
	m_efkManager.Reset();
}

/*
* @brief カメラ位置の設定
* @param[in] pos カメラ位置
*/
void LibEffekseer::SetViewPosition(DirectX::XMFLOAT3 pos)
{
	m_viewPosition.X = pos.x;
	m_viewPosition.Y = pos.y;
	m_viewPosition.Z = pos.z;
}

/*
* @brief カメラ行列設定
* @param[in] view 転置なしビュー行列
* @param[in] proj 転置なしプロジェクション行列
*/
void LibEffekseer::SetCameraMatrix(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	memcpy_s(m_viewMatrix.Values, sizeof(m_viewMatrix.Values), view.m, sizeof(view.m));
	memcpy_s(m_projMatrix.Values, sizeof(m_projMatrix.Values), proj.m, sizeof(proj.m));
}

/*
* @brief すべてのエフェクトの描画
*/
void LibEffekseer::Draw()
{
	// レイヤーパラメータの設定
	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = m_viewPosition;
	m_efkManager->SetLayerParameter(0, layerParameter);

	// マネージャーの更新
	Effekseer::Manager::UpdateParameter updateParameter;
	m_efkManager->Update(updateParameter);

	// レンダラーの更新
	m_efkRenderer->SetTime(1.0f / 60.0f);
	m_efkRenderer->SetProjectionMatrix(m_projMatrix);
	m_efkRenderer->SetCameraMatrix(m_viewMatrix);

	// 描画処理
	m_efkRenderer->BeginRendering();
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = m_efkRenderer->GetCameraProjectionMatrix();
	m_efkManager->Draw(drawParameter);
	m_efkRenderer->EndRendering();
}

/*
* @brief Effekseerマネージャーの取得
* @return マネージャーへの参照
*/
Effekseer::ManagerRef LibEffekseer::GetManager()
{
	return m_efkManager;
}

/*
* @brief エフェクトファイルの読み込み
* @param[in] fileName 読み込むファイル名
* @return エフェクトデータ
*/
Effekseer::EffectRef LibEffekseer::Create(const char* fileName)
{
	static char16_t u16Path[MAX_PATH] = u"";
	char16_t* ptr = u16Path;
	while(*fileName != '\0')
	{
		mbrtoc16(ptr, fileName, 1, nullptr);
		++ptr;
		++fileName;
	}
	*ptr = '\0';
	return Effekseer::Effect::Create(m_efkManager, u16Path);
}