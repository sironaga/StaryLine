#pragma once
// includes
#define _DIRECTX_
#include "DirectX.h"
#include "FieldVertex.h"
#include "Sprite.h"
#include "ModelEx.h"
#include "Texture.h"
#include "LibEffekseer.h"

// defines
#define MOVESPEED (0.5f)	// 移動速度
#define START_PLAYER (12)	//プレイヤーの開始位置

class CPlayer
{
	/*＝＝＝＝＝＝＝＝＝＝四大処理＝＝＝＝＝＝＝＝＝＝*/
public:
	CPlayer();		// コンストラクタ
	~CPlayer();		// デストラクタ
	void Update();	// 更新処理
	void Draw();	// 描画処理

	/*＝＝＝＝＝＝＝＝＝＝内部処理＝＝＝＝＝＝＝＝＝＝*/
	/*＝＝＝＝＝＝＝＝＝ プレイヤー ＝＝＝＝＝＝＝＝＝*/
public:
	// プレイヤーの状態
	enum E_PLAYER_STATE
	{
		STOP = 0,	// 止まっている状態
		MOVE,		// 動いている情報
	}m_ePlayerState;

	/* Getter */
	// プレイヤーの座標取得
	Sprite* m_pPlayer;

	const DirectX::XMFLOAT3 GetPlayerPos() { return m_tBrushPos; }
	// プレイヤーの状態取得
	const E_PLAYER_STATE GetPlayerState() { return m_ePlayerState; }
	// 目的地の取得
	const int GetPlayerDestination() { return m_eDestination; }

	const bool GetPlayerPhase() { return m_bDrawing; }

private:
	// 移動方向用列挙型
	enum E_DESTINATION
	{
		UP = 0,		// 上
		UPRIGHT,	// 右上
		RIGHT,		// 右
		DOWNRIGHT,	// 右下
		DOWN,		// 下
		DOWNLEFT,	// 左下
		LEFT,		// 左
		UPLEFT,		// 左上

		DEFAULT,	// 真ん中(基本状態)
	}m_eDestination;

	enum E_SPRITE
	{
		PLAYER = 0,
		TIMER,

		MAX_SPRITE
	};

	void UpdateStop();							// 止まっている状態での更新処理
	void UpdateMove();							// 動いている状態での更新処理

	void DrawModel(E_SPRITE type);

	void PlayerInput();							// コントローラー入力
private:
	DirectX::XMFLOAT3 m_tBrushPos;						// プレイヤーの座標
	DirectX::XMFLOAT3 m_tBrushRotate;
	float m_fBrushSize;
	Texture* m_pPlayerTex;

	int m_nNowVertex;							// 今の頂点
	int m_nDestination;							// 目的地の頂点
	bool bCanMoveCheck;							// 目的地へ行けるかどうか

	bool m_bDrawing;                      //描画タイム中かどうか

public:
	void SetPlayerStop();

	/*＝＝＝＝＝＝＝＝＝＝タイマー＝＝＝＝＝＝＝＝＝＝*/
private:
	void TimeProcess();							// タイマー処理用関数
private:
	ID3D11Buffer* m_pVtxTimer;					// タイマー描画用頂点情報
	ID3D11ShaderResourceView* m_pTexTimer;		// タイマー描画用テクスチャ
	Vertex vtxTimer[4];						// タイマー用頂点情報
	FPSTIMER tPlayerTimer;						// タイマー
	float fTimerSize;							// 時間によって減らすタイマーのサイズ
	float fDrawTime;							// 描画制限時間
	float fBonusTime;							// 描画時間のボーナス
	bool bTimerStart;							// タイマーが始まるかどうか

	/*＝＝＝＝＝＝＝＝＝フィールド＝＝＝＝＝＝＝＝＝*/
public:
	/* Setter */
	// Playerクラスアドレス引き渡し
	void SetFieldVertexAddress(CFieldVertex*);
	bool GetCanMove();
private:
	CFieldVertex* m_pFieldVtx;	// FieldVertexクラスのアドレス
	Sprite* m_pSprite;
	CModelEx* m_pModel;
	/*＝＝＝＝＝＝＝＝＝スプライト＝＝＝＝＝＝＝＝＝*/
	DirectX::XMMATRIX pos;
	DirectX::XMMATRIX size;
	DirectX::XMMATRIX rotate;
	DirectX::XMMATRIX mat;
	/*＝＝＝＝＝＝＝＝＝エフェクト＝＝＝＝＝＝＝＝＝*/
	Effekseer::EffectRef m_Effect;
};