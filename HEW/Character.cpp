#include "Character.h"
#include "DirectXTex/TextureLoad.h"
#include "FieldVertex.h"
#include "Main.h"
#include "SoundList.h"

#define MAX_CHARACTER_SEARCH_COLLISION_WIDTH(Num)  ((Num / 2) + (m_tSize.X))	//キャラクターの横の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(Num) ((Num / 2) + (m_tSize.Y))	//キャラクターの縦の索敵当たり判定(索敵範囲)

#define MAX_CHARACTER_ATK_COLLISION_WIDTH(Num)  ((m_tSize.X * (Num / 2)) + (m_tSize.X))		//キャラクターの横の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_HEIGHT(Num) ((m_tSize.Y * (Num / 2)) + (m_tSize.Y))		//キャラクターの縦の攻撃当たり判定(相手の人数)

#define PLAYER_SPLIT_X (4)	// プレイヤーテクスチャ分割数X
#define PLAYER_SPLIT_Y (4)	// プレイヤーテクスチャ分割数Y
#define ANIME_TIME (10)		// プレイヤーアニメーションカウント

enum AllyTexture
{
	Ally3,
	Ally4,
	Ally5,
	Ally6,
	Ally7,
	Ally8,
	MAX_AllyTex,
};
//enum AllyBufferTexture
//{
//	MAX_AllyBufferTex,
//};
enum EnemyTexture
{
	Enemy1,
	Enemy2,
	Enemy3,
	MAX_EnemyTex,
};

void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize);

Texture* g_pAllyTex[MAX_AllyTex];
//Texture* g_pAllyBufferTex[MAX_AllyBufferTex];
Texture* g_pEnemyTex[MAX_EnemyTex];

Texture* g_pLeaderTex[MAX_LEADERS];

Texture* g_pCollisionTex;

Texture* g_pHpGageTex[2];

Sprite* g_pSprite;

CFieldVertex* g_pFieldVtx;



void IninCharacterTexture(CFieldVertex* InAddress,int StageNum)	//テクスチャ読み込み
{
	g_pFieldVtx = InAddress;

	for(int i = 0; i < MAX_AllyTex;i++)
		g_pAllyTex[i] = new Texture();
	//for (int i = 0; i < MAX_AllyBufferTex; i++)
	//	g_pAllyBufferTex[i] = new Texture();
	for(int i = 0; i < MAX_EnemyTex;i++)
		g_pEnemyTex[i] = new Texture();

	g_pCollisionTex = new Texture();

	g_pLeaderTex[0] = new Texture();
	g_pLeaderTex[1] = new Texture();

	g_pHpGageTex[0] = new Texture();
	g_pHpGageTex[1] = new Texture();

	HRESULT hr;
	hr = g_pAllyTex[Ally3]->Create("Asset/味方/3.png");
	hr = g_pAllyTex[Ally4]->Create("Asset/味方/4.png");
	hr = g_pAllyTex[Ally5]->Create("Asset/味方/5.png");
	hr = g_pAllyTex[Ally6]->Create("Asset/味方/6.png");
	hr = g_pAllyTex[Ally7]->Create("Asset/味方/7.png");
	hr = g_pAllyTex[Ally8]->Create("Asset/味方/8.png");
	hr = g_pEnemyTex[Enemy1]->Create("Asset/敵/3.png");
	hr = g_pEnemyTex[Enemy2]->Create("Asset/敵/4.png");
	hr = g_pEnemyTex[Enemy3]->Create("Asset/敵/5.png");

	hr = g_pCollisionTex->Create("Asset/Star/CLStar.png");

	hr = g_pLeaderTex[0]->Create("Asset/Player/Lini.png");
	switch (StageNum)
	{
	case 0:
		g_pLeaderTex[1]->Create("Asset/敵/BossNo1.png");
		break;
	case 1:
		break;
	case 2:
		break;
	}

	g_pHpGageTex[0]->Create("Asset/HpGage/HpGageBase.png");
	g_pHpGageTex[1]->Create("Asset/HpGage/HpGage.png");

	
	
}

CFighter::CFighter(int InCornerCount, float InSize)
	:m_tStatus(St_Create)
	, m_tSearchCollision{ 0.0f,0.0f }
	, m_tAtkCollision{ 0.0f,0.0f }
	, nCornerCount(InCornerCount)
	, m_fHp(0.0f)
	//, m_fShield(0.0f)
	, m_fAtk(0.0f)
	//, m_tAtkType(AT_Physics)
	, m_fAtkCharge(0.0f)
	, m_fAtkChargeMax(0.0f)
	, m_fAtkAnimationTime(0.0f)
	, m_fAtkAnimationMaxTime(0.0f)
	//, m_bCreateInit(false)
	, m_bIsAttack(false)
	, m_bFirstBattlePosSetting(false)
	, m_nTargetNumber(-1)
	, m_bIsHit(false)
{
	m_tPos.X = 0.0f;
	m_tPos.Y = 0.0f;
	m_tPos.Z = 0.0f;
	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	
	
	m_pSourceAttack = GetSound(SE_ATTACK, false);
	m_Number = GetSoundNumber(SE_ATTACK);
	
	//m_pEffect = new CEffect("Asset/Player/Player.png", 4, 4);
}

CFighter::~CFighter()
{
	delete m_pHpGage;
	m_pHpGage = nullptr;

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

	//if (m_pEffect)
	//{
	//	delete m_pEffect;
	//	m_pEffect = nullptr;
	//}
	if (g_pFieldVtx)
	{
		g_pFieldVtx = nullptr;
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

	m_pSourceAttack->DestroyVoice();
}

void CFighter::CollisionDraw(void)
{
	/*当たり判定テスト*/
	for (int i = 0; i < 8; i++)
	{
		//テクスチャの指定
		g_pSprite->SetTexture(g_pCollisionTex);
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
		g_pSprite->Draw();

		g_pSprite->ReSetSprite();
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
	//switch (nCornerCount)
	//{
	//default:
	//case 3:
	//case 4:
	//	if (m_fShield > 0.0f)
	//		m_fShield -= pFighter->GetAtk();
	//	else
	//		m_fHp -= pFighter->GetAtk();
	//	break;
	//case 5:
	//	if (m_fShield > 0.0f)
	//	{
	//		if (pFighter->GetAtkType() == AT_Magic)
	//			m_fShield -= pFighter->GetAtk();
	//		else
	//			m_fShield -= pFighter->GetAtk() * 0.5f;
	//	}
	//	else
	//	{
	//		if (pFighter->GetAtkType() == AT_Magic)
	m_fHp -= pFighter->GetAtk();
	//		else
	//			m_fHp -= pFighter->GetAtk() * 0.5f;
	//	}
	//	break;
	//}
	m_bIsHit = true;
	//m_pEffect->Play();
}

CAlly::CAlly(int InCornerCount, float InSize)
	:CFighter(InCornerCount,InSize)
{
	SettingStatus();
	m_pHpGage = new CHpUI(m_fHp);
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

	//m_pEffect->Update();
}

void CAlly::Draw(void)
{

	//体力ゲージの描画
	m_pHpGage->Draw();

	//キャラクターの描画
	g_pSprite->SetTexture(g_pAllyTex[nCornerCount - 3]);

	if(m_bIsHit) g_pSprite->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	else g_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	g_pSprite->Draw();

	//m_bIsHit = false;

	
	//m_pEffect->SetEffectTexture();
	//m_pEffect->SetEffect3D({ m_tPos.X,m_tPos.Y,m_tPos.Z });
	//m_pEffect->Draw();

	g_pSprite->ReSetSprite();
}

void CAlly::CreateUpdate(void)
{
	//生成アニメーション

	//生成移動
	//m_tPos.Y -= 10.0f;
	//if (m_tPos.Y < -600.0f)
	//{
	//生成アニメーションが終わったら
	SetStatus(St_Battle);
	m_bFirstBattlePosSetting = false;
	//}
}

void CAlly::BattleUpdate(void)
{
	if (m_bIsAttack)
	{
		//攻撃音
		if (m_pSourceAttack)
		{
			m_pSourceAttack->SetVolume(0.7f);
			m_pSourceAttack->Start();
		}
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{

			//アニメーション処理

			m_fAtkAnimationTime++;
			if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
	}
	else
	{
		//m_pSourceAttack->Stop();
	}
	//戦闘アニメーション(攻撃範囲内に敵がいたら)
	
	//移動アニメーション(移動していたら)

	//待機アニメーション(移動していなかったら)

	//HPUIの更新処理
	m_pHpGage->Update(m_fHp, { m_tPos.X,m_tPos.Y,m_tPos.Z }, m_tSize.Y);
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
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
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
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Hexagon:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Heptagon:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Octagon:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}

CEnemy::CEnemy(int InCornerCount, float InSize)
	:CFighter(InCornerCount, InSize)
{
	SettingStatus();
	m_pHpGage = new CHpUI(m_fHp);

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
	//m_pSourceAttack = nullptr;
	
	//if (m_pVtx) m_pVtx->Release();
}

void CEnemy::Update(void)
{
	m_pHpGage->Update(m_fHp, { m_tPos.X,m_tPos.Y,m_tPos.Z }, m_tSize.Y);

	// 状況に応じて処理を分ける
	switch (m_tStatus)
	{
	case St_Create:	CreateUpdate();		break;
	case St_Battle:	BattleUpdate();		break;
	case St_Death:	DeathUpdate();		break;
	default:
		break;
	}

	//m_pEffect->Update();
}

void CEnemy::Draw(void)
{

	//体力ゲージの描画
	m_pHpGage->Draw();

	//キャラクターの描画
	g_pSprite->SetTexture(g_pEnemyTex[nCornerCount - 3]);

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	g_pSprite->Draw();

	//m_pEffect->SetEffectTexture();
	//m_pEffect->SetEffect3D({ m_tPos.X,m_tPos.Y,m_tPos.Z });
	//m_pEffect->Draw();

	g_pSprite->ReSetSprite();
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
	if (m_bIsAttack)
	{
		if (m_fAtkChargeMax == m_fAtkCharge + m_fAtkAnimationTime)
		{
			////攻撃音
			//m_pSourceAttack = GetSound(SE_ATTACK, false);
			m_pSourceAttack->SetVolume(0.7f);
			m_pSourceAttack->Start();
		}
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{

			//アニメーション処理

			m_fAtkAnimationTime++;
			if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
	}
	else
	{
		//m_pSourceAttack->Stop();
	}
	//戦闘アニメーション(攻撃範囲内に敵がいたら)
	
	//移動アニメーション(移動していたら)

	//待機アニメーション(移動していなかったら)

	//HPUIの更新処理
	m_pHpGage->Update(m_fHp, { m_tPos.X,m_tPos.Y,m_tPos.Z }, m_tSize.Y);
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
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 10.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
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
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}

//CAllyBuffer::CAllyBuffer(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress)
//	: m_tStatus(St_Create)
//	, nCornerCount((Corner)InCornerCount)
//	, m_tPos(FirstPos)
//	, IsBuff(false)
//	, m_pCamera(InAddress)
//{
//	SettingStatus();
//
//	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
//	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
//	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
//}
//
//CAllyBuffer::~CAllyBuffer()
//{
//	if (m_pSprite)
//	{
//		delete m_pSprite;
//		m_pSprite = nullptr;
//	}
//}
//
//void CAllyBuffer::Update(void)
//{
//	switch (m_tStatus)
//	{
//	case St_Create:CreateUpdate();break;
//	case St_Battle:BattleUpdate();break;
//	case St_Death:DeathUpdate();break;
//	}
//}
//
//void CAllyBuffer::Draw(void)
//{
//	m_pSprite->SetTexture(g_pAllyBufferTex[nCornerCount - 6]);
//
//	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });
//
//	m_pSprite->Draw();
//
//	m_pSprite->ReSetSprite();
//}
//
//void CAllyBuffer::CreateUpdate(void)
//{
//	//生成アニメーション
//
//	//生成アニメーションが終わったら
//	SetStatus(St_Battle);
//}
//
//void CAllyBuffer::BattleUpdate(void)
//{
//	//アニメーション(移動していなかったら)
//}
//
//void CAllyBuffer::DeathUpdate(void)
//{
//	//死亡アニメーション
//
//	//死亡アニメーションが終わったら
//	SetStatus(St_Delete);
//}
//
//void CAllyBuffer::SettingStatus(void)
//{
//	switch (nCornerCount)
//	{
//	default:
//		break;
//	case Hexagon:
//		m_tBuff = BT_Shield;
//		break;
//	case Heptagon:
//		m_tBuff = BT_Attack;
//		break;
//	case Octagon:
//		m_tBuff = BT_ReSummon;
//		break;
//	}
//}
//
//void CAllyBuffer::DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize)
//{
//	//移動行列(Translation)
//	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
//		InPos.x,
//		InPos.y,
//		InPos.z,
//		0.0f
//	));
//	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
//		AngleX,
//		AngleY,
//		AngleZ,
//		0.0f
//	));
//
//	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);
//	//それぞれの行列を掛け合わせて格納
//	DirectX::XMMATRIX mat = S * R * T;
//
//	DirectX::XMFLOAT4X4 wvp[3];
//	DirectX::XMMATRIX world;
//	world = mat;
//
//	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
//	wvp[1] = m_pCamera->GetViewMatrix();
//	wvp[2] = m_pCamera->GetProjectionMatrix();
//
//	m_pSprite->SetWorld(wvp[0]);
//	m_pSprite->SetView(wvp[1]);
//	m_pSprite->SetProjection(wvp[2]);
//}
//CEnemyBoss::CEnemyBoss(int BossTypeNum, float InSize, CVector3<float> FirstPos, Camera* InAddress)
//	:CFighter(BossTypeNum,InSize,FirstPos,InAddress)
//{
//	SettingStatus();
//}
//
//CEnemyBoss::~CEnemyBoss()
//{
//
//}
//
//void CEnemyBoss::Update(void)
//{
//	switch (m_tStatus)
//	{
//	case St_Create:CreateUpdate();
//		break;
//	case St_Battle:BattleUpdate();
//		break;
//	case St_Death:DeathUpdate();
//		break;
//	}
//}
//
//void CEnemyBoss::Draw(void)
//{
//	m_pSprite->SetTexture(g_pEnemyBossTex);
//
//	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });
//
//	m_pSprite->Draw();
//
//	m_pSprite->ReSetSprite();
//}
//
//void CEnemyBoss::CreateUpdate(void)
//{
//	//生成アニメーション
//
//	//生成アニメーションが終わったら
//	SetStatus(St_Battle);
//
//}
//
//void CEnemyBoss::BattleUpdate(void)
//{
//	//戦闘アニメーション(攻撃範囲内に敵がいたら)
//	if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
//	{
//		//アニメーション処理
//
//		m_fAtkAnimationTime++;
//		if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
//	}
//	//移動アニメーション(移動していたら)
//
//	//待機アニメーション(移動していなかったら)
//}
//
//void CEnemyBoss::DeathUpdate(void)
//{
//	//死亡アニメーション
//
//	//死亡アニメーションが終わったら
//	SetStatus(St_Delete);
//}
//
//void CEnemyBoss::SettingStatus(void)
//{
//	switch (nCornerCount)
//	{
//	default:
//		break;
//	case Type1:
//		m_fHp = 100.0f;
//		m_fAtk = 5.0f;
//		m_tAtkType = AT_Physics;
//		m_fAtkAnimationMaxTime = 0.0f;
//		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
//		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(3);			//キャラクターの中心からの横の当たり判定
//		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(3);			//キャラクターの中心からの縦の当たり判定
//		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);	//キャラクターの中心からの横の索敵当たり判定
//		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
//		break;
//	case Type2:
//		break;
//	case Type3:
//		break;
//	}
//
//}
//
CLeader::CLeader(float InSize, CVector3<float>FirstPos, int InTextureNumber)
	:m_tStatus(St_Create)
	,m_tPos(FirstPos)
	,m_fHp(100.0f)
	,m_nTextureNumber(InTextureNumber)
	, m_pHpGage(nullptr)
{
	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする

	m_pHpGage = new CHpUI(m_fHp);
}

CLeader::~CLeader()
{
	delete m_pHpGage;
	m_pHpGage = nullptr;
}

void CLeader::Update(void)
{
	m_pHpGage->Update(m_fHp,{m_tPos.X,m_tPos.Y,m_tPos.Z},m_tSize.Y);

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

void CLeader::Draw()
{
	//体力ゲージの描画
	m_pHpGage->Draw();

	g_pSprite->SetTexture(g_pLeaderTex[m_nTextureNumber]);

	g_pSprite->SetUVPos({ 0.0f,0.0f });
	g_pSprite->SetUVScale({ 1.0f,1.0f });

	DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z });

	g_pSprite->Draw();
	
	g_pSprite->ReSetSprite();
}

void CLeader::Damage(CFighter* pFighter)
{
	m_fHp -= pFighter->GetAtk();
}

void CLeader::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(St_Battle);

}

void CLeader::BattleUpdate(void)
{
	//待機アニメーション(移動していなかったら)
	
	//HPUIの更新処理
	m_pHpGage->Update(m_fHp, { m_tPos.X,m_tPos.Y,m_tPos.Z }, m_tSize.Y);
}

void CLeader::DeathUpdate(void)
{
	//死亡アニメーション

	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
}

void HPGageUIUpdate(void)
{

}

void HPGageUIDraw(void)
{

}

CHpUI::CHpUI(float FullHp)
	:m_fFullHp(FullHp)
	,m_tUIPos()
	,m_tUIScale()
{
	m_tUIScale.y = 0.8f;
	m_tUIScale.z = 1.0f;
}

CHpUI::~CHpUI()
{
}

void CHpUI::Update(float InHp,DirectX::XMFLOAT3 InPos, float InSizeY)
{
	int HpRatio = 0;

	m_tUIPos.x = InPos.x;
	m_tUIPos.y = InPos.y + InSizeY + 1.0f;
	m_tUIPos.z = InPos.z;

	HpRatio = (InHp / m_fFullHp) * 10.0f;

	m_tUIScale.x = HpRatio;
}

void CHpUI::Draw(void)
{
	//ベースの描画
	g_pSprite->SetTexture(g_pHpGageTex[0]);

	g_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	DrawSetting({ m_tUIPos.x,m_tUIPos.y,m_tUIPos.z + 0.1f }, { 10.0f,1.0f,1.0f });

	g_pSprite->Draw();

	g_pSprite->ReSetSprite();

	//ゲージの描画
	g_pSprite->SetTexture(g_pHpGageTex[1]);

	g_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	DrawSetting({ m_tUIPos.x - (5 -(m_tUIScale.x / 2)),m_tUIPos.y,m_tUIPos.z }, m_tUIScale);

	g_pSprite->Draw();

	g_pSprite->ReSetSprite();
}

void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize)
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		InPos.x,
		InPos.y,
		InPos.z,
		0.0f
	));

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		0.0f,
		0.0f
	));

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	g_pSprite->SetWorld(wvp[0]);
	g_pSprite->SetView(wvp[1]);
	g_pSprite->SetProjection(wvp[2]);
}
