#pragma once

// includes
#include "DirectX.h"
#include "ModelEx.h"
#include "LibEffekseer.h"
#include "FieldVertex.h"
#include "EffectManager.h"

// defines
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
private:
	// プレイヤーの状態
	enum E_PLAYER_STATE
	{
		STOP = 0,	// 止まっている状態
		MOVE,		// 動いている情報
	}m_ePlayerState;
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

	void UpdateStop();						// 止まっている状態での更新処理
	void UpdateMove();						// 動いている状態での更新処理

	void DrawModel();						// モデルの描画処理

	void PlayerInput();						// コントローラー入力処理
private:
	CModelEx* m_pModel;						// プレイヤー(筆)のモデル
	DirectX::XMFLOAT3 m_tBrushPos;			// プレイヤー(筆)の座標
	DirectX::XMFLOAT3 m_tBrushRotate;		// プレイヤー(筆)の回転
	float m_fBrushSize;						// プレイヤー(筆)のサイズ
	float m_fBrushSpeed;					// プレイヤー(筆)の速度
	float m_fAddSpeed;						// 速度の加算

	int m_nNowVertex;						// 今の頂点
	int m_nDestination;						// 目的地の頂点
	bool m_bCanMoveCheck;					// 目的地へ行けるかどうか

	bool m_bDrawing;						// 作図中かどうか

public:
	/* Getter */
	// プレイヤーの座標取得
	const DirectX::XMFLOAT3 GetPlayerPos() { return m_tBrushPos; }
	// 目的地の取得
	const int GetPlayerDestination() { return m_eDestination; }
	// 描画中か取得
	const bool GetCanMove() { return m_bDrawing; }

	/* Setter */
	// 再作図開始のための設定
	void SetPlayerStop();	

	/*＝＝＝＝＝＝＝＝＝＝タイマー＝＝＝＝＝＝＝＝＝＝*/
private:
	void TimeProcess();						// タイマーの処理
private:
	ID3D11Buffer* m_pVtxTimer[3];				// タイマー描画用頂点情報
	ID3D11ShaderResourceView* m_pTexTimer[3];	// タイマー描画用テクスチャ
	Vertex vtxTimer[3][4];						// タイマーの各頂点
	float fTimerSize;						// 時間によって減らすタイマーのサイズ
	
	/*＝＝＝＝＝＝＝＝＝エフェクト＝＝＝＝＝＝＝＝＝*/
private:

	/*＝＝＝＝＝＝＝＝＝フィールド＝＝＝＝＝＝＝＝＝*/
public:
	/* Setter */
	// FieldVertexクラスアドレス引き渡し
	void SetFieldVertexAddress(CFieldVertex* InAddress);
private:
	CFieldVertex* m_pFieldVtx;				// FieldVertexクラスのアドレス
};