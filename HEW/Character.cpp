#include "Character.h"
#include "DirectXTex/TextureLoad.h"
#include "FieldVertex.h"
#include "Main.h"
#include "Geometory.h"
#include "ShaderList.h"

#define MAX_CHARACTER_SEARCH_COLLISION_WIDTH(Num)  ((Num / 2) + (m_tSize.X))	//キャラクターの横の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(Num) ((Num / 2) + (m_tSize.Y))	//キャラクターの縦の索敵当たり判定(索敵範囲)

#define MAX_CHARACTER_ATK_COLLISION_WIDTH(Num)  ((m_tSize.X * (Num / 2)) + (m_tSize.X))		//キャラクターの横の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_HEIGHT(Num) ((m_tSize.Y * (Num / 2)) + (m_tSize.Y))		//キャラクターの縦の攻撃当たり判定(相手の人数)

enum AllyTexture
{
	Ally3,
	Ally4,
	//Ally5,
	//Ally6,
	//Ally7,
	//Ally8,
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
	//Enemy3,
	MAX_EnemyTex,
};

enum class CharactersEffect
{
	FighterAttack,
	MAX,
};

void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize, Sprite* Sprite);

//Texture* g_pAllyTex[MAX_AllyTex];
Model* g_pAllyModel[MAX_AllyTex];

//Texture* g_pAllyBufferTex[MAX_AllyBufferTex];
//Texture* g_pEnemyTex[MAX_EnemyTex];
Model* g_pEnemyModel[MAX_EnemyTex];

Texture* g_pPlayerTex[CLeader::MAX_StatusMode];
Texture* g_pBosTex;

Model* g_pBosCar;

//Texture* g_pCollisionTex;

Texture* g_pHpGageTex[2][3];

//Sprite* g_pSprite;

CFieldVertex* g_pFieldVtx;

CSoundList* g_AttackSound;

CEffectManager* g_pCharacterEffects[(int)CharactersEffect::MAX];

void IninCharacterTexture(CFieldVertex* InAddress,int StageNum)	//テクスチャ読み込み
{
	g_pFieldVtx = InAddress;

	for (int i = 0; i < MAX_AllyTex; i++)
		g_pAllyModel[i] = new Model();

		//g_pAllyTex[i] = new Texture();
	//for (int i = 0; i < MAX_AllyBufferTex; i++)
	//	g_pAllyBufferTex[i] = new Texture();
	for (int i = 0; i < MAX_EnemyTex; i++)
		g_pEnemyModel[i] = new Model();
		//g_pEnemyTex[i] = new Texture();

	//g_pCollisionTex = new Texture();

	g_pPlayerTex[CLeader::WandOn] = new Texture();
	g_pPlayerTex[CLeader::WandOff] = new Texture();

	g_pBosTex = new Texture();

	g_pBosCar = new Model();

	g_pHpGageTex[0][0] = new Texture();
	g_pHpGageTex[0][1] = new Texture();
	g_pHpGageTex[0][2] = new Texture();
	g_pHpGageTex[1][0] = new Texture();
	g_pHpGageTex[1][1] = new Texture();
	g_pHpGageTex[1][2] = new Texture();

	HRESULT hr;
	//hr = g_pAllyTex[Ally3]->Create("Asset/味方/SankakuSD.png");
	//hr = g_pAllyTex[Ally4]->Create("Asset/味方/ShikakuSD.png");
	g_pAllyModel[Ally3]->Load("Assets/Model/Character/Triangle/Triangle.fbx", 0.05f, Model::XFlip);
	g_pAllyModel[Ally4]->Load("Assets/Model/Character/Square/Square.fbx", 0.05f, Model::XFlip);
	//hr = g_pAllyTex[Ally5]->Create("Asset/味方/5.png");
	//hr = g_pAllyTex[Ally6]->Create("Asset/味方/6.png");
	//hr = g_pAllyTex[Ally7]->Create("Asset/味方/7.png");
	//hr = g_pAllyTex[Ally8]->Create("Asset/味方/8.png");
	//hr = g_pEnemyTex[Enemy1]->Create("Asset/敵/zako1SD_Left.png");
	//hr = g_pEnemyTex[Enemy2]->Create("Asset/敵/4.png");
	g_pEnemyModel[Enemy1]->Load("Assets/Model/Character/Square/Square.fbx", 0.05f, Model::XFlip);
	g_pEnemyModel[Enemy2]->Load("Assets/Model/Character/Triangle/Triangle.fbx", 0.05f, Model::XFlip);
	//hr = g_pEnemyTex[Enemy3]->Create("Asset/敵/5.png");

	//hr = g_pCollisionTex->Create("Asset/Star/CLStar.png");

	hr = g_pPlayerTex[CLeader::WandOn]->Create("Asset/Player/LinieSD_WandOn_sprite.png");
	hr = g_pPlayerTex[CLeader::WandOff]->Create("Asset/Player/LinieSD_WandOff_sprite.png");

	switch (StageNum)
	{
	case 0:
		g_pBosTex->Create("Asset/敵/BossNo1.png");
		g_pBosCar->Load("Assets/Model/Boss01_Car.fbx", 1.0f, Model::XFlip);
		break;
	case 1:
		break;
	case 2:
		break;
	}

	g_pHpGageTex[0][0]->Create("Asset/HpGage/UI_HP_Gage_Linie.png");
	g_pHpGageTex[0][1]->Create("Asset/HpGage/UI_HP_top_Linie.png");
	g_pHpGageTex[0][2]->Create("Asset/HpGage/UI_HP_under_Linie.png");

	g_pHpGageTex[1][0]->Create("Asset/HpGage/UI_HP_Gage_Nugar.png");
	g_pHpGageTex[1][1]->Create("Asset/HpGage/UI_HP_top_Nugar.png");
	g_pHpGageTex[1][2]->Create("Asset/HpGage/UI_HP_under_Nugar.png");

	g_AttackSound = new CSoundList(SE_ATTACK);
	
	g_pCharacterEffects[(int)CharactersEffect::FighterAttack] = new CEffectManager(TEX_PASS("Effect/Fire.efk"));
}
void UnIninCharacterTexture()
{
	for (int i = 0; i < MAX_AllyTex; i++)
	{
		//if (g_pAllyTex)
		//{
		//	delete g_pAllyTex[i];
		//	g_pAllyTex[i] = nullptr;
		//}
		if (g_pAllyModel[i])
		{
			delete g_pAllyModel[i];
			g_pAllyModel[i] = nullptr;
		}
	}
	for (int i = 0; i < MAX_EnemyTex; i++)
	{
		if (g_pEnemyModel[i])
		{
			delete g_pEnemyModel[i];
			g_pEnemyModel[i] = nullptr;
		}
	}
	for (int i= 0; i < (int)CharactersEffect::MAX; i++)
	{
		delete g_pCharacterEffects[i];
		g_pCharacterEffects[i] = nullptr;
	}
	//if (g_pCollisionTex)
	//{
	//	delete g_pCollisionTex;
	//	g_pCollisionTex = nullptr;
	//}
	//for (int i = 0; i < 3; i++)
	//{
	//	delete g_pAllyTex[i];
	//	g_pAllyTex[i] = nullptr;
	//}
	//for (int i = 0; i < 3; i++)
	//{
	//	delete g_pEnemyTex[i];
	//	g_pEnemyTex[i] = nullptr;
	//}
}
void UnInitSound()
{
	if (g_AttackSound)
	{
		delete g_AttackSound;
		g_AttackSound = nullptr;
	}
}

CFighter::CFighter(int InCornerCount)
	:m_tStatus(St_Create)
	, m_tSearchCollision{ 0.0f,0.0f }
	, m_tAtkCollision{ 0.0f,0.0f }
	, m_tOldPos()
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
	, m_fTimeSound(0)
	, m_bTimeSoundStart(false)
	, m_MoveFlag(false)
	, m_pEffect{}
{
	//m_pSprite = new Sprite();


	switch (InCornerCount)
	{
	case 3:
	case 5:
	case 7:
		nCornerCount = 3;
		break;
	case 4:
	case 6:
	case 8:
		nCornerCount = 4;
		break;
	}
	m_tPos.X = 0.0f;
	m_tPos.Y = 0.0f;
	m_tPos.Z = 0.0f;
	m_tSize.X = NORMAL_SIZE;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE;	//面積分サイズを大きくする
	m_pSourceAttack = g_AttackSound->m_sound->CreateSourceVoice(m_pSourceAttack);
	XAUDIO2_BUFFER buffer = g_AttackSound->GetBuffer(false);
	m_pSourceAttack->SubmitSourceBuffer(&buffer);
	//m_pEffect = new CEffect("Asset/Player/Player.png", 4, 4);
}

CFighter::~CFighter()
{
	delete m_pHpGage;
	m_pHpGage = nullptr;

	
	for (int i = 0; i < (int)CharactersEffect::MAX; i++)
	{
		if (m_pEffect[i])
		{
			m_pEffect[i] = nullptr;
		}
	}
	if (g_pFieldVtx)
	{
		g_pFieldVtx = nullptr;
	}
	

	
	if (m_pSourceAttack)
	{
		m_pSourceAttack->ExitLoop();
		m_pSourceAttack->Stop();
		m_pSourceAttack->DestroyVoice();
		m_pSourceAttack = nullptr;
	}
	
}

//void CFighter::CollisionDraw(void)
//{
//	CVector3<float> Size;
//	Size.X = m_tSize.X * 0.2f;
//	Size.Y = m_tSize.Y * 0.2f;
//	Size.Z = m_tSize.Z * 0.2f;
//
//	
//	/*当たり判定テスト*/
//	for (int i = 0; i < 8; i++)
//	{
//		//テクスチャの指定
//		m_pSprite->SetTexture(g_pCollisionTex);
//		switch (i)
//		{
//		case 0:	DrawSetting({
//			m_tPos.X - Size.X,
//			m_tPos.Y - Size.Y,
//			m_tPos.Z - Size.Z,
//			//m_tPos.X - m_tAtkCollision.Width,
//			//m_tPos.Y - m_tAtkCollision.Height,	
//			//m_tPos.Z - m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f },m_pSprite);
//			break;
//		case 1:	DrawSetting({ 
//			m_tPos.X + Size.X,
//			m_tPos.Y - Size.Y,
//			m_tPos.Z - Size.Z,
//			//m_tPos.X + m_tAtkCollision.Width,
//			//m_tPos.Y - m_tAtkCollision.Height,	
//			//m_tPos.Z - m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f }, m_pSprite);
//			break;
//		case 2:	DrawSetting({ 
//			m_tPos.X - Size.X,
//			m_tPos.Y + Size.Y,
//			m_tPos.Z - Size.Z,
//			//m_tPos.X - m_tAtkCollision.Width,
//			//m_tPos.Y + m_tAtkCollision.Height,	
//			//m_tPos.Z - m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f }, m_pSprite);
//			break;
//		case 3:	DrawSetting({ 
//			m_tPos.X + Size.X,
//			m_tPos.Y + Size.Y,
//			m_tPos.Z - Size.Z,
//			//m_tPos.X + m_tAtkCollision.Width,
//			//m_tPos.Y + m_tAtkCollision.Height,	
//			//m_tPos.Z - m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f }, m_pSprite);
//			break;
//		case 4:	DrawSetting({ 
//			m_tPos.X - Size.X,
//			m_tPos.Y - Size.Y,
//			m_tPos.Z + Size.Z,
//			//m_tPos.X - m_tAtkCollision.Width,
//			//m_tPos.Y - m_tAtkCollision.Height,	
//			//m_tPos.Z + m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f }, m_pSprite);
//			break;
//		case 5:	DrawSetting({ 
//			m_tPos.X + Size.X,
//			m_tPos.Y - Size.Y,
//			m_tPos.Z + Size.Z,
//			//m_tPos.X + m_tAtkCollision.Width,
//			//m_tPos.Y - m_tAtkCollision.Height,	
//			//m_tPos.Z + m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f }, m_pSprite);
//			break;
//		case 6:	DrawSetting({ 
//			m_tPos.X - Size.X,
//			m_tPos.Y + Size.Y,
//			m_tPos.Z + Size.Z,
//			//m_tPos.X - m_tAtkCollision.Width,
//			//m_tPos.Y + m_tAtkCollision.Height,	
//			//m_tPos.Z + m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f }, m_pSprite);
//			break;
//		case 7:	DrawSetting({ 
//			m_tPos.X + Size.X,
//			m_tPos.Y + Size.Y,
//			m_tPos.Z + Size.Z,
//			//m_tPos.X + m_tAtkCollision.Width,
//			//m_tPos.Y + m_tAtkCollision.Height,	
//			//m_tPos.Z + m_tAtkCollision.Width, 
//			},
//			{ 3.0f,3.0f,3.0f }, m_pSprite);
//			break;
//		}
//		m_pSprite->Draw();
//
//		m_pSprite->ReSetSprite();
//	}
//}

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
			m_tPos.Y + m_tAtkCollision.Height >= InPos.Y - (InSize.Y / 2)	/*自分の攻撃当たり判定の下端 >= 相手の下端*/
			||
			m_tPos.Y - m_tAtkCollision.Height <= InPos.Y + (InSize.Y / 2)	/*自分の攻撃当たり判定の上端 <= 相手の上端*/
			&&
			m_tPos.Y + m_tAtkCollision.Height >= InPos.Y + (InSize.Y / 2)	/*自分の攻撃当たり判定の下端 >= 相手の上端*/)
		{
			if (m_tPos.Z - m_tAtkCollision.Width <= InPos.Z - (InSize.Z / 2)	/*自分の攻撃当たり判定の前端 <= 相手の前端*/
				&&
				m_tPos.Z + m_tAtkCollision.Width >= InPos.Z - (InSize.Z / 2)	/*自分の攻撃当たり判定の奥端 >= 相手の前端*/
				||
				m_tPos.Z - m_tAtkCollision.Width <= InPos.Z + (InSize.Z / 2)	/*自分の攻撃当たり判定の前端 <= 相手の奥端*/
				&&
				m_tPos.Z + m_tAtkCollision.Width >= InPos.Z + (InSize.Z / 2)	/*自分の攻撃当たり判定の奥端 >= 相手の奥端*/)
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
		if (m_tPos.Y - m_tSearchCollision.Height <= InPos.Y - InSize.Y	/*自分の索敵当たり判定の上端 <= 相手の上端*/
			&&
			m_tPos.Y + m_tSearchCollision.Height >= InPos.Y - InSize.Y	/*自分の索敵当たり判定の下端 >= 相手の上端*/
			||
			m_tPos.Y - m_tSearchCollision.Height <= InPos.Y + InSize.Y	/*自分の索敵当たり判定の上端 <= 相手の下端*/
			&&
			m_tPos.Y + m_tSearchCollision.Height >= InPos.Y + InSize.Y	/*自分の索敵当たり判定の下端 >= 相手の下端*/)
		{
			if (m_tPos.Z - m_tSearchCollision.Width <= InPos.Z - InSize.Z	/*自分の索敵当たり判定の前端 <= 相手の前端*/
				&&
				m_tPos.Z + m_tSearchCollision.Width >= InPos.Z - InSize.Z	/*自分の索敵当たり判定の奥端 >= 相手の前端*/
				||
				m_tPos.Z - m_tSearchCollision.Width <= InPos.Z + InSize.Z	/*自分の索敵当たり判定の前端 <= 相手の奥端*/
				&&
				m_tPos.Z + m_tSearchCollision.Width >= InPos.Z + InSize.Z	/*自分の索敵当たり判定の奥端 >= 相手の奥端*/)
			{
				return true;
			}
		}
	}
	return false;
}

bool CFighter::OverlapCheck(CVector3<float> InPos, CVector3<float> InSize)
{
	//サイズを重なりように調整
	CVector3<float> Size;
	Size.X = m_tSize.X * 0.2f;
	Size.Y = m_tSize.Y * 0.2f;
	Size.Z = m_tSize.Z * 0.2f;

	InSize.X = InSize.X * 0.2f;
	InSize.Y = InSize.Y * 0.2f;
	InSize.Z = InSize.Z * 0.2f;

	if (m_tPos.X - Size.X <= InPos.X - InSize.X	/*自分の左端 <= 相手の左端*/
		&&
		m_tPos.X + Size.X >= InPos.X - InSize.X	/*自分の右端 >= 相手の左端*/
		||
		m_tPos.X - Size.X <= InPos.X + InSize.X	/*自分の左端 <= 相手の右端*/
		&&
		m_tPos.X + Size.X >= InPos.X + InSize.X	/*自分の右端 >= 相手の右端*/)
	{
		if (m_tPos.Y - Size.Y <= InPos.Y - InSize.Y	/*自分の上端 <= 相手の上端*/
			&&
			m_tPos.Y + Size.Y >= InPos.Y - InSize.Y	/*自分の下端 >= 相手の上端*/
			||
			m_tPos.Y - Size.Y <= InPos.Y + InSize.Y	/*自分の上端 <= 相手の下端*/
			&&
			m_tPos.Y + Size.Y >= InPos.Y + InSize.Y	/*自分の下端 >= 相手の下端*/)
		{
			if (m_tPos.Z - Size.X <= InPos.Z - InSize.Z	/*自分の前端 <= 相手の前端*/
				&&
				m_tPos.Z + Size.X >= InPos.Z - InSize.Z	/*自分の奥端 >= 相手の前端*/
				||
				m_tPos.Z - Size.X <= InPos.Z + InSize.Z	/*自分の前端 <= 相手の奥端*/
				&&
				m_tPos.Z + Size.X >= InPos.Z + InSize.Z	/*自分の奥端 >= 相手の奥端*/)
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


CAlly::CAlly(int InCornerCount)
	:CFighter(InCornerCount)
{
	SettingStatus();
	m_pHpGage = new CHpUI(m_fHp,CHpUI::Ally);
	switch (nCornerCount)
	{
	case 3:
		m_pModel = g_pAllyModel[Ally3];
		break;
	case 4:
		m_pModel = g_pAllyModel[Ally4];
		break;
	}
	m_pEffect[(int)FighterEffect::Attack] = g_pCharacterEffects[(int)CharactersEffect::FighterAttack];
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

	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if(m_pEffect[i])
		m_pEffect[i]->Update();
	}
}

void CAlly::Draw(void)
{

	//体力ゲージの描画
	m_pHpGage->Draw();

	//キャラクターの描画
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.X, m_tPos.Y, m_tPos.Z, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.X, m_tSize.Y, m_tSize.Z);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(140.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
	{

		Model::Mesh mesh = *m_pModel->GetMesh(i);

		Model::Material material = *m_pModel->GetMaterial(mesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		if (m_pModel) {
			m_pModel->Draw(i);
		}
	}
	////キャラクターの描画
	//m_pSprite->SetTexture(g_pAllyTex[nCornerCount - 3]);

	//if(m_bIsHit) m_pSprite->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	//else m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	//m_pSprite->SetUVScale({ 1.0f, 1.0f });

	//DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z }, m_pSprite);

	//m_pSprite->Draw();

	////m_bIsHit = false;

	//
	////m_pEffect->SetEffectTexture();
	////m_pEffect->SetEffect3D({ m_tPos.X,m_tPos.Y,m_tPos.Z });
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
		m_pEffect[i]->Draw();
	}
	//m_pSprite->ReSetSprite();
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
		//攻撃エフェクト
		m_pEffect[(int)FighterEffect::Attack]->Play({ m_tPos.X, m_tPos.Y, m_tPos.Z - m_tSize.Z/2}, 60);
		//攻撃音
		if (!m_bTimeSoundStart)
		{
			XAUDIO2_BUFFER buffer = g_AttackSound->GetBuffer(false);
			m_pSourceAttack->SubmitSourceBuffer(&buffer);
			m_pSourceAttack->SetVolume(0.7f);
			m_pSourceAttack->Start();
			m_bTimeSoundStart = true;
		}

		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{

			//アニメーション処理

			m_fAtkAnimationTime++;
			if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
	}
	//攻撃音の再生時間
	if (m_bTimeSoundStart)
	{
		m_fTimeSound++;
	}
	if (m_fTimeSound>30)
	{
		m_pSourceAttack->Stop();
		m_bTimeSoundStart = false;
		m_fTimeSound = 0;
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
		m_fHp = 50.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 50.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 50.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	//case Pentagon:
	//	m_fHp = 10.0f;
	//	m_fAtk = 1.0f;
	//	//m_tAtkType = AT_Physics;
	//	m_fAtkAnimationMaxTime = 0.0f;
	//	m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
	//	m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
	//	m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
	//	m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
	//	m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
	//	break;
	//case Hexagon:
	//	m_fHp = 10.0f;
	//	m_fAtk = 1.0f;
	//	//m_tAtkType = AT_Physics;
	//	m_fAtkAnimationMaxTime = 0.0f;
	//	m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
	//	m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
	//	m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
	//	m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
	//	m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
	//	break;
	//case Heptagon:
	//	m_fHp = 10.0f;
	//	m_fAtk = 1.0f;
	//	//m_tAtkType = AT_Physics;
	//	m_fAtkAnimationMaxTime = 0.0f;
	//	m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
	//	m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
	//	m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
	//	m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
	//	m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
	//	break;
	//case Octagon:
	//	m_fHp = 10.0f;
	//	m_fAtk = 1.0f;
	//	//m_tAtkType = AT_Physics;
	//	m_fAtkAnimationMaxTime = 0.0f;
	//	m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
	//	m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
	//	m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
	//	m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
	//	m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
	//	break;
	}
}

CEnemy::CEnemy(int InCornerCount)
	:CFighter(InCornerCount)
{
	SettingStatus();
	m_pHpGage = new CHpUI(m_fHp,CHpUI::Enemy);

	switch (nCornerCount)
	{
	case 3:
		m_pModel = g_pEnemyModel[Enemy1];
		break;
	case 4:
		m_pModel = g_pEnemyModel[Enemy2];
		break;
	}
}

CEnemy::~CEnemy()
{

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
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.X, m_tPos.Y, m_tPos.Z, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.X, m_tSize.Y, m_tSize.Z);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
	{

		Model::Mesh mesh = *m_pModel->GetMesh(i);

		Model::Material material = *m_pModel->GetMaterial(mesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		if (m_pModel) {
			m_pModel->Draw(i);
		}
	}

	////キャラクターの描画
	//m_pSprite->SetTexture(g_pEnemyTex[nCornerCount - 3]);

	//DrawSetting({ m_tPos.X, m_tPos.Y, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z }, m_pSprite);

	//m_pSprite->Draw();

	////m_pEffect->SetEffectTexture();
	////m_pEffect->SetEffect3D({ m_tPos.X,m_tPos.Y,m_tPos.Z });
	////m_pEffect->Draw();

	//m_pSprite->ReSetSprite();
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
		//攻撃音
		if (!m_bTimeSoundStart)
		{
			XAUDIO2_BUFFER buffer = g_AttackSound->GetBuffer(false);
			m_pSourceAttack->SubmitSourceBuffer(&buffer);
			m_pSourceAttack->SetVolume(0.7f);
			m_pSourceAttack->Start();
			m_bTimeSoundStart = true;
		}
		
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{

			//アニメーション処理

			m_fAtkAnimationTime++;
			if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
	}
	//攻撃音の再生時間
	if(m_bTimeSoundStart)
	{
		m_fTimeSound++;
	}
	if(m_fTimeSound > 30)
	{
		m_pSourceAttack->Stop();
		m_bTimeSoundStart = false;
		m_fTimeSound = 0;
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
		m_fHp = 50.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 50.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 50.0f;
		m_fAtk = 1.0f;
		//m_tAtkType = AT_Physics;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	//case Pentagon:
	//	m_fHp = 10.0f;
	//	m_fAtk = 1.0f;
	//	//m_tAtkType = AT_Physics;
	//	m_fAtkAnimationMaxTime = 0.0f;
	//	m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
	//	m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
	//	m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
	//	m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10000);		//キャラクターの中心からの横の索敵当たり判定
	//	m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10000);	//キャラクターの中心からの横の索敵当たり判定
	//	break;
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
	, m_nStatusMode(WandOff)
	, m_nAnimationFrame(0)
{
	m_tSize.X = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Y = NORMAL_SIZE * InSize;	//面積分サイズを大きくする
	m_tSize.Z = NORMAL_SIZE * InSize;	//面積分サイズを大きくする

	m_pSprite[0] = new Sprite();
	m_pSprite[1] = new Sprite();

	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		m_pHpGage = new CHpUI(m_fHp,CHpUI::Player);
		m_nAnimationFrame = 1;
		m_nAnimationX = 8;
		m_nAnimationY = 8;
		break;
	case 1://ボス
		m_pHpGage = new CHpUI(m_fHp, CHpUI::Bos);
		m_nAnimationFrame = 1;
		m_nAnimationX = 1;
		m_nAnimationY = 1;
		break;
	}

	m_tUVPos = { 0.0f,0.0f };
	m_tUVScale = { 1.0f / m_nAnimationX,1.0f / m_nAnimationY };

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

	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		m_pSprite[m_nTextureNumber]->SetTexture(g_pPlayerTex[m_nStatusMode]);
		DrawSetting({ m_tPos.X, m_tPos.Y , m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z }, m_pSprite[m_nTextureNumber]);
		break;
	case 1://ボス
		if (g_pBosCar)
		{
			SetRender3D();
			DirectX::XMFLOAT4X4 wvp[3];
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.X - 2.0f, m_tPos.Y, m_tPos.Z, 0.0f));
			//拡大縮小行列(Scaling)
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
			//回転行列(Rotation)
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
			//それぞれの行列を掛け合わせて格納
			DirectX::XMMATRIX mat = S * R * T;

			world = mat;

			DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
			wvp[1] = GetView();
			wvp[2] = GetProj();

			Geometory::SetView(wvp[1]);
			Geometory::SetProjection(wvp[2]);

			ShaderList::SetWVP(wvp);

			g_pBosCar->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
			g_pBosCar->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

			for (int i = 0; i < g_pBosCar->GetMeshNum(); ++i)
			{

				Model::Mesh mesh = *g_pBosCar->GetMesh(i);

				Model::Material material = *g_pBosCar->GetMaterial(mesh.materialID);
				material.ambient.x = 0.85f; // x (r) 
				material.ambient.y = 0.85f; // y (g) 
				material.ambient.z = 0.85f; // z (b) 
				ShaderList::SetMaterial(material);

				if (g_pBosCar) {
					g_pBosCar->Draw(i);
				}
			}
		}
		m_pSprite[m_nTextureNumber]->SetTexture(g_pBosTex);
		DrawSetting({ m_tPos.X, m_tPos.Y + 30.0f, m_tPos.Z }, { m_tSize.X, m_tSize.Y, m_tSize.Z }, m_pSprite[m_nTextureNumber]);
		break;
	}

	m_pSprite[m_nTextureNumber]->SettingUVAnimation(m_pSprite[m_nTextureNumber], m_nAnimationX, m_nAnimationY, m_nAnimationFrame, false);

	m_pSprite[m_nTextureNumber]->Draw();
	
	m_pSprite[m_nTextureNumber]->ReSetSprite();

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
	//待機アニメーション

	//HPUIの更新処理
	m_pHpGage->Update(m_fHp, { m_tPos.X,m_tPos.Y,m_tPos.Z }, m_tSize.Y);
}

void CLeader::DeathUpdate(void)
{
	//死亡アニメーション

	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
}

CHpUI::CHpUI(float FullHp, HpUINumber Number)
	:m_fFullHp(FullHp)
	,m_tUIPos()
	,m_tUIScale()
	,m_tNumber(Number)
{
	m_pSprite = new Sprite();

	switch (m_tNumber)
	{
	case CHpUI::Ally:
		m_tUIScale.y = 0.8f;
		m_tUIScale.z = 0.5f;
		m_fAnchorPoint = 2.5f;
		break;
	case CHpUI::Enemy:
		m_tUIScale.y = 0.8f;
		m_tUIScale.z = 0.5f;
		m_fAnchorPoint = 4.0f;
		break;
	case CHpUI::Bos:
	case CHpUI::Player:
		m_tUIScale.y = 3.0f;
		m_tUIScale.z = 1.0f;
		m_fAnchorPoint = 5.0f;
		break;
	}
}

CHpUI::~CHpUI()
{
}

void CHpUI::Update(float InHp,DirectX::XMFLOAT3 InPos, float InSizeY)
{
	int HpRatio = 0;

	switch (m_tNumber)
	{
	case CHpUI::Ally:
		m_tUIPos.x = InPos.x;
		m_tUIPos.y = InPos.y + InSizeY - 1.0f;
		m_tUIPos.z = InPos.z;
		HpRatio = (InHp / m_fFullHp) * 4.0f;
		break;
	case CHpUI::Enemy:
		m_tUIPos.x = InPos.x;
		m_tUIPos.y = InPos.y + InSizeY - 1.0f;
		m_tUIPos.z = InPos.z;
		HpRatio = (InHp / m_fFullHp) * 4.0f;
		break;
	case CHpUI::Bos:
		m_tUIPos.x = InPos.x;
		m_tUIPos.y = InPos.y + InSizeY + 21.0f;
		m_tUIPos.z = InPos.z;
		HpRatio = (InHp / m_fFullHp) * 50.0f;
		break;
	case CHpUI::Player:
		m_tUIPos.x = InPos.x;
		m_tUIPos.y = InPos.y + InSizeY + 0.0f;
		m_tUIPos.z = InPos.z;
		HpRatio = (InHp / m_fFullHp) * 50.0f;
		break;
	}


	m_tUIScale.x = HpRatio;
}

void CHpUI::Draw(void)
{
	SetRender2D();
	switch (m_tNumber)
	{
	case CHpUI::Ally:
		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[0][0]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		////ベースの描画
		//m_pSprite->SetTexture(g_pHpGageTex[0][0]);

		//m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		//DrawSetting({ m_tUIPos.x,m_tUIPos.y,m_tUIPos.z }, { 10.0f,5.0f,5.0f }, m_pSprite);

		//m_pSprite->Draw();

		//m_pSprite->ReSetSprite();

		break;
	case CHpUI::Enemy:
		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[1][0]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		////ベースの描画
		//m_pSprite->SetTexture(g_pHpGageTex[1][0]);

		//m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		//DrawSetting({ m_tUIPos.x,m_tUIPos.y,m_tUIPos.z }, { 10.0f,5.0f,5.0f }, m_pSprite);

		//m_pSprite->Draw();

		//m_pSprite->ReSetSprite();

		break;
	case CHpUI::Bos:

		//ベース(Under)の下描画
		m_pSprite->SetTexture(g_pHpGageTex[1][2]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - 20.0f,m_tUIPos.y,m_tUIPos.z }, { 60.0f,15.0f,5.0f }, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[1][0]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x + (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		//ベース(Top)の描画
		m_pSprite->SetTexture(g_pHpGageTex[1][1]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - 20.0f,m_tUIPos.y,m_tUIPos.z }, { 60.0f,15.0f,5.0f }, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		break;

	case CHpUI::Player:
		//ベース(Under)の描画
		m_pSprite->SetTexture(g_pHpGageTex[0][2]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x + 20.0f,m_tUIPos.y,m_tUIPos.z }, { 60.0f,15.0f,5.0f }, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[0][0]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		//ベース(Top)の描画
		m_pSprite->SetTexture(g_pHpGageTex[0][1]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x + 20.0f,m_tUIPos.y,m_tUIPos.z }, { 60.0f,15.0f,5.0f }, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		break;
	}

}

void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize,Sprite* Sprite)
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

	Sprite->SetWorld(wvp[0]);
	Sprite->SetView(wvp[1]);
	Sprite->SetProjection(wvp[2]);
}