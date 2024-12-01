// Effect.h
// 編集者 AT12A07_小川蓮 AT12A_43_吉田京志郎
// 
// エフェクトのライブラリをまとめたヘッダーです
// 使用する際は各クラス内にコンポジションとして実装してください

#pragma once

/* includes */
#include <DirectXMath.h>
#include "Sprite.h"
#include "Texture.h"
#include "Camera.h"

class CEffect
{
public:
	// コンストラクタ
	// エフェクトのファイルをロードするためのコンストラクタです
	// 第一引数にファイルへのパス、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	CEffect(const char* EffectFile,int nSplitX, int nSplitY);
	// デストラクタ
	// 各終了処理にてdeleteして呼び出してくださ
	~CEffect();

	// Update
	// エフェクトの更新処理です
	// 各更新処理にて常に呼び出してください
	// 引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void Update();

	// Draw
	// エフェクトの描画処理です
	// 各描画処理にて常に呼び出してください
	// 引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void Draw();

	// SetEffectPos
	// エフェクトを出す場所を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に座標、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectPos(DirectX::XMFLOAT2 pos);

	// SetEffectScale
	// エフェクトの大きさの倍率を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数にサイズ(倍率)、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectScale(DirectX::XMFLOAT2 scale);

	// SetEffectRotate
	// エフェクトの回転を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に回転(ラジアン角)、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectRotate(DirectX::XMFLOAT3 rotate);

	// SetEffectColor
	// エフェクトの色を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に色(RGBa)、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectColor(DirectX::XMFLOAT4 color);

	// SetEffectColor
	// エフェクトの再生速度を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に速度、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	// void SetEffectSpeed(float speed);

	void SetEffectTexture();

	// SetEffectSprit
	// 3Dの変換行列を設定するSetterです
	// 描画の前に呼び出してください
	void SetEffect3D(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size = { 1.0f,1.0f,1.0f }, DirectX::XMFLOAT3 rotate = { 0.0f,0.0f,0.0f });

	// SetEffectState
	// エフェクトのSetterをまとめたSetterです
	// Update関数の前に呼び出してください
	// デフォルト引数で座標の中央、1.0fのサイズ、無回転、全色、1.0fのスピード、2D描画になっています
	void SetEffectState(
		DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT3 size = { 1.0f,1.0f,1.0f },
		DirectX::XMFLOAT3 rotate = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }, 
		bool b3D = false
	);

	// Play
	// エフェクトを再生する関数です
	// エフェクトを出したいタイミングでこの関数を呼び出してください
	void Play();

	// Play
	// エフェクトが再生されているかを取得する関数です
	bool IsPlay();
private:
	DirectX::XMFLOAT2 m_tSplit;
	DirectX::XMFLOAT3 m_tPos;
	DirectX::XMFLOAT3 m_tSize;
	DirectX::XMFLOAT3 m_tRotate;
	DirectX::XMFLOAT4 m_tColor;
	int m_nSplitX;
	int m_nSplitY;
	float m_fSpeed;
	bool m_bPlay;
	Sprite* m_pEffect;
	Texture* m_pTexture;
	Camera* m_pCamera;

	DirectX::XMFLOAT2 UpdatePosTex(int nSplitX, int nSplitY, int nAnimationSwap);
};

