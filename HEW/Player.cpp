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

#define TIMER_LEFT (1400.0f)							// タイマーのサイズ左
#define TIMER_RIGHT(time) (time * 60.0f + -TIMER_LEFT)	// タイマーのサイズ右
#define TIMER_UP (950.0f)								// タイマーのサイズ上
#define TIMER_DOWN (900.0f)								// タイマーのサイズ下

// constants
constexpr float ce_fPlayerSize = 10.0f;				// プレイヤー(筆)のサイズ

CPlayer::CPlayer()
	: m_ePlayerState(STOP), m_eDestination(DEFAULT)
	//, m_pVtxPlayer(nullptr), m_pTexPlayerWait(nullptr)
	, m_nNowVertex(START_PLAYER), m_nDestination(START_PLAYER)
	, m_tPos{}, m_tPosTex{ 0.0f, 0.0f }, m_tSizeTex{ 0.25f ,0.25f }
	, bCanMoveCheck(false), m_bChangePhase(false)
	, m_pPlayer(nullptr), m_pPlayerTex(nullptr)
	, pos{}, size{}, rotate{}, mat{}

	, m_pVtxTimer(nullptr), m_pTexTimer(nullptr)
	, vtxTimer{}, fTimerSize(TIMER_RIGHT(DRAWTIME(0.0f)))
	, tPlayerTimer{}, fDrawTime(BASE_DRAWTIME), fBonusTime(0.0f)
	, bTimerStart(false)

	, m_pFieldVtx(nullptr), m_pSprite(nullptr),m_pModel(nullptr)
{

	// プレイヤー頂点情報
	m_pPlayer->Init();

	m_pModel = new Model();
	m_pModel->Load(MODEL_PASS("Scale_Standard.fbx", 1.0f, Model::XFlip));

	// プレイヤーテクスチャ読み込み
	//HRESULT hr;
	//hr = LoadTextureFromFile(GetDevice(), "Asset/Player/Player.png", &m_pTexPlayerWait);
	//if (FAILED(hr))MessageBox(NULL, "エラー:Player.png", "Player.cpp", MB_OK);
	m_pPlayerTex = new Texture();
	HRESULT hr = m_pPlayerTex->Create("Asset/Player/Player.png");
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
	if (m_pPlayer)m_pPlayer->Uninit();
	SAFE_DELETE(m_pPlayerTex);

	SAFE_RELEASE(m_pVtxTimer);
	SAFE_RELEASE(m_pTexTimer);
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

	DrawAnimation();
	DrawSprite3D(PLAYER);
	m_pPlayer->Draw();
	m_pPlayer->ReSetSprite();

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
	ReSetSprite();;

	//頂点シェーダーに渡す変換行列を作成
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world, view, proj;
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		-70.0f,
		0.0f,
		0.0f,
		0.0f
	));
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		0.0f,
		0.0f
	));

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	// 計算用のデータから読み取り用のデータに変換
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(mat));
	//DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
	//DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));

	wvp[1] = GetView();// 修正してください
	wvp[2] = GetProj();// 修正してください

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	//シェーダーへ変換行列を設定
	ShaderList::SetWVP(wvp); // 引数にはXMFLOAT4X4型の、要素数３の配列のアドレスを渡すこと

	// モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	// 複数のメッシュで構成されている場合、ある部分は金属的な表現、ある部分は非金属的な表現と
	//  分ける場合がある。前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
	//  切り替える。
	for (int j = 0; j < m_pModel->GetMeshNum(); j++)
	{
		//	  モデルのメッシュを取得
		Model::Mesh mesh = *m_pModel->GetMesh(j);
		//	  メッシュに割り当てられているマテリアルを取得
		Model::Material material = *m_pModel->GetMaterial(mesh.materialID);
		//		  シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);
		//		  モデルの描画
		m_pModel->Draw(j);
	}


}

void CPlayer::UpdateReady()
{
	// ×ボタンorスペースキーでスタート
	if (WithGetKeyTriger(VK_SPACE,XINPUT_GAMEPAD_B)) m_ePlayerState = STOP;
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
				m_bChangePhase = true;
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
		m_tPos.y += MOVESPEED;
		break;
	case CPlayer::UPRIGHT:
		m_tPos.x += MOVESPEED;
		m_tPos.y += MOVESPEED;
		break;
	case CPlayer::RIGHT:
		m_tPos.x += MOVESPEED;
		break;
	case CPlayer::DOWNRIGHT:
		m_tPos.x += MOVESPEED;
		m_tPos.y -= MOVESPEED;
		break;
	case CPlayer::DOWN:
		m_tPos.y -= MOVESPEED;
		break;
	case CPlayer::DOWNLEFT:
		m_tPos.x -= MOVESPEED;
		m_tPos.y -= MOVESPEED;
		break;
	case CPlayer::LEFT:
		m_tPos.x -= MOVESPEED;
		break;
	case CPlayer::UPLEFT:
		m_tPos.x -= MOVESPEED;
		m_tPos.y += MOVESPEED;
		break;
	default:
		break;
	}

	// プレイヤーが目的地の頂点に到着したら
	if (m_tPos.x == m_pFieldVtx->GetVertexPos(m_nDestination).x && m_tPos.y == m_pFieldVtx->GetVertexPos(m_nDestination).y)
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
	if (!bTimerStart)
	{
		fTimerSize = TIMER_RIGHT(DRAWTIME(fBonusTime));
		tPlayerTimer.time = tPlayerTimer.oldTime = timeGetTime();
		bTimerStart = true;
	}
	else
	{
		// 現在時刻の更新
		tPlayerTimer.time = timeGetTime();

		// 制限時間がきたら図形判定に移る
		if (tPlayerTimer.time - tPlayerTimer.oldTime >= SECONDS(DRAWTIME(fBonusTime)))
		{
			fTimerSize = TIMER_RIGHT(DRAWTIME(fBonusTime));
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
	case CPlayer::STOP: m_pPlayer->SetTexture(m_pPlayerTex);	break; // 待機モーション
	case CPlayer::MOVE: m_pPlayer->SetTexture(m_pPlayerTex);	break;	// 動いてるモーションに変える(後から)
	default:break;
	}

	m_tPosTex = m_pSprite->GetPosTex(PLAYER_SPLIT_X, PLAYER_SPLIT_Y, ANIME_TIME);

	m_pPlayer->SetUVPos(m_tPosTex);
	m_pPlayer->SetUVScale(m_tSizeTex);
}

void CPlayer::DrawSprite3D(E_SPRITE type)
{
	switch (type)
	{
	case CPlayer::PLAYER:
		pos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x, m_tPos.y, m_tPos.z, 0.0f));
		size = DirectX::XMMatrixScaling(ce_fPlayerSize, ce_fPlayerSize, 0.0f);
		rotate = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		mat = size * rotate * pos;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, mat);
		m_pPlayer->SetWorld(world);
		m_pPlayer->SetView(GetView());
		m_pPlayer->SetProjection(GetProj());
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

void CPlayer::SetPlayerStop()
{
	m_ePlayerState = STOP;//プレイヤーの動きを止める
	bCanMoveCheck = false;//
	m_bChangePhase = false;//フェーズの変更を初期化
	bTimerStart = false;
}