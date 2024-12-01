#include "Character.h"
#include "DirectXTex/TextureLoad.h"
#include "FieldVertex.h"

#define MAX_CHARACTER_SEARCH_COLLISION_WIDTH(Num)  ((Num / 2) + (m_tSize.X))	//キャラクターの横の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(Num) ((Num / 2) + (m_tSize.Y))	//キャラクターの縦の索敵当たり判定(索敵範囲)

#define MAX_CHARACTER_ATK_COLLISION_WIDTH(Num)  ((m_tSize.X * (Num / 2)) + (m_tSize.X))		//キャラクターの横の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_HEIGHT(Num) ((m_tSize.Y * (Num / 2)) + (m_tSize.Y))		//キャラクターの縦の攻撃当たり判定(相手の人数)

#define AngleX (0.0f)
#define AngleY (0.0f)
#define AngleZ (0.0f)

#define PLAYER_SPLIT_X (4)	// プレイヤーテクスチャ分割数X
#define PLAYER_SPLIT_Y (4)	// プレイヤーテクスチャ分割数Y
#define ANIME_TIME (10)		// プレイヤーアニメーションカウント

enum AllyTexture
{
	Ally3,
	Ally4,
	Ally5,
	MAX_AllyTex,
};
enum AllyBufferTexture
{
	Ally6,
	Ally7,
	Ally8,
	MAX_AllyBufferTex,
};
enum EnemyTexture
{
	Enemy1,
	Enemy2,
	Enemy3,
	MAX_EnemyTex,
};

Texture* g_pAllyTex[MAX_AllyTex];
Texture* g_pAllyBufferTex[MAX_AllyBufferTex];
Texture* g_pEnemyTex[MAX_EnemyTex];

Texture* g_pAllyPlayerTex;
Texture* g_pEnemyBossTex;

Texture* g_pCollisionTex;

CFieldVertex* g_pFieldVtx;

void IninCharacterTexture(CFieldVertex* InAddress,int StageNum)	//テクスチャ読み込み
{
	g_pFieldVtx = InAddress;

	for(int i = 0; i < MAX_AllyTex;i++)
		g_pAllyTex[i] = new Texture();
	for (int i = 0; i < MAX_AllyBufferTex; i++)
		g_pAllyBufferTex[i] = new Texture();
	for(int i = 0; i < MAX_EnemyTex;i++)
		g_pEnemyTex[i] = new Texture();

	g_pCollisionTex = new Texture();

	g_pAllyPlayerTex = new Texture();
	g_pEnemyBossTex = new Texture();

	HRESULT hr;
	hr = g_pAllyTex[Ally3]->Create("Asset/味方/3.png");
	hr = g_pAllyTex[Ally4]->Create("Asset/味方/4.png");
	hr = g_pAllyTex[Ally5]->Create("Asset/味方/5.png");
	hr = g_pAllyBufferTex[Ally6]->Create("Asset/味方/6.png");
	hr = g_pAllyBufferTex[Ally7]->Create("Asset/味方/7.png");
	hr = g_pAllyBufferTex[Ally8]->Create("Asset/味方/8.png");
	hr = g_pEnemyTex[Enemy1]->Create("Asset/敵/3.png");
	hr = g_pEnemyTex[Enemy2]->Create("Asset/敵/4.png");
	hr = g_pEnemyTex[Enemy3]->Create("Asset/敵/5.png");

	hr = g_pCollisionTex->Create("Asset/Star/CLStar.png");

	hr = g_pAllyPlayerTex->Create("Asset/Player/Player.png");
	
	switch (StageNum)
	{
	case 0:
		hr = g_pEnemyBossTex->Create("Asset/敵/BossNo1.png");
		break;
	case 1:
		break;
	case 2:
		break;
	}
}

CFighter::CFighter(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress)
	:m_tStatus(St_Create)
	, m_tSearchCollision{ 0.0f,0.0f }
	, m_tAtkCollision{ 0.0f,0.0f }
	, m_tPos(FirstPos)
	, nCornerCount(InCornerCount)
	, m_fHp(0.0f)
	, m_fShield(0.0f)
	, m_fAtk(0.0f)
	, m_tAtkType(AT_Physics)
	, m_fAtkCharge(0.0f)
	, m_fAtkChargeMax(0.0f)
	, m_fAtkAnimationTime(0.0f)
	, m_fAtkAnimationMaxTime(0.0f)
	, m_bCreateInit(false)
	, m_bIsAttack(false)
	, m_bFirstBattlePosSetting(false)
	, m_nTargetNumber(-1)
	, m_pCamera(InAddress)
	, m_pSprite(nullptr)
	, m_bIsHit(false)
{
	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする

	m_pEffect = new CEffect("Asset/Player/Player.png", 4, 4);
}

CFighter::~CFighter()
{
	for (int i = 0; i < MAX_AllyTex; i++)
	{
		if (g_pAllyTex)
		{
			delete g_pAllyTex[i];
			g_pAllyTex[i] = nullptr;
		}
	}
	for (int i = 0; i < MAX_EnemyTex; i++)
	{
		if (g_pEnemyTex[i])
		{
			delete g_pEnemyTex[i];
			g_pEnemyTex[i] = nullptr;
		}
	}

	if (g_pCollisionTex)
	{
		delete g_pCollisionTex;
		g_pCollisionTex = nullptr;
	}

	if (m_pEffect)
	{
		delete m_pEffect;
		m_pEffect = nullptr;
	}
	if (m_pSprite)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
	if (g_pFieldVtx)
	{
		delete g_pFieldVtx;
		g_pFieldVtx = nullptr;
	}
	if (!m_pSprite)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
	for (int i = 0; i < 3; i++)
	{
		delete g_pAllyTex[i];
		g_pAllyTex[i] = nullptr;
	}
	for (int i = 0; i < 3; i++)
	{
		delete g_pEnemyTex[i];
		g_pEnemyTex[i] = nullptr;
	}
}

void CFighter::CollisionDraw(void)
{
	/*当たり判定テスト*/
	for (int i = 0; i < 8; i++)
	{
		//テクスチャの指定
		m_pSprite->SetTexture(g_pCollisionTex);
		switch (i)
		{
		case 0:	DrawSetting({ 
			m_tPos.X - m_tAtkCollision.Width,
			m_tPos.Y - m_tAtkCollision.Height,	
			m_tPos.Z - m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		case 1:	DrawSetting({ 
			m_tPos.X + m_tAtkCollision.Width,
			m_tPos.Y - m_tAtkCollision.Height,	
			m_tPos.Z - m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		case 2:	DrawSetting({ 
			m_tPos.X - m_tAtkCollision.Width,
			m_tPos.Y + m_tAtkCollision.Height,	
			m_tPos.Z - m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		case 3:	DrawSetting({ 
			m_tPos.X + m_tAtkCollision.Width,
			m_tPos.Y + m_tAtkCollision.Height,	
			m_tPos.Z - m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		case 4:	DrawSetting({ 
			m_tPos.X - m_tAtkCollision.Width,
			m_tPos.Y - m_tAtkCollision.Height,	
			m_tPos.Z + m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		case 5:	DrawSetting({ 
			m_tPos.X + m_tAtkCollision.Width,
			m_tPos.Y - m_tAtkCollision.Height,	
			m_tPos.Z + m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		case 6:	DrawSetting({ 
			m_tPos.X - m_tAtkCollision.Width,
			m_tPos.Y + m_tAtkCollision.Height,	
			m_tPos.Z + m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		case 7:	DrawSetting({ 
			m_tPos.X + m_tAtkCollision.Width,
			m_tPos.Y + m_tAtkCollision.Height,	
			m_tPos.Z + m_tAtkCollision.Width, },
			{ 3.0f,3.0f,3.0f });
			break;
		}
		m_pSprite->Draw();

	}
}

bool CFighter::AtkCollisionCheck(CVector3<float> InSize, CVector3<float> InPos)
{
	if (m_tPos.X - m_tAtkCollision.Width <= InPos.X - (InSize.X / 2)	/*自分の攻撃当たり判定の左端 <= 相手の左端*/
		&&
		m_tPos.X + m_tAtkCollision.Width >= InPos.X - (InSize.X / 2)	/*自分の攻撃当たり判定の右端 >= 相手の左端*/
		||
		m_tPos.X - m_tAtkCollision.Width <= InPos.X + (InSize.X / 2)	/*自分の攻撃当たり判定の左端 <= 相手の右端*/
		&&
		m_tPos.X + m_tAtkCollision.Width >= InPos.X + (InSize.X / 2)	/*自分の攻撃当たり判定の右端 >= 相手の右端*/)
	{
		if (m_tPos.Y - m_tAtkCollision.Height <= InPos.Y - (InSize.Y / 2)	/*自分の攻撃当たり判定の上端 <= 相手の下端*/
			&&
			m_tPos.Y + m_tAtkCollision.Height >= InPos.Y - (InSize.Y / 2)/*自分の攻撃当たり判定の下端 >= 相手の下端*/
			||
			m_tPos.Y - m_tAtkCollision.Height <= InPos.Y + (InSize.Y / 2)	/*自分の攻撃当たり判定の上端 <= 相手の上端*/
			&&
			m_tPos.Y + m_tAtkCollision.Height >= InPos.Y + (InSize.Y / 2)	/*自分の攻撃当たり判定の下端 >= 相手の上端*/)
		{
			if (m_tPos.Z - m_tAtkCollision.Width <= InPos.Z - (InSize.Z / 2)	/*自分の攻撃当たり判定の上端 <= 相手の下端*/
				&&
				m_tPos.Z + m_tAtkCollision.Width >= InPos.Z - (InSize.Z / 2)/*自分の攻撃当たり判定の下端 >= 相手の下端*/
				||
				m_tPos.Z - m_tAtkCollision.Width <= InPos.Z + (InSize.Z / 2)	/*自分の攻撃当たり判定の上端 <= 相手の上端*/
				&&
				m_tPos.Z + m_tAtkCollision.Width >= InPos.Z + (InSize.Z / 2)	/*自分の攻撃当たり判定の下端 >= 相手の上端*/)
			{
				return true;
			}
		}
	}
	return false;
}

bool CFighter::SearchCollisionCheck(CVector3<float> InSize, CVector3<float>InPos)
{
	if (m_tPos.X - m_tSearchCollision.Width <= InPos.X - InSize.X	/*自分の索敵当たり判定の左端 <= 相手の左端*/
		&&
		m_tPos.X + m_tSearchCollision.Width >= InPos.X - InSize.X	/*自分の索敵当たり判定の右端 >= 相手の左端*/
		||
		m_tPos.X - m_tSearchCollision.Width <= InPos.X + InSize.X	/*自分の索敵当たり判定の左端 <= 相手の右端*/
		&&
		m_tPos.X + m_tSearchCollision.Width >= InPos.X + InSize.X	/*自分の索敵当たり判定の右端 >= 相手の右端*/)
	{
		if (m_tPos.Y - m_tSearchCollision.Height <= InPos.Y - InSize.Y	/*自分の索敵当たり判定の左端 <= 相手の左端*/
			&&
			m_tPos.Y + m_tSearchCollision.Height >= InPos.Y - InSize.Y	/*自分の索敵当たり判定の右端 >= 相手の左端*/
			||
			m_tPos.Y - m_tSearchCollision.Height <= InPos.Y + InSize.Y	/*自分の索敵当たり判定の左端 <= 相手の右端*/
			&&
			m_tPos.Y + m_tSearchCollision.Height >= InPos.Y + InSize.Y	/*自分の索敵当たり判定の右端 >= 相手の右端*/)
		{
			if (m_tPos.Z - m_tSearchCollision.Width <= InPos.Z - InSize.Z	/*自分の索敵当たり判定の左端 <= 相手の左端*/
				&&
				m_tPos.Z + m_tSearchCollision.Width >= InPos.Z - InSize.Z	/*自分の索敵当たり判定の右端 >= 相手の左端*/
				||
				m_tPos.Z - m_tSearchCollision.Width <= InPos.Z + InSize.Z	/*自分の索敵当たり判定の左端 <= 相手の右端*/
				&&
				m_tPos.Z + m_tSearchCollision.Width >= InPos.Z + InSize.Z	/*自分の索敵当たり判定の右端 >= 相手の右端*/)
			{
				return true;
			}
		}
	}
	return false;
}

void CFighter::Damage(CFighter* pFighter)
{
	switch (nCornerCount)
	{
	default:
	case 3:
	case 4:
		if (m_fShield > 0.0f)
			m_fShield -= pFighter->GetAtk();
		else
			m_fHp -= pFighter->GetAtk();
		break;
	case 5:
		if (m_fShield > 0.0f)
		{
			if (pFighter->GetAtkType() == AT_Magic)
				m_fShield -= pFighter->GetAtk();
			else
				m_fShield -= pFighter->GetAtk() * 0.5f;
		}
		else
		{
			if (pFighter->GetAtkType() == AT_Magic)
				m_fHp -= pFighter->GetAtk();
			else
				m_fHp -= pFighter->GetAtk() * 0.5f;
		}
		break;
	}
	m_bIsHit = true;
	m_pEffect->Play();
}

void CFighter::DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize)
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		InPos.x,
		InPos.y,
		InPos.z,
		0.0f
	));

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		AngleX,
		AngleY,
		AngleZ,
		0.0f
	));

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = m_pCamera->GetViewMatrix();
	wvp[2] = m_pCamera->GetProjectionMatrix();

	m_pSprite->SetWorld(wvp[0]);
	m_pSprite->SetView(wvp[1]);
	m_pSprite->SetProjection(wvp[2]);
}

CAlly::CAlly(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress)
	:CFighter(InCornerCount,InSize, FirstPos,InAddress)
{
	SettingStatus();
	//Vertex vtx[] = {
	//	//背景表示の座標
	//	{{-m_tSize.X / 2, -m_tSize.Y / 2,0.0f}, {0.0f, 0.0f}},
	//	{{-m_tSize.X / 2,  m_tSize.Y / 2,0.0f}, {0.0f, 1.0f}},
	//	{{ m_tSize.X / 2, -m_tSize.Y / 2,0.0f}, {1.0f, 0.0f}},
	//	{{ m_tSize.X / 2,  m_tSize.Y / 2,0.0f}, {1.0f, 1.0f}},
	//};
	//m_pVtx = CreateVertexBuffer(vtx, 4);
}

CAlly::~CAlly()
{
	//if (m_pVtx) m_pVtx->Release();
}

void CAlly::Update(void)
{
	// 状況に応じて処理を分ける
	switch (m_tStatus)
	{
	case St_Create:	CreateUpdate();		break;
	case St_Battle:	BattleUpdate();		break;
	case St_Death:	DeathUpdate();		break;
	default:
		break;
	}

	m_pEffect->Update();
}

void CAlly::Draw(void)
{
	m_pSprite->SetTexture(g_pAllyTex[nCornerCount - 3]);

	if(m_bIsHit) m_pSprite->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	else m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	m_pSprite->Draw();

	m_bIsHit = false;

	
	m_pEffect->SetEffect3D({ m_tPos.X,m_tPos.Y,m_tPos.Z });
	m_pEffect->Draw();

	m_pSprite->ReSetSprite();
}

void CAlly::CreateUpdate(void)
{
	//生成アニメーション

	//生成移動
	m_tPos.Y -= 10.0f;
	if (m_tPos.Y < -600.0f)
	{
		//生成アニメーションが終わったら
		SetStatus(St_Battle);
		m_bFirstBattlePosSetting = false;
	}
}

void CAlly::BattleUpdate(void)
{
	//戦闘アニメーション(攻撃範囲内に敵がいたら)
	if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
	{
		//アニメーション処理

		m_fAtkAnimationTime++;
		if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
	}
	//移動アニメーション(移動していたら)

	//待機アニメーション(移動していなかったら)
}

void CAlly::DeathUpdate(void)
{
	//死亡アニメーション
	// 
	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
}

void CAlly::SettingStatus(void)
{
	switch (nCornerCount)
	{
	default:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 20.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(2);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(2);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Pentagon:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Magic;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(2);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(2);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}

CEnemy::CEnemy(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress)
	:CFighter(InCornerCount, InSize, FirstPos,InAddress)
{
	SettingStatus();
	//Vertex vtx[] = {
	//	//背景表示の座標
	//	{{-m_tSize.X / 2, -m_tSize.Y / 2, 0.0f}, {0.0f, 0.0f}},
	//	{{-m_tSize.X / 2,  m_tSize.Y / 2, 0.0f}, {0.0f, 1.0f}},
	//	{{ m_tSize.X / 2, -m_tSize.Y / 2, 0.0f}, {1.0f, 0.0f}},
	//	{{ m_tSize.X / 2,  m_tSize.Y / 2, 0.0f}, {1.0f, 1.0f}},
	//};
	//m_pVtx = CreateVertexBuffer(vtx, 4);
}

CEnemy::~CEnemy()
{
	//if (m_pVtx) m_pVtx->Release();
}

void CEnemy::Update(void)
{
	// 状況に応じて処理を分ける
	switch (m_tStatus)
	{
	case St_Create:	CreateUpdate();		break;
	case St_Battle:	BattleUpdate();		break;
	case St_Death:	DeathUpdate();		break;
	default:
		break;
	}

	m_pEffect->Update();
}

void CEnemy::Draw(void)
{
	m_pSprite->SetTexture(g_pEnemyTex[nCornerCount - 3]);

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	m_pSprite->Draw();

	m_pEffect->SetEffect3D({ m_tPos.X,m_tPos.Y,m_tPos.Z });
	m_pEffect->Draw();

	m_pSprite->ReSetSprite();
}

void CEnemy::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(St_Battle);
	m_bFirstBattlePosSetting = false;
}

void CEnemy::BattleUpdate(void)
{
	//戦闘アニメーション(攻撃範囲内に敵がいたら)
	if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
	{
		//アニメーション処理

		m_fAtkAnimationTime++;
		if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
	}
	//移動アニメーション(移動していたら)

	//待機アニメーション(移動していなかったら)
}

void CEnemy::DeathUpdate(void)
{
	//死亡アニメーション
	
	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
	
}

void CEnemy::SettingStatus(void)
{
	switch (nCornerCount)
	{
	default:
		m_fHp = 0.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(0);		//キャラクターの中心からの横の当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(0);		//キャラクターの中心からの縦の当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 10.0f;
		m_fShield = 5.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Pentagon:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;						//攻撃アニメーションの最大時間
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;	//攻撃クールタイム
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}

CAllyBuffer::CAllyBuffer(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress)
	: m_tStatus(St_Create)
	, nCornerCount((Corner)InCornerCount)
	, m_tPos(FirstPos)
	, IsBuff(false)
	, m_pCamera(InAddress)
{
	SettingStatus();

	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
}

CAllyBuffer::~CAllyBuffer()
{
	if (m_pSprite)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
}

void CAllyBuffer::Update(void)
{
	switch (m_tStatus)
	{
	case St_Create:CreateUpdate();break;
	case St_Battle:BattleUpdate();break;
	case St_Death:DeathUpdate();break;
	}
}

void CAllyBuffer::Draw(void)
{
	m_pSprite->SetTexture(g_pAllyBufferTex[nCornerCount - 6]);

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	m_pSprite->Draw();

	m_pSprite->ReSetSprite();
}

void CAllyBuffer::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(St_Battle);
}

void CAllyBuffer::BattleUpdate(void)
{
	//アニメーション(移動していなかったら)
}

void CAllyBuffer::DeathUpdate(void)
{
	//死亡アニメーション

	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
}

void CAllyBuffer::SettingStatus(void)
{
	switch (nCornerCount)
	{
	default:
		break;
	case Hexagon:
		m_tBuff = BT_Shield;
		break;
	case Heptagon:
		m_tBuff = BT_Attack;
		break;
	case Octagon:
		m_tBuff = BT_ReSummon;
		break;
	}
}

void CAllyBuffer::DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize)
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		InPos.x,
		InPos.y,
		InPos.z,
		0.0f
	));
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		AngleX,
		AngleY,
		AngleZ,
		0.0f
	));

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = m_pCamera->GetViewMatrix();
	wvp[2] = m_pCamera->GetProjectionMatrix();

	m_pSprite->SetWorld(wvp[0]);
	m_pSprite->SetView(wvp[1]);
	m_pSprite->SetProjection(wvp[2]);
}

CEnemyBoss::CEnemyBoss(int BossTypeNum, float InSize, CVector3<float> FirstPos, Camera* InAddress)
	:CFighter(BossTypeNum,InSize,FirstPos,InAddress)
{
	SettingStatus();
}

CEnemyBoss::~CEnemyBoss()
{

}

void CEnemyBoss::Update(void)
{
	switch (m_tStatus)
	{
	case St_Create:CreateUpdate();
		break;
	case St_Battle:BattleUpdate();
		break;
	case St_Death:DeathUpdate();
		break;
	}
}

void CEnemyBoss::Draw(void)
{
	m_pSprite->SetTexture(g_pEnemyBossTex);

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	m_pSprite->Draw();

	m_pSprite->ReSetSprite();
}

void CEnemyBoss::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(St_Battle);

}

void CEnemyBoss::BattleUpdate(void)
{
	//戦闘アニメーション(攻撃範囲内に敵がいたら)
	if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
	{
		//アニメーション処理

		m_fAtkAnimationTime++;
		if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
	}
	//移動アニメーション(移動していたら)

	//待機アニメーション(移動していなかったら)
}

void CEnemyBoss::DeathUpdate(void)
{
	//死亡アニメーション

	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
}

void CEnemyBoss::SettingStatus(void)
{
	switch (nCornerCount)
	{
	default:
		break;
	case Type1:
		m_fHp = 100.0f;
		m_fAtk = 5.0f;
		m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(3);			//キャラクターの中心からの横の当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(3);			//キャラクターの中心からの縦の当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);	//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Type2:
		break;
	case Type3:
		break;
	}

}

CAllyPlayer::CAllyPlayer(float InSize, CVector3<float>FirstPos, Camera* InAddress)
	:m_tStatus(St_Create)
	,m_tPos(FirstPos)
	,m_fHp(10.0f)
	,m_pCamera(InAddress)
	,m_pSprite(nullptr)
{
	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
}

CAllyPlayer::~CAllyPlayer()
{
	if (m_pSprite)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
}

void CAllyPlayer::Update(void)
{
	switch (m_tStatus)
	{
	case St_Create:CreateUpdate();
		break;
	case St_Battle:BattleUpdate();
		break;
	case St_Death:DeathUpdate();
		break;
	}
}

void CAllyPlayer::Draw(void)
{
	m_tTexPos = GetPosTex(PLAYER_SPLIT_X, PLAYER_SPLIT_Y, ANIME_TIME);

	m_pSprite->SetUVPos({ m_tTexPos.X,m_tTexPos.Y });
	m_pSprite->SetUVScale({ 0.25f,0.25f });

	m_pSprite->SetTexture(g_pAllyPlayerTex);

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	m_pSprite->Draw();
	
	m_pSprite->ReSetSprite();
}

void CAllyPlayer::Damage(CFighter* pFighter)
{
	m_fHp -= pFighter->GetAtk();
}

void CAllyPlayer::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(St_Battle);

}

void CAllyPlayer::BattleUpdate(void)
{
	//待機アニメーション(移動していなかったら)
}

void CAllyPlayer::DeathUpdate(void)
{
	//死亡アニメーション

	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
}

void CAllyPlayer::DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize)
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		InPos.x,
		InPos.y,
		InPos.z,
		0.0f
	));
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		AngleX,
		AngleY,
		AngleZ,
		0.0f
	));

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = m_pCamera->GetViewMatrix();
	wvp[2] = m_pCamera->GetProjectionMatrix();

	m_pSprite->SetWorld(wvp[0]);
	m_pSprite->SetView(wvp[1]);
	m_pSprite->SetProjection(wvp[2]);
}

CVector2<float> CAllyPlayer::GetPosTex(int nSplitX, int nSplitY, int nAnimationSwap)
{
	CVector2<float> tex;
	static int nSplit = nSplitX * nSplitY;
	static int nAnimePage = 0;
	static int nAnimeCount = 0;

	// テクスチャアニメーション用
	if (nAnimeCount >= nAnimationSwap)
	{
		nAnimePage++;	// 次のシーケンステクスチャに移る
		nAnimeCount = 0;
	}
	else nAnimeCount++;

	if (nAnimePage >= nSplit)
	{
		nAnimePage = 0;	// 最初のシーケンステクスチャに戻る
	}
	// 横のシーケンステクスチャの動き
	switch (nAnimePage % nSplitX)
	{
	default:break;
	case 0: tex.X = 0.0 / (float)nSplitX; break;
	case 1: tex.X = 1.0 / (float)nSplitX; break;
	case 2: tex.X = 2.0 / (float)nSplitX; break;
	case 3: tex.X = 3.0 / (float)nSplitX; break;
	}
	// 縦のシーケンステクスチャの動き
	switch (nAnimePage / nSplitY)
	{
	default:break;
	case 0: tex.Y = 0.0 / (float)nSplitY; break;
	case 1: tex.Y = 1.0 / (float)nSplitY; break;
	case 2: tex.Y = 2.0 / (float)nSplitY; break;
	case 3: tex.Y = 3.0 / (float)nSplitY; break;
	}

	return tex;
}
