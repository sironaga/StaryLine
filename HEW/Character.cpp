#include "Character.h"
#include "DirectXTex/TextureLoad.h"
#include "FieldVertex.h"
#include"SpriteDrawer.h"

#define MAX_CHARACTER_SEARCH_COLLISION_WIDTH(Num)  ((Num / 2) + (m_tSize.X))	//キャラクターの横の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(Num) ((Num / 2) + (m_tSize.Y))	//キャラクターの縦の索敵当たり判定(索敵範囲)

#define MAX_CHARACTER_ATK_COLLISION_WIDTH(Num)  ((m_tSize.X * (Num / 2)) + (m_tSize.X))		//キャラクターの横の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_HEIGHT(Num) ((m_tSize.Y * (Num / 2)) + (m_tSize.Y))		//キャラクターの縦の攻撃当たり判定(相手の人数)

#define NORMAL_SIZE (200)

CFieldVertex* g_pFieldVtx;
ID3D11ShaderResourceView* g_pAllyTex[6];	//味方画像のテクスチャ
ID3D11ShaderResourceView* g_pEnemyTex[6];	//敵画像のテクスチャ
ID3D11ShaderResourceView* g_pCollisionTex;

void IninCharacterTexture(CFieldVertex* InAddress)	//テクスチャ読み込み
{
	g_pFieldVtx = InAddress;
	HRESULT hr;

	hr = LoadTextureFromFile(GetDevice(), "Asset/味方/3.png", &g_pAllyTex[0]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/味方/4.png", &g_pAllyTex[1]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/味方/5.png", &g_pAllyTex[2]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/味方/6.png", &g_pAllyTex[3]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/味方/7.png", &g_pAllyTex[4]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/味方/8.png", &g_pAllyTex[5]);

	hr = LoadTextureFromFile(GetDevice(), "Asset/敵/3.png", &g_pEnemyTex[0]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/敵/4.png", &g_pEnemyTex[1]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/敵/5.png", &g_pEnemyTex[2]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/敵/6.png", &g_pEnemyTex[3]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/敵/7.png", &g_pEnemyTex[4]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/敵/8.png", &g_pEnemyTex[5]);

	/*当たり判定テスト用*/
	hr = LoadTextureFromFile(GetDevice(), "Asset/Star/image_(2-1).png", &g_pCollisionTex);

	if (FAILED(hr)) {
		MessageBox(NULL, "Character 画像", "Error", MB_OK);
	}

}

CFighter::CFighter(int InCornerCount, float InSize, CVector3<float> FirstPos)
	:m_tStatus(Create)
	, m_tSearchCollision{ 0.0f,0.0f }
	, m_tAtkCollision{ 0.0f,0.0f }
	, m_tPos(FirstPos)
	, nCornerCount(InCornerCount)
	, m_fHp(0.0f)
	, m_fPhysicsAtk(0.0f)
	, m_fMagicAtk(0.0f)
	, m_fAtkCharge(0.0f)
	, m_fAtkChargeMax(0.0f)
	, m_bCreateInit(false)
	, m_bIsAttack(false)
	, m_pVtx(nullptr)
	, m_nTargetNumber(-1)
{
	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする

	if (nCornerCount > 8)//角数が8を超えていたら
	{
		//switch (m_tPolygon.nCornerCount)//追加角数別に追加要素を設定する
		//{
		//case 9:
		//	break;
		//case 10:
		//	break;
		//case 11:
		//	break;
		//case 12:
		//	break;
		//case 13:
		//	break;
		//case 14:
		//	break;
		//case 15:
		//	break;
		//case 16:
		//	break;
		//case 17:
		//	break;
		//case 18:
		//	break;
		//case 19:
		//	break;
		//case 20:
		//	break;
		//case 21:
		//	break;
		//case 22:
		//	break;
		//case 23:
		//	break;
		//case 24:
		//	break;
		//}
		nCornerCount = 8;//角数を8に設定
	}
}

CFighter::~CFighter()
{
	if (g_pFieldVtx)
	{
		delete g_pFieldVtx;
		g_pFieldVtx = nullptr;
	}
	for (int i = 0; i < 6; i++)
	{
		delete g_pAllyTex[i];
		g_pAllyTex[i] = nullptr;
		delete g_pEnemyTex[i];
		g_pEnemyTex[i] = nullptr;
	}
}

void CFighter::CollisionDraw(void)
{
	/*当たり判定テスト*/
//テクスチャの指定
	SetSpriteTexture(g_pCollisionTex);

	//スプライトの設定
	SetSpritePos(m_tPos.X - m_tAtkCollision.Width, m_tPos.Y - m_tAtkCollision.Height);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//描画
	DrawSprite(m_pVtx, sizeof(Vertex));

	//設定のリセット
	ReSetSprite();

	//テクスチャの指定
	SetSpriteTexture(g_pCollisionTex);

	//スプライトの設定
	SetSpritePos(m_tPos.X + m_tAtkCollision.Width, m_tPos.Y - m_tAtkCollision.Height);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//描画
	DrawSprite(m_pVtx, sizeof(Vertex));

	//設定のリセット
	ReSetSprite();

	//テクスチャの指定
	SetSpriteTexture(g_pCollisionTex);

	//スプライトの設定
	SetSpritePos(m_tPos.X - m_tAtkCollision.Width, m_tPos.Y + m_tAtkCollision.Height);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//描画
	DrawSprite(m_pVtx, sizeof(Vertex));

	//設定のリセット
	ReSetSprite();

	//テクスチャの指定
	SetSpriteTexture(g_pCollisionTex);

	//スプライトの設定
	SetSpritePos(m_tPos.X + m_tAtkCollision.Width, m_tPos.Y + m_tAtkCollision.Height);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//描画
	DrawSprite(m_pVtx, sizeof(Vertex));

	//設定のリセット
	ReSetSprite();

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
			return true;
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
			return true;
		}
	}
	return false;
}

void CFighter::Damage(float InDamage)
{
	m_fHp -= InDamage;
}

CAlly::CAlly(int InCornerCount, float InSize, CVector3<float> FirstPos)
	:CFighter(InCornerCount,InSize, FirstPos)
{
	SettingStatus();
	Vertex vtx[] = {
		//背景表示の座標
		{{-m_tSize.X / 2, -m_tSize.Y / 2,0.0f}, {0.0f, 0.0f}},
		{{-m_tSize.X / 2,  m_tSize.Y / 2,0.0f}, {0.0f, 1.0f}},
		{{ m_tSize.X / 2, -m_tSize.Y / 2,0.0f}, {1.0f, 0.0f}},
		{{ m_tSize.X / 2,  m_tSize.Y / 2,0.0f}, {1.0f, 1.0f}},
	};
	m_pVtx = CreateVertexBuffer(vtx, 4);
}

CAlly::~CAlly()
{
	if (m_pVtx) m_pVtx->Release();
}

void CAlly::Update(void)
{
	// 状況に応じて処理を分ける
	switch (m_tStatus)
	{
	case CFighter::Create:	CreateUpdate();		break;
	case CFighter::Battle:	BattleUpdate();		break;
	case CFighter::Death:	DeathUpdate();		break;
	default:
		break;
	}
}

void CAlly::Draw(void)
{
	//テクスチャの指定
	SetSpriteTexture(g_pAllyTex[nCornerCount - 3]);

	//スプライトの設定
	SetSpritePos(m_tPos.X, m_tPos.Y);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//描画
	DrawSprite(m_pVtx, sizeof(Vertex));

	//設定のリセット
	ReSetSprite();
}

void CAlly::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(Battle);
}

void CAlly::BattleUpdate(void)
{
	//戦闘アニメーション(攻撃範囲内に敵がいたら)

	//移動アニメーション(移動していたら)

	//待機アニメーション(移動していなかったら)
}

void CAlly::DeathUpdate(void)
{
	//死亡アニメーション
	// 
	//死亡アニメーションが終わったら
	SetStatus(Delete);
}

void CAlly::SettingStatus(void)
{
	switch (nCornerCount)
	{
	default:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 0.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 0.0f;
		m_fAtkChargeMax = 20.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(2);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(2);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Pentagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 0.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(2);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(2);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Hexagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Heptagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Octagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}

CEnemy::CEnemy(int InCornerCount, float InSize, CVector3<float> FirstPos)
	:CFighter(InCornerCount, InSize, FirstPos)
{
	SettingStatus();
	Vertex vtx[] = {
		//背景表示の座標
		{{-m_tSize.X / 2, -m_tSize.Y / 2, 0.0f}, {0.0f, 0.0f}},
		{{-m_tSize.X / 2,  m_tSize.Y / 2, 0.0f}, {0.0f, 1.0f}},
		{{ m_tSize.X / 2, -m_tSize.Y / 2, 0.0f}, {1.0f, 0.0f}},
		{{ m_tSize.X / 2,  m_tSize.Y / 2, 0.0f}, {1.0f, 1.0f}},
	};
	m_pVtx = CreateVertexBuffer(vtx, 4);
}

CEnemy::~CEnemy()
{
	if (m_pVtx) m_pVtx->Release();
}

void CEnemy::Update(void)
{
	// 状況に応じて処理を分ける
	switch (m_tStatus)
	{
	case CFighter::Create:	CreateUpdate();		break;
	case CFighter::Battle:	BattleUpdate();		break;
	case CFighter::Death:	DeathUpdate();		break;
	default:
		break;
	}
}

void CEnemy::Draw(void)
{
	//テクスチャの指定
	SetSpriteTexture(g_pEnemyTex[nCornerCount - 3]);

	//スプライトの設定
	SetSpritePos(m_tPos.X, m_tPos.Y);

	//大きさの設定
	SetSpriteScale(1.0f, 1.0f);

	//背景色の設定
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

	//描画
	DrawSprite(m_pVtx, sizeof(Vertex));

	//設定のリセット
	ReSetSprite();
}

void CEnemy::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(Battle);
}

void CEnemy::BattleUpdate(void)
{
	//戦闘アニメーション(攻撃範囲内に敵がいたら)

	//移動アニメーション(移動していたら)

	//待機アニメーション(移動していなかったら)
}

void CEnemy::DeathUpdate(void)
{
	//死亡アニメーション
	
	//死亡アニメーションが終わったら
	SetStatus(Delete);
	
}

void CEnemy::SettingStatus(void)
{
	switch (nCornerCount)
	{
	default:
		m_fHp = 0.0f;
		m_fPhysicsAtk = 0.0f;
		m_fMagicAtk = 0.0f;
		m_fAtkChargeMax = 0.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(0);		//キャラクターの中心からの横の当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(0);		//キャラクターの中心からの縦の当たり判定
		break;
	case Triangle:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Pentagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Hexagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Heptagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Octagon:
		m_fHp = 10.0f;
		m_fPhysicsAtk = 1.0f;
		m_fMagicAtk = 1.0f;
		m_fAtkChargeMax = 10.0f;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(100);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(100);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}