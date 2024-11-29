// Effect.h
// 編集者 AT12A07_小川蓮 AT12A_43_吉田京志郎
// 
// エフェクトのライブラリをまとめたヘッダーです
// 使用する際は各クラス内にコンポジションとして実装してください

#pragma once

/* includes */
#include "Effekseer/EffekseerForDXLib.h"
#include <DirectXMath.h>

class CEffect
{
public:
	// コンストラクタ
	// エフェクトのファイルをロードするためのコンストラクタです
	// 第一引数にファイルへのパス、第二引数に初期のサイズを設定してください
	// 第二引数はデフォルトで1.0fが入っています
	CEffect(const char* EffectFile, float size = 1.0f);
	// デストラクタ
	// 各終了処理にてdeleteして呼び出してくださ
	~CEffect();

	// Update
	// エフェクトの更新処理です
	// 各更新処理にて常に呼び出してください
	// 引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void Update(bool is3D);

	// Draw
	// エフェクトの描画処理です
	// 各描画処理にて常に呼び出してください
	// 引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void Draw(bool is3D);

	// SetEffectPos
	// エフェクトを出す場所を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に座標、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectPos(DirectX::XMFLOAT3 pos, bool is3D);

	// SetEffectScale
	// エフェクトの大きさの倍率を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数にサイズ(倍率)、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectScale(DirectX::XMFLOAT3 size, bool is3D);

	// SetEffectRotate
	// エフェクトの回転を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に回転(ラジアン角)、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectRotate(DirectX::XMFLOAT3 rotate, bool is3D);

	// SetEffectColor
	// エフェクトの色を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に色(RGBa)、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectColor(DirectX::XMFLOAT4 color, bool is3D);

	// SetEffectColor
	// エフェクトの再生速度を決めるSetterです
	// Update関数の前に呼び出してください
	// 第一引数に速度、第二引数に3Dかどうか(true = 3D,false = 2D)を設定してください
	void SetEffectSpeed(float speed, bool is3D);

	// SetEffectState
	// エフェクトのSetterをまとめたSetterです
	// Update関数の前に呼び出してください
	// デフォルト引数で座標の中央、1.0fのサイズ、無回転、全色、1.0fのスピード、2D描画になっています
	void SetEffectState(
		DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT3 size = { 1.0f,1.0f,1.0f },
		DirectX::XMFLOAT3 rotate = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f },
		float speed = 1.0f,
		bool is3D = false);

	// Play
	// エフェクトを再生する関数です
	// エフェクトを出したいタイミングでこの関数を呼び出してください
	void Play(bool is3D);

private:
	int effectResourceHandle;	// エフェクトリソースの読み込み用
	int playingEffectHandle;	// エフェクトプレイヤー用
	int grBackgroundHandle;		// エフェクトの描画用
};

