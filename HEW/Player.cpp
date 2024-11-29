// includes
#include "Player.h"
#include "Controller.h"
#include "SceneGame.h"
#include "DirectXTex/TextureLoad.h"
#include"SpriteDrawer.h"
#include"Input.h"
// defines
#define BASE_DRAWTIME (10.0f)						// ボーナス抜きの基礎制限時間
#define DRAWTIME(bonus) (BASE_DRAWTIME  + bonus)	// 制限時間

#define PLAYER_SPLIT_X (4)	// プレイヤーテクスチャ分割数X
#define PLAYER_SPLIT_Y (4)	// プレイヤーテクスチャ分割数Y
#define ANIME_TIME (10)		// プレイヤーアニメーションカウント

#define TIMER_LEFT (1400.0f)							// タイマーのサイズ左
#define TIMER_RIGHT(time) (time * 60.0f + -TIMER_LEFT)	// タイマーのサイズ右
#define TIMER_UP (950.0f)								// タイマーのサイズ上
#define TIMER_DOWN (900.0f)								// タイマーのサイズ下

// constants
constexpr float ce_fPlayerSize = 100.0f;				// プレイヤー(筆)のサイズ

CPlayer::CPlayer()
	: m_ePlayerState(STOP), m_eDestination(DEFAULT)
	, m_pVtxPlayer(nullptr), m_pTexPlayerWait(nullptr)
	, m_nNowVertex(START_PLAYER), m_nDestination(START_PLAYER)
	, m_tPos{}, m_tPosTex{}, m_tSizeTex{}
	, bCanMoveCheck(false)

	, m_pVtxTimer(nullptr), m_pTexTimer(nullptr)
	, vtxTimer{}, fTimerSize(TIMER_RIGHT(DRAWTIME(0.0f)))
	, tPlayerTimer{}, fDrawTime(BASE_DRAWTIME), fBonusTime(0.0f)
	, bTimerStart(false)

	, m_pFieldVtx(nullptr), m_pSprite(nullptr)
{
	// プレイヤーテクスチャ初期化
	m_tPosTex.X = 0.0f;
	m_tPosTex.Y = 0.0f;
	m_tSizeTex.X = 0.25f;
	m_tSizeTex.Y = 0.25f;

	// プレイヤー頂点情報
	Vertex vtxPlayer[] = {
	{{-ce_fPlayerSize,-ce_fPlayerSize,0.0f},{ m_tPosTex.X				, m_tPosTex.Y				 }},
	{{-ce_fPlayerSize, ce_fPlayerSize,0.0f},{ m_tPosTex.X				, m_tPosTex.Y + m_tSizeTex.Y }},
	{{ ce_fPlayerSize,-ce_fPlayerSize,0.0f},{ m_tPosTex.X + m_tSizeTex.X, m_tPosTex.Y				 }},
	{{ ce_fPlayerSize, ce_fPlayerSize,0.0f},{ m_tPosTex.X + m_tSizeTex.X, m_tPosTex.Y + m_tSizeTex.Y }},
	};
	m_pVtxPlayer = CreateVertexBuffer(vtxPlayer, 4);

	// プレイヤーテクスチャ読み込み
	HRESULT hr;
	hr = LoadTextureFromFile(GetDevice(), "Asset/Player/Player.png", &m_pTexPlayerWait);
	if (FAILED(hr))MessageBox(NULL, "エラー:Player.png", "Player.cpp", MB_OK);

	// 制限時間頂点情報
	vtxTimer[0][0] = { { -TIMER_LEFT,	-TIMER_UP,		0.0f },	{ 0.0f, 0.0f} };
	vtxTimer[0][1] = { { -TIMER_LEFT,	-TIMER_DOWN,	0.0f },	{ 0.0f, 1.0f} };
	vtxTimer[0][2] = { {  fTimerSize,	-TIMER_UP,		0.0f },	{ 1.0f, 0.0f} };
	vtxTimer[0][3] = { {  fTimerSize,	-TIMER_DOWN,	0.0f },	{ 1.0f, 1.0f} };
	m_pVtxTimer = CreateVertexBuffer(vtxTimer, 4);

	// 制限時間テクスチャ読み込み
	hr = LoadTextureFromFile(GetDevice(), "Asset/Player/Timer.jpg", &m_pTexTimer);
	if (FAILED(hr))MessageBox(NULL, "エラー:Timer.jpg", "Player.cpp", MB_OK);
}

CPlayer::~CPlayer()
{
	//プレイヤー頂点情報、テクスチャ情報の解放
	if (m_pVtxPlayer)m_pVtxPlayer->Release();
	if (m_pTexPlayerWait)m_pTexPlayerWait->Release();

	if (m_pVtxTimer)m_pVtxTimer->Release();
	if (m_pTexTimer)m_pTexTimer->Release();
}

void CPlayer::Update()
{
	TimeProcess();

	// 状態別更新処理
	switch (m_ePlayerState)
	{
	case CPlayer::READY:UpdateReady(); break;
	case CPlayer::STOP: UpdateStop(); break;
	case CPlayer::MOVE: UpdateMove(); break;
	default:break;
	}
}

void CPlayer::Draw()
{
	//スプライトの座標の設定
	SetSpritePos(m_tPos.X, m_tPos.Y);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャアニメーション
	DrawAnimation();

	// UVのセット
	SetSpriteUVPos(m_tPosTex.X, m_tPosTex.Y);
	SetSpriteUVScale(1.0f, 1.0f);

	// 描画
	DrawSprite(m_pVtxPlayer, sizeof(Vertex));

	// スプライト設定のリセット
	ReSetSprite();

	//スプライトの座標の設定
	SetSpritePos(0.0f, 0.0f);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
	SetSpriteTexture(m_pTexTimer);

	// 描画
	DrawSprite(m_pVtxTimer, sizeof(Vertex));

	// スプライト設定のリセット
	ReSetSprite();
}

void CPlayer::UpdateReady()
{
	// ×ボタンorスペースキーでスタート
	if (CGetButtons(XINPUT_GAMEPAD_A) || IsKeyPress(VK_SPACE)) m_ePlayerState = STOP;
}

void CPlayer::UpdateStop()
{
	// プレイヤーの座標を現在の頂点番号の座標と同じにする
	m_tPos = m_pFieldVtx->GetVertexPos(m_nNowVertex);

	// 八方向いずれかに移動可能か確認
	if (!bCanMoveCheck)
	{
		// 移動方向を真ん中に初期化
		m_eDestination = DEFAULT;
		for (int i = 0, Count = 0; i < 8; i++)
		{
			// 8方向に行けるかどうかチェック
			// 行けない方向がある場合、その度にカウントを増やす
			if (m_pFieldVtx->GetRoadStop(i))Count++;

			// 8方向全てに移動が出来ないなら即座にクールタイムに移る
			if (Count == 8)
			{
				ChangePhase(SHAPESCHECK);
				fTimerSize = -TIMER_LEFT;
				bCanMoveCheck = true;
				return;
			}
		}
		// 移動可能かのチェック終了
		bCanMoveCheck = true;
	}

	// プレイヤーのコントローラー、キーボード入力処理
	PlayerInput();

	// 移動方向が行き止まりではない　かつ　移動方向が今の位置と一緒ではない
	if (!m_pFieldVtx->GetRoadStop(m_eDestination) && m_nNowVertex != m_nDestination)
	{
		// ×ボタンorスペースキーで移動開始
		if (CGetButtons(XINPUT_GAMEPAD_A) || IsKeyPress(VK_SPACE))
		{
			bTimerStart = true;
			m_ePlayerState = MOVE;
		}
	}
}

void CPlayer::UpdateMove()
{
	// 各種方向別移動処理
	switch (m_eDestination)
	{
	case CPlayer::UP:
		m_tPos.Y -= MOVESPEED;
		break;
	case CPlayer::UPRIGHT:
		m_tPos.X += MOVESPEED;
		m_tPos.Y -= MOVESPEED;
		break;
	case CPlayer::RIGHT:
		m_tPos.X += MOVESPEED;
		break;
	case CPlayer::DOWNRIGHT:
		m_tPos.X += MOVESPEED;
		m_tPos.Y += MOVESPEED;
		break;
	case CPlayer::DOWN:
		m_tPos.Y += MOVESPEED;
		break;
	case CPlayer::DOWNLEFT:
		m_tPos.X -= MOVESPEED;
		m_tPos.Y += MOVESPEED;
		break;
	case CPlayer::LEFT:
		m_tPos.X -= MOVESPEED;
		break;
	case CPlayer::UPLEFT:
		m_tPos.X -= MOVESPEED;
		m_tPos.Y -= MOVESPEED;
		break;
	default:
		break;
	}

	// プレイヤーが目的地の頂点に到着したら
	if (m_tPos.X == m_pFieldVtx->GetVertexPos(m_nDestination).X && m_tPos.Y == m_pFieldVtx->GetVertexPos(m_nDestination).Y)
	{
		// 今の頂点を目的地の頂点に更新する
		m_nNowVertex = m_nDestination;
		// 8方向に移動可能かどうかを再び判定する
		bCanMoveCheck = false;
		// 止まっている状態に変更する
		m_ePlayerState = STOP;
	}
}

void CPlayer::PlayerInput()
{
	// 左スティックの状態を取得
	FLOAT2 tControllerMove = CGetLStick();

	//デッドゾーン設定
	if (tControllerMove.X <= DEADZONE && tControllerMove.X >= -DEADZONE) tControllerMove.X = 0.0f;
	if (tControllerMove.Y <= DEADZONE && tControllerMove.Y >= -DEADZONE) tControllerMove.Y = 0.0f;

	// 目的地の更新
	if ((tControllerMove.X == 0.0f && tControllerMove.Y > 0.0f) || IsKeyPress('W'))m_eDestination = UP;
	else if ((tControllerMove.X > 0.0f && tControllerMove.Y > 0.0f) || IsKeyPress('E')) m_eDestination = UPRIGHT;
	else if ((tControllerMove.X > 0.0f && tControllerMove.Y == 0.0f) || IsKeyPress('D')) m_eDestination = RIGHT;
	else if ((tControllerMove.X > 0.0f && tControllerMove.Y < 0.0f) || IsKeyPress('C')) m_eDestination = DOWNRIGHT;
	else if ((tControllerMove.X == 0.0f && tControllerMove.Y < 0.0f) || IsKeyPress('X')) m_eDestination = DOWN;
	else if ((tControllerMove.X < 0.0f && tControllerMove.Y < 0.0f) || IsKeyPress('Z')) m_eDestination = DOWNLEFT;
	else if ((tControllerMove.X < 0.0f && tControllerMove.Y == 0.0f) || IsKeyPress('A')) m_eDestination = LEFT;
	else if ((tControllerMove.X < 0.0f && tControllerMove.Y > 0.0f) || IsKeyPress('Q')) m_eDestination = UPLEFT;
	//else m_eDestination = DEFAULT;

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

void CPlayer::SetPlayerStop()
{
	m_ePlayerState = STOP;
}

void CPlayer::TimeProcess()
{
	//タイマースタート
	if (!bTimerStart)
	{
		fTimerSize = TIMER_RIGHT(DRAWTIME(fBonusTime));
		tPlayerTimer.time = tPlayerTimer.oldTime = timeGetTime();

	}
	else
	{
		// 現在時刻の更新
		tPlayerTimer.time = timeGetTime();

		// 制限時間がきたら図形判定に移る
		if (tPlayerTimer.time - tPlayerTimer.oldTime >= SECONDS(DRAWTIME(fBonusTime)) && m_ePlayerState != MOVE)
		{
			ChangePhase(SHAPESCHECK);
			fTimerSize = TIMER_RIGHT(DRAWTIME(fBonusTime));
			bTimerStart = false;
			return;
		}
		else
		{
			// 時間に応じてタイマーを減らす
			// タイマーの左端と右端の差の絶対値を取って、それを600フレームで消費する
			fTimerSize -= (TIMER_LEFT - TIMER_RIGHT(DRAWTIME(fBonusTime)) * -1) / (DRAWTIME(fBonusTime) * 60);
			if (fTimerSize <= -TIMER_LEFT) fTimerSize = -TIMER_LEFT;
		}
	}
	// 制限時間頂点情報の更新
	vtxTimer[0][0] = { { -TIMER_LEFT,	-TIMER_UP,	0.0f },	{ 0.0f, 0.0f} };
	vtxTimer[0][1] = { { -TIMER_LEFT,	-TIMER_DOWN,0.0f },	{ 0.0f, 1.0f} };
	vtxTimer[0][2] = { {  fTimerSize,	-TIMER_UP,	0.0f },	{ 1.0f, 0.0f} };
	vtxTimer[0][3] = { {  fTimerSize,	-TIMER_DOWN,0.0f },	{ 1.0f, 1.0f} };
	m_pVtxTimer = CreateVertexBuffer(vtxTimer, 4);
}

void CPlayer::DrawAnimation()
{
	// プレイヤー状態別アニメーション
	switch (m_ePlayerState)
	{
	case CPlayer::STOP: SetSpriteTexture(m_pTexPlayerWait);	break; // 待機モーション
	case CPlayer::MOVE: SetSpriteTexture(m_pTexPlayerWait); break;	// 動いてるモーションに変える(後から)
	default:break;
	}

	m_tPosTex = m_pSprite->GetPosTex(PLAYER_SPLIT_X, PLAYER_SPLIT_Y, ANIME_TIME);
}

void CPlayer::SetFieldVertexAddress(CFieldVertex* InAddress)
{
	// CFieldVertexアドレスの取得
	m_pFieldVtx = InAddress;
}