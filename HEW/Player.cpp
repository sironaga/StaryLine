// includes
#include "Player.h"
#include "Main.h"
#include "SceneGame.h"
#include "Defines.h"
#include "InputEx.h"
#include "SpriteDrawer.h"
#include "DirectXTex/TextureLoad.h"
#include "SoundList.h"
#include "Sprite.h"

// defines
#define BRUSH_SPEED (0.5f)	// 移動速度
#define BRUSH_SPEED_FEVER (0.5f)	// フィーバータイム中の移動速度

// constans
constexpr float BRUSH_AJUSTPOS_X = 3.0f;		// プレイヤー(筆)の横座標補正値
constexpr float BRUSH_AJUSTPOS_Y = 8.0f;		// プレイヤー(筆)の縦座標補正値
constexpr float BRUSH_SIZE = 10.0f;				// プレイヤー(筆)のサイズ
constexpr float BRUSH_ROTATE_X = 140.0f;		// プレイヤー(筆)のX軸回転
constexpr float BRUSH_ROTATE_Y = -20.0f;		// プレイヤー(筆)のY軸回転

constexpr float TIMER_BAR_HARFSIZE_X = 50.0f;		// タイマーゲージの横ハーフサイズ
constexpr float TIMER_BAR_HARFSIZE_Y = 400.0f;		// タイマーゲージの縦ハーフサイズ
constexpr float TIMER_OUT_HARFSIZE_X = 130.0f;		// タイマー入れ物の横ハーフサイズ
constexpr float TIMER_OUT_HARFSIZE_Y = 780.0f;		// タイマー入れ物の縦ハーフサイズ

constexpr float TIMER_BAR_OFFSET_X = -700.0;		// タイマーゲージの横オフセット
constexpr float TIMER_BAR_OFFSET_Y = -340.0f;		// タイマーゲージの縦オフセット
constexpr float TIMER_OUT_OFFSET_X = -700.0;		// タイマー入れ物の横オフセット
constexpr float TIMER_OUT_OFFSET_Y = -510.0f;		// タイマー入れ物の縦オフセット

constexpr float ARROW_AJUST_POS =  100.0f;		// 矢印の座標X,Yの補正値
constexpr float ARROW_SIZE =  100.0f;			// 矢印の座標X,Yの補正値

IXAudio2SourceVoice* g_WalkSe;
CSoundList* g_PlayerSound;

CPlayer::CPlayer()
	// プレイヤー(筆)の初期化処理
	: m_pModel(nullptr)
	, m_tBrushPos{}, m_fBrushSize(BRUSH_SIZE), m_tBrushRotate{ DirectX::XMConvertToRadians(BRUSH_ROTATE_X), DirectX::XMConvertToRadians(BRUSH_ROTATE_Y), 0.0f }
	, m_fBrushSpeed(BRUSH_SPEED), m_fAddSpeed(BRUSH_SPEED)
	, m_nNowVertex(START_PLAYER), m_nDestination(START_PLAYER)
	, m_ePlayerState(STOP), m_eDestination(DEFAULT)
	, m_bCanMoveCheck(false), m_bDrawing(true)
	, m_tArrowInfo{}, m_pTexture{}

	// タイマーの初期化処理
	, m_pVtxTimer{}, m_pTexTimer{}
	, vtxTimer{},fTimerSize(-TIMER_BAR_HARFSIZE_Y)

	// FieldVertexアドレスの初期化処理
	, m_pFieldVtx(nullptr)
{
	// プレイヤー(筆)モデルの読み込み
	m_pModel = new CModelEx(MODEL_PASS("Player/Lini_FountainPen.fbx"));

	vtxTimer[0][0] = { { -WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide()) , HEIGHT_RATIO(fTimerSize,		   GetNowHeight()),  0.0f },	{ 0.0f, 0.0f } };
	vtxTimer[0][1] = { { -WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide()) , HEIGHT_RATIO(TIMER_BAR_HARFSIZE_Y,GetNowHeight()), 0.0f },	{ 0.0f, 1.0f } };
	vtxTimer[0][2] = { {  WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide())	, HEIGHT_RATIO(fTimerSize,		   GetNowHeight()),  0.0f },	{ 1.0f, 0.0f } };
	vtxTimer[0][3] = { {  WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide())	, HEIGHT_RATIO(TIMER_BAR_HARFSIZE_Y,GetNowHeight()), 0.0f },	{ 1.0f, 1.0f } };
	m_pVtxTimer[0] = CreateVertexBuffer(vtxTimer[0], 4);

	// 制限時間頂点情報
	for (int i = 1; i < 3; i++)
	{
		vtxTimer[i][0] = { { -WIDE_RATIO(TIMER_OUT_HARFSIZE_X,GetNowWide()) , HEIGHT_RATIO(fTimerSize          ,GetNowHeight()),0.0f },	{ 0.0f, 0.0f } };
		vtxTimer[i][1] = { { -WIDE_RATIO(TIMER_OUT_HARFSIZE_X,GetNowWide()) , HEIGHT_RATIO(TIMER_OUT_HARFSIZE_Y,GetNowHeight()),0.0f },	{ 0.0f, 1.0f } };
		vtxTimer[i][2] = { {  WIDE_RATIO(TIMER_OUT_HARFSIZE_X,GetNowWide())	, HEIGHT_RATIO(fTimerSize          ,GetNowHeight()),0.0f },	{ 1.0f, 0.0f } };
		vtxTimer[i][3] = { {  WIDE_RATIO(TIMER_OUT_HARFSIZE_X,GetNowWide())	, HEIGHT_RATIO(TIMER_OUT_HARFSIZE_Y,GetNowHeight()),0.0f },	{ 1.0f, 1.0f } };
		m_pVtxTimer[i] = CreateVertexBuffer(vtxTimer[i], 4);
	}
	// 制限時間テクスチャ読み込み
	HRESULT hr = LoadTextureFromFile(GetDevice(), TEX_PASS("Player/UI_Drawing_Gage.png"), &m_pTexTimer[0]);
	if (FAILED(hr))MessageBox(NULL, "Error:UI_Drawing_Gage.png", "Player.cpp", MB_OK);
	hr = LoadTextureFromFile(GetDevice(), TEX_PASS("Player/UI_Drawing_top.png"), &m_pTexTimer[1]);
	if (FAILED(hr))MessageBox(NULL, "Error:UI_Drawing_top.png", "Player.cpp", MB_OK);
	hr = LoadTextureFromFile(GetDevice(), TEX_PASS("Player/UI_Drawing_under.png"), &m_pTexTimer[2]);
	if (FAILED(hr))MessageBox(NULL, "Error:UI_Drawing_under.png", "Player.cpp", MB_OK);

	for (int i = 0; i < MAX_SPRITE; i++)
	{
		m_pTexture[i] = new Texture();
		m_pTexture[i]->Create(TEX_PASS("Player/Arrow.png"));
	}

	for (int i = 0; i < 8; i++)
	{
		m_tArrowInfo[i].param = new SpriteParam();

		m_tArrowInfo[i].param->size = { ARROW_SIZE ,ARROW_SIZE };
		m_tArrowInfo[i].param->rotate.z = TORAD(i * (360.0f / 8));

		m_tArrowInfo[i].state = NONE_SELECT;
	}

	// エフェクト読み込み
	//m_Effect = LibEffekseer::Create(TEX_PASS("Effect/Fire.efk"));

	g_PlayerSound = new CSoundList(SE_WALK);
	g_PlayerSound->SetMasterVolume();
	g_WalkSe = g_PlayerSound->GetSound(true);
}

CPlayer::~CPlayer()
{	
	for (int i = 0; i < 3; i++)
	{
		SAFE_RELEASE(m_pVtxTimer[i]);	// タイマー頂点情報の解放
		SAFE_RELEASE(m_pTexTimer[i]);	// タイマーテクスチャ情報の解放
	}
	SAFE_DELETE(m_pModel);		// プレイヤーモデルの解放

	//音の解放
	if (g_PlayerSound)
	{
		delete g_PlayerSound;
		g_PlayerSound = nullptr;
	}
	if (g_WalkSe)
	{
		//g_WalkSe->Stop();
		g_WalkSe = nullptr;
	}

}

void CPlayer::Update()
{
	// タイマー処理
	TimeProcess();

	// 状態別更新処理
	if (m_bDrawing)	//召喚フェーズじゃない時
	{
		switch (m_ePlayerState)
		{
		case CPlayer::STOP: UpdateStop(); break;	// 停止状態の処理
		case CPlayer::MOVE: UpdateMove(); break;	// 動いている状態の処理
		default:break;
		}
	}
	else
	{
		g_WalkSe->Stop();
		XAUDIO2_BUFFER buffer;
		buffer = g_PlayerSound->GetBuffer(true);
		g_WalkSe->FlushSourceBuffers();
		g_WalkSe->SubmitSourceBuffer(&buffer);
	}

	//m_tArrowInfo[UP].param->pos			= { m_tBrushPos.x,						m_tBrushPos.y + ARROW_AJUST_POS };
	//m_tArrowInfo[UPRIGHT].param->pos	= { m_tBrushPos.x + ARROW_AJUST_POS,	m_tBrushPos.y + ARROW_AJUST_POS };
	//m_tArrowInfo[RIGHT].param->pos		= { m_tBrushPos.x + ARROW_AJUST_POS,	m_tBrushPos.y					};
	//m_tArrowInfo[DOWNRIGHT].param->pos	= { m_tBrushPos.x + ARROW_AJUST_POS,	m_tBrushPos.y - ARROW_AJUST_POS };
	//m_tArrowInfo[DOWN].param->pos		= { m_tBrushPos.x,						m_tBrushPos.y - ARROW_AJUST_POS };
	//m_tArrowInfo[DOWNLEFT].param->pos	= { m_tBrushPos.x - ARROW_AJUST_POS,	m_tBrushPos.y - ARROW_AJUST_POS };
	//m_tArrowInfo[LEFT].param->pos		= { m_tBrushPos.x - ARROW_AJUST_POS,	m_tBrushPos.y					};
	//m_tArrowInfo[UPLEFT].param->pos		= { m_tBrushPos.x - ARROW_AJUST_POS,	m_tBrushPos.y + ARROW_AJUST_POS };

	//for (int i = 0; i < 8; i++)
	//{
	//	switch (m_tArrowInfo[i].state)
	//	{
	//	case NONE_SELECT:	m_tArrowInfo[i].param->color = { 1.0f, 1.0f, 1.0f, 0.5f }; break;
	//	case SELECTED:		m_tArrowInfo[i].param->color = { 1.0f, 1.0f, 1.0f, 1.0f }; break;
	//	case CANNOT_SELECT:	m_tArrowInfo[i].param->color = { 1.0f, 1.0f, 1.0f, 0.0f }; break;
	//	default:
	//		break;
	//	}
	//}
}

void CPlayer::Draw()
{
	/* タイマーの描画 */
	SetRender2D();											// 2D表現のセット

	SetSpritePos(WIDE_RATIO(TIMER_OUT_OFFSET_X,GetNowWide()), HEIGHT_RATIO(TIMER_OUT_OFFSET_Y,GetNowHeight()));							// タイマーの座標の設定
	SetSpriteScale(1.0f, 1.0f);								// タイマーの大きさの設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);	// タイマーの色の設定
	SetSpriteTexture(m_pTexTimer[2]);							// タイマーのテクスチャの設定
	DrawSprite(m_pVtxTimer[2], sizeof(Vertex));				// タイマーの描画

	ReSetSprite();

	SetSpritePos(WIDE_RATIO(TIMER_BAR_OFFSET_X,GetNowWide()), HEIGHT_RATIO(TIMER_BAR_OFFSET_Y,GetNowHeight()));							// タイマーの座標の設定
	SetSpriteScale(1.0f, 1.0f);								// タイマーの大きさの設定
	if (m_bDrawing)SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);	// タイマーの色の設定
	else SetSpriteColor(1.0f, 1.0f, 1.0f, 0.3f);			// 作図時間でないときは透明度を上げる
	SetSpriteTexture(m_pTexTimer[0]);							// タイマーのテクスチャの設定
	DrawSprite(m_pVtxTimer[0], sizeof(Vertex));				// タイマーの描画

	ReSetSprite();
	
	SetSpritePos(WIDE_RATIO(TIMER_OUT_OFFSET_X,GetNowWide()), HEIGHT_RATIO(TIMER_OUT_OFFSET_Y,GetNowHeight()));							// タイマーの座標の設定
	SetSpriteScale(1.0f, 1.0f);								// タイマーの大きさの設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);	// タイマーの色の設定
	SetSpriteTexture(m_pTexTimer[1]);							// タイマーのテクスチャの設定
	DrawSprite(m_pVtxTimer[1], sizeof(Vertex));				// タイマーの描画

	ReSetSprite();											// スプライト設定のリセット

	//for (int i = 0; i < 8; i++)
	//{
	//	Sprite::SetParam(m_tArrowInfo[i].param);
	//	Sprite::SetTexture(m_pTexture[Arrow]);
	//	Sprite::Draw();
	//}

	/* プレイヤーの描画 */
	SetRender3D();											// 3D表現のセット
	DrawModel();											// プレイヤー(筆)の描画

	/* エフェクトの描画 */

}

void CPlayer::UpdateStop()
{
	g_WalkSe->Stop();
	XAUDIO2_BUFFER buffer;
	buffer = g_PlayerSound->GetBuffer(true);
	g_WalkSe->FlushSourceBuffers();
	g_WalkSe->SubmitSourceBuffer(&buffer);

	for (int i = 0; i < 8; i++)
	{
		m_tArrowInfo[i].state = NONE_SELECT;
	}

	// プレイヤーのコントローラー、キーボード入力処理
	PlayerInput();

	if (!m_bCanMoveCheck)	// 移動可能か未チェック
	{
		//m_eDestination = DEFAULT;	// 移動方向を真ん中に初期化
		for (int i = 0, Count = 0; i < 8; i++)
		{
			// 8方向に行けるかどうかチェック
			// 行けない方向がある場合、その度にカウントを増やす
			if (m_pFieldVtx->GetRoadStop(i)) Count++;

			// 8方向全てに移動が出来ないなら
			if (Count == 8)
			{
				m_tArrowInfo[i].state = CANNOT_SELECT;
				m_bDrawing = false;				// 即座に作図終了
				m_bCanMoveCheck = true;			// 移動可能かのチェック終了
				fTimerSize = TIMER_BAR_HARFSIZE_Y;	// タイマーを一番下まで落とす
				return;							// 関数を抜ける
			}
		}
		m_bCanMoveCheck = true;					// 移動可能かのチェック終了
	}

	m_tArrowInfo[m_nDestination].state = SELECTED;

	// プレイヤーの座標を現在の頂点番号の座標と同じにする
	m_tBrushPos = m_pFieldVtx->GetVertexPos(m_nNowVertex);


	// 移動方向が行き止まりではない　かつ　移動方向が今の位置と一緒ではない
	if (!m_pFieldVtx->GetRoadStop(m_eDestination) && m_nNowVertex != m_nDestination)
	{
		// ×ボタンorスペースキーで移動開始
		if (CGetButtons(XINPUT_GAMEPAD_A) || IsKeyPress(VK_SPACE))
		{
			m_ePlayerState = MOVE;	// 動いている状態に変える
		}
	}
}

void CPlayer::UpdateMove()
{
	if (GetFeverMode())m_fBrushSpeed = BRUSH_SPEED_FEVER;
	else m_fBrushSpeed = BRUSH_SPEED;

	// 各種方向別移動処理
	switch (m_eDestination)
	{
	case CPlayer::UP:
		m_tBrushPos.y += m_fBrushSpeed;
		break;
	case CPlayer::UPRIGHT:
		m_tBrushPos.x += m_fBrushSpeed;
		m_tBrushPos.y += m_fBrushSpeed;
		break;
	case CPlayer::RIGHT:
		m_tBrushPos.x += m_fBrushSpeed;
		break;
	case CPlayer::DOWNRIGHT:
		m_tBrushPos.x += m_fBrushSpeed;
		m_tBrushPos.y -= m_fBrushSpeed;
		break;
	case CPlayer::DOWN:
		m_tBrushPos.y -= m_fBrushSpeed;
		break;
	case CPlayer::DOWNLEFT:
		m_tBrushPos.x -= m_fBrushSpeed;
		m_tBrushPos.y -= m_fBrushSpeed;
		break;
	case CPlayer::LEFT:
		m_tBrushPos.x -= m_fBrushSpeed;
		break;
	case CPlayer::UPLEFT:
		m_tBrushPos.x -= m_fBrushSpeed;
		m_tBrushPos.y += m_fBrushSpeed;
		break;
	default:
		break;
	}

	// プレイヤーが目的地の頂点に到着したら
	if (m_tBrushPos.x == m_pFieldVtx->GetVertexPos(m_nDestination).x && m_tBrushPos.y == m_pFieldVtx->GetVertexPos(m_nDestination).y)
	{
		m_nNowVertex = m_nDestination;	// 今の頂点を目的地の頂点に更新する
		m_bCanMoveCheck = false;		// 8方向に移動可能かどうかを再び判定する
		m_ePlayerState = STOP;			// 止まっている状態に変更する
	}

	if(g_WalkSe)SetVolumeSE(g_WalkSe);
	g_WalkSe->Start();
}

void CPlayer::DrawModel()
{
	// アニメーション
	if (m_bDrawing)	// 作図中のとき
	{
		if (m_fBrushSize >= 1.0f) m_fBrushSize = 1.0f;
		else m_fBrushSize += 0.05f;
	}
	else // 作図中でないとき
	{
		if (m_fBrushSize <= 0.0f)
		{
			m_fBrushSize = 0.0f;
			m_tBrushRotate = { DirectX::XMConvertToRadians(140.0f), DirectX::XMConvertToRadians(-20.0f), 0.0f };
		}
		else
		{
			m_fBrushSize -= 0.05f;
			m_tBrushRotate.z += DirectX::XMConvertToRadians(20.0f);
		}
	}


	SetRender3D();		// 3D表現のセット
	m_pModel->SetPostion(m_tBrushPos.x + BRUSH_AJUSTPOS_X, m_tBrushPos.y + BRUSH_AJUSTPOS_Y, m_tBrushPos.z);	// 座標のセット
	m_pModel->SetRotation(m_tBrushRotate.x, m_tBrushRotate.y, m_tBrushRotate.z);	// 回転のセット
	m_pModel->SetScale(m_fBrushSize, m_fBrushSize, m_fBrushSize);	// サイズのセット
	m_pModel->SetViewMatrix(GetView());			// View座標のセット
	m_pModel->SetProjectionMatrix(GetProj());	// Projection座標のセット
	m_pModel->Draw();	// モデルの描画
}

void CPlayer::PlayerInput()
{
	// 左スティックの状態を取得
	DirectX::XMFLOAT2 tControllerMove = CGetLStick();

	//デッドゾーン設定
	if (tControllerMove.x <= DEADZONE && tControllerMove.x >= -DEADZONE) tControllerMove.x = 0.0f;
	if (tControllerMove.y <= DEADZONE && tControllerMove.y >= -DEADZONE) tControllerMove.y = 0.0f;

	/* 目的地の更新 */
	if ((tControllerMove.x > 0.0f		&& tControllerMove.y > 0.0f) ) m_eDestination = UPRIGHT;
	else if ((tControllerMove.x >  0.0f && tControllerMove.y < 0.0f) ) m_eDestination = DOWNRIGHT;
	else if ((tControllerMove.x <  0.0f && tControllerMove.y < 0.0f) ) m_eDestination = DOWNLEFT;
	else if ((tControllerMove.x <  0.0f && tControllerMove.y > 0.0f) ) m_eDestination = UPLEFT;
	else if ((tControllerMove.x == 0.0f && tControllerMove.y > 0.0f) )m_eDestination = UP;
	else if ((tControllerMove.x >  0.0f && tControllerMove.y == 0.0f)) m_eDestination = RIGHT;
	else if ((tControllerMove.x == 0.0f && tControllerMove.y < 0.0f) ) m_eDestination = DOWN;
	else if ((tControllerMove.x <  0.0f && tControllerMove.y == 0.0f)) m_eDestination = LEFT;

	// キー入力情報の取得
	DIRECTION KeyData = WASDKeyBorad();
	switch (KeyData)
	{
	case D_above: m_eDestination = UP;				break; 
	case D_upper_right:m_eDestination = UPRIGHT;	break; 
	case D_upper_left: m_eDestination = UPLEFT;		break; 
	case D_right: m_eDestination = RIGHT;			break; 
	case D_left:m_eDestination = LEFT;				break;
	case D_under: m_eDestination = DOWN;			break;
	case D_under_right:m_eDestination = DOWNRIGHT;	break;
	case D_under_left: m_eDestination = DOWNLEFT;	break;
	case D_no:break;
	}


	// 目的地ごとに目的地の頂点を設定
	switch (m_eDestination)
	{
	case CPlayer::UP:m_nDestination = m_nNowVertex - 5;			break;
	case CPlayer::UPRIGHT:m_nDestination = m_nNowVertex - 4;	break;
	case CPlayer::RIGHT:m_nDestination = m_nNowVertex + 1;		break;
	case CPlayer::DOWNRIGHT:m_nDestination = m_nNowVertex + 6;	break;
	case CPlayer::DOWN:m_nDestination = m_nNowVertex + 5;		break;
	case CPlayer::DOWNLEFT:m_nDestination = m_nNowVertex + 4;	break;
	case CPlayer::LEFT:m_nDestination = m_nNowVertex - 1;		break;
	case CPlayer::UPLEFT:m_nDestination = m_nNowVertex - 6;		break;
	case CPlayer::DEFAULT:m_nDestination = m_nNowVertex;		break;
	default:break;
	}
}

void CPlayer::TimeProcess()
{
	//タイマースタート
	if (GetFeverMode())m_bDrawing = true;
	else
	{
		if (m_bDrawing)	// 作図中のとき
		{
			fTimerSize += (TIMER_BAR_HARFSIZE_Y * 2) / (10.0f * 60.0f);	// 時間ごとにタイマーを下げる
			if (fTimerSize >= TIMER_BAR_HARFSIZE_Y)
			{
				fTimerSize = TIMER_BAR_HARFSIZE_Y;	// 下がり切ったらその位置で固定する
				m_bDrawing = false;				// 作図を終わる
			}
		}
		else // 作図中でないとき
		{
			fTimerSize -= 5.0f;	// タイマーを上げ続ける
			if (fTimerSize <= -TIMER_BAR_HARFSIZE_Y) fTimerSize = -TIMER_BAR_HARFSIZE_Y;	// 上がり切ったらその位置で固定する

		}
	}

	// 制限時間頂点情報の更新
	vtxTimer[0][0] = { { -WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide()) ,	HEIGHT_RATIO(fTimerSize,			GetNowHeight()),0.0f },	{ 0.0f, 0.0f} };
	vtxTimer[0][1] = { { -WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide()) ,	HEIGHT_RATIO(TIMER_BAR_HARFSIZE_Y,	GetNowHeight()),0.0f },	{ 0.0f, 1.0f} };
	vtxTimer[0][2] = { {  WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide())	,	HEIGHT_RATIO(fTimerSize,			GetNowHeight()),0.0f },	{ 1.0f, 0.0f} };
	vtxTimer[0][3] = { {  WIDE_RATIO(TIMER_BAR_HARFSIZE_X,GetNowWide())	,	HEIGHT_RATIO(TIMER_BAR_HARFSIZE_Y,	GetNowHeight()),0.0f },	{ 1.0f, 1.0f} };
	m_pVtxTimer[0] = CreateVertexBuffer(vtxTimer[0], 4);
}

void CPlayer::SetFieldVertexAddress(CFieldVertex* InAddress)
{
	m_pFieldVtx = InAddress;	// CFieldVertexアドレスの取得
}

void CPlayer::SetPlayerStop()
{
	m_ePlayerState = STOP;		// プレイヤーの動きを止める
	m_bCanMoveCheck = false;	// 移動可能かのチェック再開
	m_bDrawing = true;			// 作図中にする
}
