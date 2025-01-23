#pragma once

#include "Texture.h"
#include "_StructList.h"

class CEffectManager_sp
{
public:
	/// @brief
	/// エフェクトのスプライトシートを読み込むコンストラクタ
	/// @param
	/// pass	画像のパス
	/// @param
	/// SplitX	スプライトシートの横分割数
	/// @param
	/// SplitY	スプライトシートの縦分割数
	/// @param
	/// Speed	エフェクトを何秒で完走するか
	CEffectManager_sp(const char* pass, int SplitX, int SplitY, float Speed = 1.0f,DirectX::XMFLOAT3 defSize = { 10.0f,10.0f,0.0f });

	/// @brief
	/// 既に読み込み済みのエフェクトを流用するコンストラクタ
	/// @param
	/// InData	既にnewしてあるCEffectManager_sp型のポインタ
	CEffectManager_sp(CEffectManager_sp* InData);

	/// @brief
	/// デストラクタ
	~CEffectManager_sp();

	/// @brief
	/// 更新処理
	/// @brief
	/// 常に呼び出す
	void Update();

	/// @brief
	/// 描画処理
	/// @brief
	/// 常に呼び出す
	///	@param
	/// is3D	3D上に描画するかどうか(true:3D,false:2D)
	void Draw(bool is3D = true);

	/// @brief
	/// 再生処理
	/// @brief
	/// この関数は常に呼び出すのではなく、再生したい箇所で一度だけ呼び出す
	///	@param
	/// isLoop	ループ再生するかどうか(true:ループする,false:ループしない)
	void Play(bool isLoop = false);

	/// @brief
	/// 座標の設定
	///	@param
	/// pos	座標(DirectX::XMFLOAT3)
	void SetPos(DirectX::XMFLOAT3 pos);

	/// @brief
	/// サイズの設定
	///	@param
	/// size	サイズ(DirectX::XMFLOAT3)
	void SetSize(DirectX::XMFLOAT3 size);

	/// @brief
	/// 回転の設定
	///	@param
	/// rotate	回転のラジアン角(DirectX::XMFLOAT3)
	void SetRotate(DirectX::XMFLOAT3 rotate);

	/// @brief
	/// 再生されているか取得
	///	@brief
	/// Play関数と同時に使う(再生されてない時に再生処理を呼び出す)
	/// @return
	/// 再生されているかどうか(true:再生中,false:再生されていない)
	bool IsPlay();
private:
	Texture* m_pTexture;
	struct Split
	{
		int x;
		int y;
	}m_tSplit;
	int m_nCount;
	int m_nPage;
	int m_nSplitNum;
	float m_fSpeed;
	bool m_bPlay;
	bool m_bLoop;
	ObjectParam m_tParam;
};