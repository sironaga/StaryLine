// includes
#include "Player.h"
#include "Controller.h"
#include "SceneGame.h"
#include "DirectXTex/TextureLoad.h"
#include "SpriteDrawer.h"
#include "InputEx.h"
#include "Main.h"
#include "Defines.h"
#include "ShaderList.h"
#include "Geometory.h"

// defines
#define BASE_DRAWTIME (10.0f)						// ボーナス抜きの基礎制限時間
#define DRAWTIME(bonus) (BASE_DRAWTIME  + bonus)	// 制限時間

#define PLAYER_SPLIT_X (4)	// プレイヤーテクスチャ分割数X
#define PLAYER_SPLIT_Y (4)	// プレイヤーテクスチャ分割数Y
#define ANIME_TIME (10)		// プレイヤーアニメーションカウント

#define PLAYER_AJUST_POSX (3.0f)
#define PLAYER_AJUST_POSY (8.0f)

// constants
constexpr float BRUSH_SIZE = 10.0f;				// プレイヤー(筆)のサイズ
constexpr float TIMER_HARFSIZE_X = 100.0f;
constexpr float TIMER_HARFSIZE_Y = 600.0f;

CPlayer::CPlayer()
	: m_ePlayerState(STOP), m_eDestination(DEFAULT)
	//, m_pVtxPlayer(nullptr), m_pTexPlayerWait(nullptr)
	, m_nNowVertex(START_PLAYER), m_nDestination(START_PLAYER)
	, m_tBrushPos{}
	, bCanMoveCheck(false), m_bDrawing(true)
	, m_pPlayer(nullptr), m_pPlayerTex(nullptr)
	, pos{}, size{}, rotate{}, mat{}
	, m_fBrushSize(BRUSH_SIZE), m_tBrushRotate{ DirectX::XMConvertToRadians(140.0f), DirectX::XMConvertToRadians(-20.0f), 0.0f }

	, m_pVtxTimer(nullptr), m_pTexTimer(nullptr)
	, vtxTimer{},fTimerSize(-TIMER_HARFSIZE_Y)
	, tPlayerTimer{}, fDrawTime(BASE_DRAWTIME), fBonusTime(0.0f)
	, bTimerStart(false)

	, m_pFieldVtx(nullptr), m_pSprite(nullptr),m_pModel(nullptr)
{

	// プレイヤー頂点情報
	m_pPlayer->Init();

	m_pModel = new CModelEx(MODEL_PASS("Player/Lini_FountainPen.fbx"));

	// プレイヤーテクスチャ読み込み
	//HRESULT hr;
	//hr = LoadTextureFromFile(GetDevice(), "Asset/Player/Player.png", &m_pTexPlayerWait);
	//if (FAILED(hr))MessageBox(NULL, "エラー:Player.png", "Player.cpp", MB_OK);
	m_pPlayerTex = new Texture();
	HRESULT hr = m_pPlayerTex->Create("Asset/Player/Player.png");
	if (FAILED(hr))MessageBox(NULL, "エラー:Player.png", "Player.cpp", MB_OK);

	// 制限時間頂点情報
	vtxTimer[0] = { { -TIMER_HARFSIZE_X ,	fTimerSize,	0.0f },	{ 0.0f, 0.0f} };
	vtxTimer[1] = { { -TIMER_HARFSIZE_X ,	TIMER_HARFSIZE_Y,	0.0f },	{ 0.0f, 1.0f} };
	vtxTimer[2] = { {  TIMER_HARFSIZE_X	,	fTimerSize,	0.0f },	{ 1.0f, 0.0f} };
	vtxTimer[3] = { {  TIMER_HARFSIZE_X	,	TIMER_HARFSIZE_Y,	0.0f },	{ 1.0f, 1.0f} };
	m_pVtxTimer = CreateVertexBuffer(vtxTimer, 4);

	// 制限時間テクスチャ読み込み
	hr = LoadTextureFromFile(GetDevice(), "Asset/Player/Timer.jpg", &m_pTexTimer);
	if (FAILED(hr))MessageBox(NULL, "エラー:Timer.jpg", "Player.cpp", MB_OK);

	m_Effect = LibEffekseer::Create(TEX_PASS("Effect/Fire.efk"));
}

CPlayer::~CPlayer()
{
	//プレイヤー頂点情報、テクスチャ情報の解放
	if (m_pPlayer)m_pPlayer->Uninit();
	SAFE_DELETE(m_pPlayerTex);

	SAFE_RELEASE(m_pVtxTimer);
	SAFE_RELEASE(m_pTexTimer);
}

void CPlayer::Update()
{
	TimeProcess();

	// 状態別更新処理
	if (m_bDrawing)	//召喚フェーズじゃない時
	{
		switch (m_ePlayerState)
		{
		case CPlayer::STOP: UpdateStop(); break;
		case CPlayer::MOVE: UpdateMove(); break;
		default:break;
		}
	}

	if (CGetButtonsTriger(XINPUT_GAMEPAD_A) || IsKeyTrigger(VK_SPACE))
	{
		LibEffekseer::GetManager()->Play(m_Effect, m_tBrushPos.x, m_tBrushPos.y, m_tBrushPos.z);
	}
}

void CPlayer::Draw()
{
	////スプライトの座標の設定
	//SetSpritePos(m_tPos.x, m_tPos.Y);

	////大きさの設定
	//SetSpriteScale(1.0f, 1.0f);

	////背景色の設定
	//SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//// テクスチャアニメーション

	//// UVのセット


	//// 描画
	//DrawSprite(m_pVtxPlayer, sizeof(Vertex));

	// スプライト設定のリセット
	//ReSetSprite();

	//DrawAnimation();
	//DrawSprite3D(PLAYER);
	//m_pPlayer->Draw();
	//m_pPlayer->ReSetSprite();

	SetRender2D();
	//スプライトの座標の設定
	SetSpritePos(-700.0f, -400.0f);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	if (m_bDrawing)SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
	else SetSpriteColor(1.0f, 1.0f, 1.0f, 0.3f);

	//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
	SetSpriteTexture(m_pTexTimer);

	// 描画
	DrawSprite(m_pVtxTimer, sizeof(Vertex));

	// スプライト設定のリセット
	ReSetSprite();

	DrawModel(PLAYER);

	SetRender2D();
	LibEffekseer::SetViewPosition(GetCameraPos());
	LibEffekseer::SetCameraMatrix(GetView(false), GetProj(false));
}

void CPlayer::UpdateStop()
{
	if (!bCanMoveCheck)	// 移動可能か未チェック
	{
		// 移動方向を真ん中に初期化
		m_eDestination = DEFAULT;
		for (int i = 0, Count = 0; i < 8; i++)
		{
			// 8方向に行けるかどうかチェック
			// 行けない方向がある場合、その度にカウントを増やす
			if (m_pFieldVtx->GetRoadStop(i))Count++;

			// 8方向全てに移動が出来ないなら
			if (Count == 8)
			{
				m_bDrawing = false;				// 即座に作図終了
				bCanMoveCheck = true;			// 移動可能かのチェック終了
				fTimerSize = TIMER_HARFSIZE_Y;	// タイマーを一番下まで落とす
				return;					// 移動不可を返す
			}
		}
		// 移動可能かのチェック終了
		bCanMoveCheck = true;
	}


	// プレイヤーの座標を現在の頂点番号の座標と同じにする
	m_tBrushPos = m_pFieldVtx->GetVertexPos(m_nNowVertex);

	// プレイヤーのコントローラー、キーボード入力処理
	PlayerInput();

	// 移動方向が行き止まりではない　かつ　移動方向が今の位置と一緒ではない
	if (!m_pFieldVtx->GetRoadStop(m_eDestination) && m_nNowVertex != m_nDestination)
	{
		// ×ボタンorスペースキーで移動開始
		if (CGetButtons(XINPUT_GAMEPAD_A) || IsKeyPress(VK_SPACE))
		{
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
		m_tBrushPos.y += MOVESPEED;
		break;
	case CPlayer::UPRIGHT:
		m_tBrushPos.x += MOVESPEED;
		m_tBrushPos.y += MOVESPEED;
		break;
	case CPlayer::RIGHT:
		m_tBrushPos.x += MOVESPEED;
		break;
	case CPlayer::DOWNRIGHT:
		m_tBrushPos.x += MOVESPEED;
		m_tBrushPos.y -= MOVESPEED;
		break;
	case CPlayer::DOWN:
		m_tBrushPos.y -= MOVESPEED;
		break;
	case CPlayer::DOWNLEFT:
		m_tBrushPos.x -= MOVESPEED;
		m_tBrushPos.y -= MOVESPEED;
		break;
	case CPlayer::LEFT:
		m_tBrushPos.x -= MOVESPEED;
		break;
	case CPlayer::UPLEFT:
		m_tBrushPos.x -= MOVESPEED;
		m_tBrushPos.y += MOVESPEED;
		break;
	default:
		break;
	}

	// プレイヤーが目的地の頂点に到着したら
	if (m_tBrushPos.x == m_pFieldVtx->GetVertexPos(m_nDestination).x && m_tBrushPos.y == m_pFieldVtx->GetVertexPos(m_nDestination).y)
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
	DirectX::XMFLOAT2 tControllerMove = CGetLStick();

	//デッドゾーン設定
	if (tControllerMove.x <= DEADZONE && tControllerMove.x >= -DEADZONE) tControllerMove.x = 0.0f;
	if (tControllerMove.y <= DEADZONE && tControllerMove.y >= -DEADZONE) tControllerMove.y = 0.0f;

	// 目的地の更新
	if ((tControllerMove.x > 0.0f		&& tControllerMove.y > 0.0f)  || IsKeyPress('E')) m_eDestination = UPRIGHT;
	else if ((tControllerMove.x >  0.0f && tControllerMove.y < 0.0f)  || IsKeyPress('C')) m_eDestination = DOWNRIGHT;
	else if ((tControllerMove.x <  0.0f && tControllerMove.y < 0.0f)  || IsKeyPress('Z')) m_eDestination = DOWNLEFT;
	else if ((tControllerMove.x <  0.0f && tControllerMove.y > 0.0f)  || IsKeyPress('Q')) m_eDestination = UPLEFT;
	else if ((tControllerMove.x == 0.0f && tControllerMove.y > 0.0f)  || IsKeyPress('W'))m_eDestination = UP;
	else if ((tControllerMove.x >  0.0f && tControllerMove.y == 0.0f) || IsKeyPress('D')) m_eDestination = RIGHT;
	else if ((tControllerMove.x == 0.0f && tControllerMove.y < 0.0f)  || IsKeyPress('X')) m_eDestination = DOWN;
	else if ((tControllerMove.x <  0.0f && tControllerMove.y == 0.0f) || IsKeyPress('A')) m_eDestination = LEFT;
	//else m_eDestination = DEFAULT

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
	if (m_bDrawing)	// 作図してる時
	{
		fTimerSize += (TIMER_HARFSIZE_Y * 2) / (10.0f * 60.0f);
		if (fTimerSize >= TIMER_HARFSIZE_Y)
		{
			fTimerSize = TIMER_HARFSIZE_Y;
			m_bDrawing = false;
		}
	}
	else			// 作図してない時
	{
		fTimerSize -= 5.0f;
		if (fTimerSize <= -TIMER_HARFSIZE_Y)
		{
			fTimerSize = -TIMER_HARFSIZE_Y;
		}
	}

	// 制限時間頂点情報の更新
	vtxTimer[0] = { { -TIMER_HARFSIZE_X ,	fTimerSize,			0.0f },	{ 0.0f, 0.0f} };
	vtxTimer[1] = { { -TIMER_HARFSIZE_X ,	TIMER_HARFSIZE_Y,	0.0f },	{ 0.0f, 1.0f} };
	vtxTimer[2] = { {  TIMER_HARFSIZE_X	,	fTimerSize,			0.0f },	{ 1.0f, 0.0f} };
	vtxTimer[3] = { {  TIMER_HARFSIZE_X	,	TIMER_HARFSIZE_Y,	0.0f },	{ 1.0f, 1.0f} };
	m_pVtxTimer = CreateVertexBuffer(vtxTimer, 4);
}

void CPlayer::DrawModel(E_SPRITE type)
{
	DirectX::XMFLOAT4X4 world;

	if (m_bDrawing)
	{
		
		if (m_fBrushSize >= 1.0f) m_fBrushSize = 1.0f;
		else
		{
			m_fBrushSize += 0.05f;
			
		}
	}
	else
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

	switch (type)
	{
	case CPlayer::PLAYER:
		SetRender3D();
		m_pModel->SetPostion(m_tBrushPos.x + PLAYER_AJUST_POSX, m_tBrushPos.y + PLAYER_AJUST_POSY, m_tBrushPos.z);
		m_pModel->SetRotation(m_tBrushRotate.x, m_tBrushRotate.y, m_tBrushRotate.z);
		m_pModel->SetScale(m_fBrushSize, m_fBrushSize, m_fBrushSize);
		m_pModel->SetViewMatrix(GetView());
		m_pModel->SetProjectionMatrix(GetProj());
		m_pModel->Draw();
		break;
	case CPlayer::TIMER:
		break;
	default:
		break;
	}
}

void CPlayer::SetFieldVertexAddress(CFieldVertex* InAddress)
{
	// CFieldVertexアドレスの取得
	m_pFieldVtx = InAddress;
}

bool CPlayer::GetCanMove()
{
	return m_bDrawing;	// 移動可能かを返す
}

void CPlayer::SetPlayerStop()
{
	m_ePlayerState = STOP;	// プレイヤーの動きを止める
	bCanMoveCheck = false;	// 	移動可能かのチェック再開
	m_bDrawing = true;

	//m_bChangePhase = false;//フェーズの変更を初期化
	//bTimerStart = false;
}