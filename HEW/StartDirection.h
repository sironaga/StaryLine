#pragma once
#include "SpriteEx.h"
#include "Main.h"
#include "NumberUI.h"


class CStartDirection
{
public:
	//コンストラクタ
	CStartDirection();
	//デストラクタ
	~CStartDirection();
	//更新処理
	void Update();
	//描画処理
	void Draw();

	//演出の出現時間
	void SetTimer(float InTime);
	//現在のDraw状態
	bool GetDraw();
private:
	FLOAT3 m_tPos[7];//各文字のポジション
	FLOAT3 m_tSize[7];//各文字のサイズ
	float m_timer;//全体でかかる時間
	bool m_NowDraw;//描画されている方
	bool m_startDraw;//startの描画判定
	SpriteEx* m_StratSprite[7];//テクスチャーの格納用
	float  m_CountTimer[7];//各文字の描画時間
	float  m_OldCountTimer[2];////各文字の描画時間
};

