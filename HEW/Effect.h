// Effect.h
// 編集者 AT12A07_小川蓮 AT12A_43_吉田京志郎
// 
// エフェクトのライブラリをまとめたヘッダーです
// 使用する際は各クラス内にコンポジションとして実装してください

#pragma once

/* includes */
#include <DirectXMath.h>

class CEffect
{
public:
	// コンストラクタ
	// エフェクトのファイルをロードするためのコンストラクタです
	// 引数にファイルへのパスを設定してください
	CEffect(const char* EffectFile);
	// デストラクタ
	// 各終了処理にてdeleteして呼び出してくださ
	~CEffect();
private:
	DirectX::XMFLOAT2 
};

