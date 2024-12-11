#ifndef __LIB_EFFEKSEER_H__
#define __LIB_EFFEKSEER_H__

#include <Effekseer/Effekseer.h>
#include <Effekseer/EffekseerRendererDX11.h>
#include <Effekseer/EffekseerSoundXAudio2.h>
#include <DirectXMath.h>

#pragma comment(lib, "xaudio2.lib")

#if _MSC_VER >= 1930
#ifdef _DEBUG
#pragma comment(lib, "Effekseer/Effekseer_vs2022D.lib")
#pragma comment(lib, "Effekseer/EffekseerRendererDX11_vs2022D.lib")
#pragma comment(lib, "Effekseer/EffekseerSoundXAudio2_vs2022D.lib")
#else
#pragma comment(lib, "Effekseer/Effekseer_vs2022.lib")
#pragma comment(lib, "Effekseer/EffekseerRendererDX11_vs2022.lib")
#pragma comment(lib, "Effekseer/EffekseerSoundXAudio2_vs2022.lib")
#endif
#elif _MSC_VER >= 1920
#ifdef _DEBUG
#pragma comment(lib, "Effekseer/Effekseer_vs2019D.lib")
#pragma comment(lib, "Effekseer/EffekseerRendererDX11_vs2019D.lib")
#pragma comment(lib, "Effekseer/EffekseerSoundXAudio2_vs2019D.lib")
#else
#pragma comment(lib, "Effekseer/Effekseer_vs2019.lib")
#pragma comment(lib, "Effekseer/EffekseerRendererDX11_vs2019.lib")
#pragma comment(lib, "Effekseer/EffekseerSoundXAudio2_vs2019.lib")
#endif
#elif _MSC_VER >= 1910
#ifdef _DEBUG
#pragma comment(lib, "Effekseer/Effekseer_vs2017D.lib")
#pragma comment(lib, "Effekseer/EffekseerRendererDX11_vs2017D.lib")
#pragma comment(lib, "Effekseer/EffekseerSoundXAudio2_vs2017D.lib")
#else
#pragma comment(lib, "Effekseer/Effekseer_vs2017.lib")
#pragma comment(lib, "Effekseer/EffekseerRendererDX11_vs2017.lib")
#pragma comment(lib, "Effekseer/EffekseerSoundXAudio2_vs2017.lib")
#endif
#endif


class LibEffekseer
{
public:
	// 基本処理
	LibEffekseer() {}
	~LibEffekseer() {}
	static void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IXAudio2* pAudio = nullptr);
	static void Uninit();

	// 描画関連処理
	static void SetViewPosition(DirectX::XMFLOAT3 pos);
	static void SetCameraMatrix(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	static void Draw();

	// エフェクト機能
	static Effekseer::ManagerRef GetManager();
	static Effekseer::EffectRef Create(const char* fileName);

private:
	static Effekseer::ManagerRef m_efkManager;
	static EffekseerRendererDX11::RendererRef m_efkRenderer;
	static EffekseerSound::SoundRef m_efkSound;
	static Effekseer::Vector3D m_viewPosition;
	static Effekseer::Matrix44 m_viewMatrix;
	static Effekseer::Matrix44 m_projMatrix;
};


#endif // __LIB_EFFEKSEER_H__