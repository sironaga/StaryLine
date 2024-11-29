#include "Effect.h"

CEffect::CEffect(const char* EffectFile, float size)
	: effectResourceHandle(-1)
	, playingEffectHandle(-1)
	, grBackgroundHandle(-1)
{
	// エフェクトリソースを読み込む。
	// 読み込む時に大きさを指定する。
	effectResourceHandle = LoadEffekseerEffect(EffectFile, size);

	// エフェクトを描画するための背景
	grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));

	// Zバッファを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(TRUE);
}

CEffect::~CEffect()
{
	if (effectResourceHandle)
	{
		DeleteEffekseerEffect(effectResourceHandle);
	}
}

void CEffect::Update(bool is3D)
{
	if (is3D) UpdateEffekseer3D();
	else if (!is3D) UpdateEffekseer2D();
}

void CEffect::Draw(bool is3D)
{
	if (playingEffectHandle)
	{
		DrawGraph(0, 0, grBackgroundHandle, TRUE);
		if (is3D)
		{
			// DXライブラリのカメラを設定する。
			SetCameraPositionAndTarget_UpVecY(VGet(10, 10, -20), VGet(0, 0, 0));
			SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
			SetCameraNearFar(1.0f, 150.0f);

			// DXライブラリのカメラとEffekseerのカメラを同期する。
			Effekseer_Sync3DSetting();

			DrawEffekseer3D();
		}
		else if (!is3D) DrawEffekseer2D();
	}
}

void CEffect::SetEffectPos(DirectX::XMFLOAT3 pos, bool is3D)
{
	if (is3D) SetPosPlayingEffekseer3DEffect(effectResourceHandle, pos.x, pos.y, pos.z);
	else if (!is3D) SetPosPlayingEffekseer2DEffect(effectResourceHandle, pos.x, pos.y, pos.z);
}

void CEffect::SetEffectScale(DirectX::XMFLOAT3 size, bool is3D)
{
	if (is3D) SetScalePlayingEffekseer3DEffect(effectResourceHandle, size.x, size.y, size.z);
	else if (!is3D) SetScalePlayingEffekseer2DEffect(effectResourceHandle, size.x, size.y, size.z);
}

void CEffect::SetEffectRotate(DirectX::XMFLOAT3 rotate, bool is3D)
{
	if (is3D) SetRotationPlayingEffekseer3DEffect(effectResourceHandle, rotate.x, rotate.y, rotate.z);
	else if (!is3D) SetRotationPlayingEffekseer2DEffect(effectResourceHandle, rotate.x, rotate.y, rotate.z);
}

void CEffect::SetEffectColor(DirectX::XMFLOAT4 color, bool is3D)
{
	if (is3D) SetColorPlayingEffekseer3DEffect(effectResourceHandle, color.x, color.y, color.z, color.w);
	else if (!is3D) SetColorPlayingEffekseer2DEffect(effectResourceHandle, color.x, color.y, color.z, color.w);
}

void CEffect::SetEffectSpeed(float speed, bool is3D)
{
	if (is3D) SetSpeedPlayingEffekseer3DEffect(effectResourceHandle, speed);
	else if (!is3D) SetSpeedPlayingEffekseer2DEffect(effectResourceHandle, speed);
}

void CEffect::SetEffectState(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT4 color, float speed, bool is3D)
{
	if (is3D) SetPosPlayingEffekseer3DEffect(effectResourceHandle, pos.x, pos.y, pos.z);
	else if (!is3D) SetPosPlayingEffekseer2DEffect(effectResourceHandle, pos.x, pos.y, pos.z);

	if (is3D) SetScalePlayingEffekseer3DEffect(effectResourceHandle, size.x, size.y, size.z);
	else if (!is3D) SetScalePlayingEffekseer2DEffect(effectResourceHandle, size.x, size.y, size.z);

	if (is3D) SetRotationPlayingEffekseer3DEffect(effectResourceHandle, rotate.x, rotate.y, rotate.z);
	else if (!is3D) SetRotationPlayingEffekseer2DEffect(effectResourceHandle, rotate.x, rotate.y, rotate.z);

	if (is3D) SetColorPlayingEffekseer3DEffect(effectResourceHandle, color.x, color.y, color.z, color.w);
	else if (!is3D) SetColorPlayingEffekseer2DEffect(effectResourceHandle, color.x, color.y, color.z, color.w);

	if (is3D) SetSpeedPlayingEffekseer3DEffect(effectResourceHandle, speed);
	else if (!is3D) SetSpeedPlayingEffekseer2DEffect(effectResourceHandle, speed);
}

void CEffect::Play(bool is3D)
{
	if (is3D) playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
	else if (!is3D) playingEffectHandle = PlayEffekseer2DEffect(effectResourceHandle);
}
