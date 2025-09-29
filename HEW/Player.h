#pragma once

// includes
#include "DirectX.h"
#include "ModelEx.h"
#include "LibEffekseer.h"
#include "FieldVertex.h"
#include "EffectManager_sp.h"
#include "Texture.h"
#include "_StructList.h"
#include "SpriteEx.h"

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
	void Reset();

	void Reload();
	void TimerReCharge();
	void TimerSetMax();
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

public:
	void UpdateStop();	// 止まっている状態での更新処理
private:
	void UpdateMove();						// 動いている状態での更新処理

	void DrawModel();						// モデルの描画処理

	void PlayerInput();						// コントローラー入力処理
	void ArrowProcess();
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
	bool m_bReCharge;
	bool m_bSwap;

	bool m_bDrawing;						// 作図中かどうか
	bool m_bStop;

	
	enum E_ARROW_STATE
	{
		NONE_SELECT,
		SELECTED,
		CANNOT_SELECT
	};
	E_ARROW_STATE m_eArrowState;
	CModelEx* m_pArrowModel;
	ObjectParam m_pArrowParam;
	DirectX::XMFLOAT2  m_tArrowCenterPos;
	DirectX::XMFLOAT2 m_tAjustPos;

	CEffectManager_sp* m_pEffect;
	ObjectParam m_tEffectParam;

	enum Timer
	{
		Timer_Under = 0,
		Timer_Gauge,
		Timer_Top,

		Timer_Max
	};
	SpriteParam m_tTimerParam[Timer_Max];
	Texture* m_pTimerTex[Timer_Max];

public:
	/* Getter */
	// プレイヤーの座標取得
	const DirectX::XMFLOAT3 GetPlayerPos() { return m_tBrushPos; }
	// 目的地の取得
	const int GetPlayerDestination() { return m_eDestination; }
	// 現在の頂点番号の取得
	const int GetNowVertex() { return m_nNowVertex; }
	// 描画中か取得
	const bool GetCanMove() { return m_bDrawing; }

	bool GetMoveStop() { return m_bStop; }

	/* Setter */
	// 再作図開始のための設定
	void SetPlayerStop();
	void SetMoveStop();
	void SetNowVertex(int inVertex) { m_nNowVertex = inVertex; }
	void SetPos(DirectX::XMFLOAT3 inPos) { m_tBrushPos = inPos; }

	/*＝＝＝＝＝＝＝＝＝＝タイマー＝＝＝＝＝＝＝＝＝＝*/
private:
	void TimeProcess();						// タイマーの処理
private:
	
	/*＝＝＝＝＝＝＝＝＝エフェクト＝＝＝＝＝＝＝＝＝*/
private:

	/*＝＝＝＝＝＝＝＝＝フィールド＝＝＝＝＝＝＝＝＝*/
public:
	/* Setter */
	// FieldVertexクラスアドレス引き渡し
	void SetFieldVertexAddress(CFieldVertex* InAddress);
private:
	CFieldVertex* m_pFieldVtx;				// FieldVertexクラスのアドレス
public:
	/*＝＝＝＝＝＝＝＝＝音＝＝＝＝＝＝＝＝＝*/
	void InitSound();
};