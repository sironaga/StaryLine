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

constexpr float TIMER_BARSIZE_X = 50.0f;		// タイマーゲージの横ハーフサイズ
constexpr float TIMER_BARSIZE_Y = 430.0f;		// タイマーゲージの縦ハーフサイズ
constexpr float TIMER_OUTSIZE_X = 130.0f;		// タイマー入れ物の横ハーフサイズ
constexpr float TIMER_OUTSIZE_Y = 600.0f;		// タイマー入れ物の縦ハーフサイズ

constexpr float TIMER_BAR_OFFSET_X = -350.0;		// タイマーゲージの横オフセット
constexpr float TIMER_BAR_OFFSET_Y = 160.0f;		// タイマーゲージの縦オフセット

constexpr float DRAW_TIME = 10.0f;				// 作図時間
constexpr float RECOVER_TIME = 5.0f;			// 回復時間

constexpr float ARROW_AJUST_POS =  100.0f;		// 矢印の座標X,Yの補正値
constexpr float ARROW_BRUSH_AJUST_X = 0.0f;	
constexpr float ARROW_BRUSH_AJUST_Y = -100.0f;	
constexpr float ARROW_SIZE =  1.0f;			// 矢印の座標X,Yの補正値

IXAudio2SourceVoice* g_pWalkSe;
CSoundList* g_pPlayerSound;

CPlayer::CPlayer()
// プレイヤー(筆)の初期化処理
	: m_pModel(nullptr)
	, m_tBrushPos{}, m_fBrushSize(BRUSH_SIZE), m_tBrushRotate{ DirectX::XMConvertToRadians(BRUSH_ROTATE_X), DirectX::XMConvertToRadians(BRUSH_ROTATE_Y), 0.0f }
	, m_fBrushSpeed(BRUSH_SPEED), m_fAddSpeed(BRUSH_SPEED)
	, m_nNowVertex(START_PLAYER), m_nDestination(START_PLAYER)
	, m_ePlayerState(STOP), m_eDestination(DEFAULT)
	, m_bCanMoveCheck(false), m_bDrawing(true)
	, m_pTimerParam{}, m_pArrowParam{}
	, m_pTimerTex{}
	, m_eArrowState{}
	, m_tArrowCenterPos{}

	// FieldVertexアドレスの初期化処理
	, m_pFieldVtx(nullptr)
{
	const char* pass[Timer_Max]
	{
		TEX_PASS("Player/UI_Drawing_under.png"),
		TEX_PASS("Player/UI_Drawing_Gauge.png"),
		TEX_PASS("Player/UI_Drawing_top.png"),
	};
	for (int i = 0; i < Timer_Max; i++)
	{
		m_pTimerTex[i] = new Texture();
		m_pTimerTex[i]->Create(pass[i]);
	}
	for (int i = 0; i < Timer_Max; i++)
	{
		m_pTimerParam[i] = new SpriteParam();
		m_pTimerParam[i]->size = { TIMER_OUTSIZE_X,TIMER_OUTSIZE_Y };
		m_pTimerParam[i]->pos = { TIMER_BAR_OFFSET_X,TIMER_BAR_OFFSET_Y };
		m_pTimerParam[i]->color = { 1.0f,1.0f,1.0f,1.0f };
		m_pTimerParam[i]->uvPos = { 0.0f,0.0f };
		m_pTimerParam[i]->uvSize = { 1.0f,1.0f };
		m_pTimerParam[i]->world = Get2DWorld();
		m_pTimerParam[i]->view = Get2DView();
		m_pTimerParam[i]->proj = Get2DProj();
	}
	m_pTimerParam[Timer_Gauge]->size = { TIMER_BARSIZE_X,TIMER_BARSIZE_Y };

	m_pArrowModel = new CModelEx(MODEL_PASS("Player/Board_Arrow.fbx"));
	m_eArrowState = NONE_SELECT;
	m_pArrowParam.pos = { 0.0f,0.0f,0.0f};
	m_pArrowParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_pArrowParam.rotate = { 0.0f,0.0f,0.0f };
	m_pArrowParam.uvPos = { 0.0f,0.0f };
	m_pArrowParam.uvSize = { 1.0f,1.0f };

	m_pModel = new CModelEx(MODEL_PASS("Player/Lini_FountainPen.fbx"));


	g_pPlayerSound = new CSoundList(SE_WALK);
	g_pPlayerSound->SetMasterVolume();
	g_pWalkSe = g_pPlayerSound->GetSound(true);
	m_bool = false;
}

CPlayer::~CPlayer()
{	
	SAFE_DELETE(m_pArrowModel);
	SAFE_DELETE(m_pModel);		// プレイヤーモデルの解放

	//音の解放
	SAFE_DELETE(g_pPlayerSound);
	if (g_pWalkSe)
	{
		//g_WalkSe->Stop();
		g_pWalkSe = nullptr;
	}

}

void CPlayer::Update()
{
	if (GetTimeStart())//Timeが動いているとき
	{
		m_bDrawing = true;			// 作図中にする
	}
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
		g_pWalkSe->Stop();
		XAUDIO2_BUFFER buffer;
		buffer = g_pPlayerSound->GetBuffer(true);
		g_pWalkSe->FlushSourceBuffers();
		g_pWalkSe->SubmitSourceBuffer(&buffer);
	}



}

void CPlayer::Draw()
{
	/* タイマーの描画 */
	SetRender2D();											// 2D表現のセット

	//SetSpritePos(WIDE_RATIO(TIMER_OUT_OFFSET_X,GetNowWide()), HEIGHT_RATIO(TIMER_OUT_OFFSET_Y,GetNowHeight()));							// タイマーの座標の設定
	//SetSpriteScale(1.0f, 1.0f);								// タイマーの大きさの設定
	//SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);	// タイマーの色の設定
	//SetSpriteTexture(m_pTexTimer[2]);							// タイマーのテクスチャの設定
	//DrawSprite(m_pVtxTimer[2], sizeof(Vertex));				// タイマーの描画

	//ReSetSprite();

	//SetSpritePos(WIDE_RATIO(TIMER_BAR_OFFSET_X,GetNowWide()), HEIGHT_RATIO(TIMER_BAR_OFFSET_Y,GetNowHeight()));							// タイマーの座標の設定
	//SetSpriteScale(1.0f, 1.0f);								// タイマーの大きさの設定
	//if (m_bDrawing)SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);	// タイマーの色の設定
	//else SetSpriteColor(1.0f, 1.0f, 1.0f, 0.3f);			// 作図時間でないときは透明度を上げる
	//SetSpriteTexture(m_pTexTimer[0]);							// タイマーのテクスチャの設定
	//DrawSprite(m_pVtxTimer[0], sizeof(Vertex));				// タイマーの描画

	//ReSetSprite();
	//
	//SetSpritePos(WIDE_RATIO(TIMER_OUT_OFFSET_X,GetNowWide()), HEIGHT_RATIO(TIMER_OUT_OFFSET_Y,GetNowHeight()));							// タイマーの座標の設定
	//SetSpriteScale(1.0f, 1.0f);								// タイマーの大きさの設定
	//SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);	// タイマーの色の設定
	//SetSpriteTexture(m_pTexTimer[1]);							// タイマーのテクスチャの設定
	//DrawSprite(m_pVtxTimer[1], sizeof(Vertex));				// タイマーの描画

	//ReSetSprite();											// スプライト設定のリセット


	//for (int i = 0; i < 8; i++)
	//{
	//	Sprite::SetParam(m_tArrowInfo[i].param);
	//	Sprite::SetTexture(m_pArrowTex);
	//	Sprite::Draw();
	//}

	if (m_bDrawing)m_pTimerParam[Timer_Gauge]->color.w = 1.0f;
	else m_pTimerParam[Timer_Gauge]->color.w = 0.1f;
	for (int i = 0; i < 3; i++)
	{
		Sprite::SetParam(m_pTimerParam[i]);
		Sprite::SetTexture(m_pTimerTex[i]);
		Sprite::Draw();
		Sprite::ReSetSprite();
	}

	float deg = m_eDestination * (360.0f / 8.0f);
	m_pArrowParam.rotate.z = DirectX::XMConvertToRadians(deg);
	m_pArrowParam.size = { ARROW_SIZE ,ARROW_SIZE,ARROW_SIZE };

	m_pArrowModel->SetPostion(m_tBrushPos.x, m_tBrushPos.y, m_tBrushPos.z);
	m_pArrowModel->SetScale(m_pArrowParam.size.x, m_pArrowParam.size.y, m_pArrowParam.size.z);
	m_pArrowModel->SetRotation(m_pArrowParam.rotate.x, m_pArrowParam.rotate.y, m_pArrowParam.rotate.z);
	m_pArrowModel->SetViewMatrix(GetView());
	m_pArrowModel->SetProjectionMatrix(GetProj());
	/*if(m_eArrowState == SELECTED)*/
	m_pArrowModel->Draw();

	/* プレイヤーの描画 */
	SetRender3D();											// 3D表現のセット
	DrawModel();											// プレイヤー(筆)の描画

	/* エフェクトの描画 */

}

void CPlayer::Reset()
{
	m_bDrawing = false;	// 作図中を解除
	m_tBrushPos = m_pFieldVtx->GetVertexPos(m_nNowVertex);
	m_pTimerParam[Timer_Gauge]->size.y += TIMER_BARSIZE_Y / (RECOVER_TIME  * 60.0f);	// タイマーを上げ続ける
	if (m_pTimerParam[Timer_Gauge]->size.y >= TIMER_BARSIZE_Y) m_pTimerParam[Timer_Gauge]->size.y = TIMER_BARSIZE_Y;	// 上がり切ったらその位置で固定する

	m_pTimerParam[Timer_Gauge]->pos = { TIMER_BAR_OFFSET_X, TIMER_BAR_OFFSET_Y + m_pTimerParam[Timer_Gauge]->size.y / 2.0f - TIMER_BARSIZE_Y / 2.0f };
}

void CPlayer::UpdateStop()
{
	g_pWalkSe->Stop();
	XAUDIO2_BUFFER buffer;
	buffer = g_pPlayerSound->GetBuffer(true);
	g_pWalkSe->FlushSourceBuffers();
	g_pWalkSe->SubmitSourceBuffer(&buffer);

	// プレイヤーのコントローラー、キーボード入力処理
	m_eArrowState = NONE_SELECT;

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
				m_bool = true;
				m_eArrowState = CANNOT_SELECT;
				m_bDrawing = false;				// 即座に作図終了
				m_bCanMoveCheck = true;			// 移動可能かのチェック終了
				m_pTimerParam[Timer_Gauge]->size.y  = 0.0f;				// タイマーを一番下まで落とす
				return;							// 関数を抜ける
			}
		}
		m_bCanMoveCheck = true;					// 移動可能かのチェック終了
	}
	// プレイヤーのコントローラー、キーボード入力処理
	//PlayerInput();


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

	if(g_pWalkSe)SetVolumeSE(g_pWalkSe);
	g_pWalkSe->Start();
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
	if ((tControllerMove.x > 0.0f && tControllerMove.y > 0.0f)) m_eDestination = UPRIGHT;
	else if ((tControllerMove.x > 0.0f && tControllerMove.y < 0.0f)) m_eDestination = DOWNRIGHT;
	else if ((tControllerMove.x < 0.0f && tControllerMove.y < 0.0f)) m_eDestination = DOWNLEFT;
	else if ((tControllerMove.x < 0.0f && tControllerMove.y > 0.0f)) m_eDestination = UPLEFT;
	else if ((tControllerMove.x == 0.0f && tControllerMove.y > 0.0f))m_eDestination = UP;
	else if ((tControllerMove.x > 0.0f && tControllerMove.y == 0.0f)) m_eDestination = RIGHT;
	else if ((tControllerMove.x == 0.0f && tControllerMove.y < 0.0f)) m_eDestination = DOWN;
	else if ((tControllerMove.x < 0.0f && tControllerMove.y == 0.0f)) m_eDestination = LEFT;
	else m_eDestination = DEFAULT;

	// キー入力情報の取得
	DIRECTION KeyData = WASDKeyBorad();

	switch (KeyData)
	{
	case D_above: m_eDestination = UP;				break; 
	case D_upper_right:
		m_eDestination = UPRIGHT;
		m_pTimerTex[0]->GetWidth();
		break; 
	case D_upper_left: m_eDestination = UPLEFT;		break; 
	case D_right: m_eDestination = RIGHT;			break; 
	case D_left:m_eDestination = LEFT;				break;
	case D_under: m_eDestination = DOWN;			break;
	case D_under_right:m_eDestination = DOWNRIGHT;	break;
	case D_under_left: m_eDestination = DOWNLEFT;	break;
	case D_no:m_eDestination = DEFAULT; break;
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

void CPlayer::ArrowProcess()
{
	if (m_eDestination < 0 || m_eDestination > 7)return;
	int no = m_eDestination;
	if (!m_pFieldVtx->GetRoadStop(no))m_eArrowState = SELECTED;
	else m_eArrowState = CANNOT_SELECT;	
}

void CPlayer::SetMoveStop()
{
	m_bool = false;
}

void CPlayer::TimeProcess()
{
	//タイマースタート
	if (GetFeverMode())m_bDrawing = true;
	else
	{
 		if (m_bDrawing && GetTimeStart())	// 作図中のとき
		{
			m_pTimerParam[Timer_Gauge]->size.y -= TIMER_BARSIZE_Y / (DRAW_TIME * 60.0f);	// 時間ごとにタイマーを下げる
			if (m_pTimerParam[Timer_Gauge]->size.y <= 0.0f)
			{
				m_pTimerParam[Timer_Gauge]->size.y = 0.0f;	// 下がり切ったらその位置で固定する
				m_bDrawing = false;				// 作図を終わる
			}
		}
		m_pTimerParam[Timer_Gauge]->pos = { TIMER_BAR_OFFSET_X, TIMER_BAR_OFFSET_Y + m_pTimerParam[Timer_Gauge]->size.y / 2.0f - TIMER_BARSIZE_Y / 2.0f };
	}
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
	m_bool = false;
}
