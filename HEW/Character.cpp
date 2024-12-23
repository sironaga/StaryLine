#include "Character.h"
#include "DirectXTex/TextureLoad.h"
#include "FieldVertex.h"
#include "Main.h"
#include "Geometory.h"
#include "ShaderList.h"

//キャラクターの横の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_WIDTH(Num)  ((m_tSize.x *(Num / 2)) + (m_tSize.x))
//キャラクターの縦の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(Num) ((m_tSize.y *(Num / 2)) + (m_tSize.y))
//キャラクターの横の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_WIDTH(Num)  ((m_tSize.x * (Num / 2)) + (m_tSize.x))
//キャラクターの縦の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_HEIGHT(Num) ((m_tSize.y * (Num / 2)) + (m_tSize.y))

//味方キャラクターの列挙型
enum class Ally
{
	Ally3,
	Ally4,
	MAX,
};

//敵キャラクターの列挙型
enum class Enemy
{
	Enemy1,
	Enemy2,
	MAX,
};

//リーダーたちの列挙型
enum class Leader
{
	Linie,
	Nugar = 1,
	Boss2 = 1,
	Boss3 = 1,
	MAX,
};

//Hpゲージの列挙型
enum class HpTexture
{
	Gage,
	Top,
};

//Characterのエフェクトの列挙型
enum class CharactersEffect
{
	FighterAttack,
	MAX,
};

/*事前読み込み用のポインタ*/
//味方
Model* g_pAllyModel[(int)Ally::MAX];
//敵
Model* g_pEnemyModel[(int)Enemy::MAX];
//リーダーたち
Model* g_pLeaderModel[(int)Leader::MAX];
//ボスの車
Model* g_pBosCar;
//Hpのテクスチャ
Texture* g_pHpGageTex[2][2];
//キャラクターのエフェクト
CEffectManager* g_pCharacterEffects[(int)CharactersEffect::MAX];
//攻撃音
CSoundList* g_AttackSound;

//事前読み込み用関数
void InitCharacterTexture(CFieldVertex* InAddress,int StageNum)
{
	/*味方キャラクターのModel読み込み*/
	for (int i = 0; i < (int)Ally::MAX; i++)
	{
		g_pAllyModel[i] = new Model();
	}
	g_pAllyModel[(int)Ally::Ally3]->Load("Assets/Model/Character/Triangle/Triangle.fbx", 0.05f, Model::XFlip);
	g_pAllyModel[(int)Ally::Ally4]->Load("Assets/Model/Character/Square/Square.fbx", 0.05f, Model::XFlip);
	
	/*敵キャラクターのModel読み込み*/
	for (int i = 0; i < (int)Enemy::MAX; i++)
	{
		g_pEnemyModel[i] = new Model();
	}
	g_pEnemyModel[(int)Enemy::Enemy1]->Load("Assets/Model/Character/Square/Square.fbx", 0.05f, Model::XFlip);
	g_pEnemyModel[(int)Enemy::Enemy2]->Load("Assets/Model/Character/Triangle/Triangle.fbx", 0.05f, Model::XFlip);

	/*Linie関連のModel読み込み*/
	//Hpゲージ
	g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Gage] = new Texture();
	g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Top] = new Texture();
	g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Gage]->Create("Asset/HpGage/UI_HP_Gage_Linie.png");
	g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Top]->Create("Asset/HpGage/UI_HP_top_Linie.png");
	//モデル
	g_pLeaderModel[(int)Leader::Linie] = new Model();
	g_pLeaderModel[(int)Leader::Linie]->Load("Assets/Model/Leader/Linie/Char_Linie.fbx", 1.0f, Model::XFlip);

	/*ボスたちのModel読み込み*/
	switch (StageNum)
	{
	case 0:
		//Hpゲージ
		g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Gage] = new Texture();
		g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Top] = new Texture();
		g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Gage]->Create("Asset/HpGage/UI_HP_Gage_Nugar.png");
		g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Top]->Create("Asset/HpGage/UI_HP_top_Nugar.png");
		//モデル
		g_pLeaderModel[(int)Leader::Nugar] = new Model();
		g_pLeaderModel[(int)Leader::Nugar]->Load("Assets/Model/Leader/Nugar/Char_Boss02_Nugar.fbx", 1.0f, Model::XFlip);
		//ボスの車
		g_pBosCar = new Model();
		g_pBosCar->Load("Assets/Model/Boss01_Car.fbx", 1.0f, Model::XFlip);
		break;
	case 1:
		//Hpゲージ
		g_pHpGageTex[(int)Leader::Boss2][(int)HpTexture::Gage]  = new Texture();
		g_pHpGageTex[(int)Leader::Boss2][(int)HpTexture::Top]  = new Texture();
		g_pHpGageTex[(int)Leader::Boss2][(int)HpTexture::Gage] ->Create("Asset/HpGage/UI_HP_Gage_Nugar.png");
		g_pHpGageTex[(int)Leader::Boss2][(int)HpTexture::Top]  ->Create("Asset/HpGage/UI_HP_top_Nugar.png");
		//モデル
		g_pLeaderModel[(int)Leader::Boss2] = new Model();
		g_pLeaderModel[(int)Leader::Boss2]->Load("Assets/Model/Leader/Nugar/Char_Boss02_Nugar.fbx", 1.0f, Model::XFlip);
		//ボスの車
		g_pBosCar = new Model();
		g_pBosCar->Load("Assets/Model/Boss01_Car.fbx", 1.0f, Model::XFlip);
		break;
	case 2:
		//Hpゲージ
		g_pHpGageTex[(int)Leader::Boss3][(int)HpTexture::Gage] = new Texture();
		g_pHpGageTex[(int)Leader::Boss3][(int)HpTexture::Top] = new Texture();
		g_pHpGageTex[(int)Leader::Boss3][(int)HpTexture::Gage]->Create("Asset/HpGage/UI_HP_Gage_Nugar.png");
		g_pHpGageTex[(int)Leader::Boss3][(int)HpTexture::Top]->Create("Asset/HpGage/UI_HP_top_Nugar.png");
		//モデル
		g_pLeaderModel[(int)Leader::Boss3] = new Model();
		g_pLeaderModel[(int)Leader::Boss3]->Load("Assets/Model/Leader/Nugar/Char_Boss02_Nugar.fbx", 1.0f, Model::XFlip);
		//ボスの車
		g_pBosCar = new Model();
		g_pBosCar->Load("Assets/Model/Boss01_Car.fbx", 1.0f, Model::XFlip);
		break;
	}

	/*サウンドの読み込み*/
	g_AttackSound = new CSoundList(SE_ATTACK);
	/*エフェクトの読み込み*/
	g_pCharacterEffects[(int)CharactersEffect::FighterAttack] = new CEffectManager(TEX_PASS("Effect/Fire.efk"));
}

//事前読み込みで用意したもののポインタ破棄
void UnInitCharacterTexture()
{
	//味方モデルの破棄
	for (int i = 0; i < (int)Ally::MAX; i++)
	{
		if (g_pAllyModel[i])
		{
			delete g_pAllyModel[i];
			g_pAllyModel[i] = nullptr;
		}
	}
	//敵モデルの破棄
	for (int i = 0; i < (int)Enemy::MAX; i++)
	{
		if (g_pEnemyModel[i])
		{
			delete g_pEnemyModel[i];
			g_pEnemyModel[i] = nullptr;
		}
	}
	//キャラクターのエフェクトの破棄
	for (int i= 0; i < (int)CharactersEffect::MAX; i++)
	{
		delete g_pCharacterEffects[i];
		g_pCharacterEffects[i] = nullptr;
	}

	//Hpテクスチャの破棄
	delete g_pHpGageTex[0][0];
	g_pHpGageTex[0][0] = nullptr;
	delete g_pHpGageTex[0][1];
	g_pHpGageTex[0][1] = nullptr;
	delete g_pHpGageTex[1][0];
	g_pHpGageTex[1][0] = nullptr;
	delete g_pHpGageTex[1][1];
	g_pHpGageTex[1][1] = nullptr;

	//サウンドの破棄
	delete g_AttackSound;
	g_AttackSound = nullptr;

	//エフェクトの破棄
	for (int i = 0; i < (int)CharactersEffect::MAX; i++)
	{
		delete g_pCharacterEffects[i];
		g_pCharacterEffects[i] = nullptr;
	}
}

//キャラクターの基底クラスのコンストラクタ
CFighter::CFighter(int InCornerCount)
	: m_tStatus(St_Create)
	, m_tSearchCollision{ 0.0f,0.0f }
	, m_tAtkCollision{ 0.0f,0.0f }
	, m_tPos{ 0.0f, 0.0f, 0.0f }
	, m_tOldPos{ 0.0f,0.0f,0.0f }
	, m_tSize{ NORMAL_SIZE ,NORMAL_SIZE ,NORMAL_SIZE }
	, m_nCornerCount(InCornerCount)
	, m_fHp(0.0f)
	, m_fAtk(0.0f)
	, m_fAtkCharge(0.0f)
	, m_fAtkChargeMax(0.0f)
	, m_fAtkAnimationTime(0.0f)
	, m_fAtkAnimationMaxTime(0.0f)
	, m_bIsHit(false)
	, m_pHpGage{}
	, m_pEffect{}
	, m_nTargetNumber(-1)
	, m_bIsAttack(false)
	, m_bFirstBattlePosSetting(false)
	, m_MoveFlag(false)
	, m_pSourceAttack(nullptr)
	, m_fTimeSound(0)
	, m_bTimeSoundStart(false)
	, m_pModel(nullptr)
{
	//サウンドの設定
	m_pSourceAttack = g_AttackSound->m_sound->CreateSourceVoice(m_pSourceAttack);
	XAUDIO2_BUFFER buffer = g_AttackSound->GetBuffer(false);
	m_pSourceAttack->SubmitSourceBuffer(&buffer);
}

//キャラクターの基底クラスのデストラクタ
CFighter::~CFighter()
{
	//Hpポインタの破棄
	delete m_pHpGage;
	m_pHpGage = nullptr;

	//エフェクトの破棄
	for (int i = 0; i < (int)CharactersEffect::MAX; i++)
	{
		if (m_pEffect[i])
		{
			m_pEffect[i] = nullptr;
		}
	}

	//サウンドの破棄
	if (m_pSourceAttack)
	{
		m_pSourceAttack->ExitLoop();
		m_pSourceAttack->Stop();
		m_pSourceAttack->DestroyVoice();
		m_pSourceAttack = nullptr;
	}

	//モデルの破棄
	if (m_pModel)
	{
		m_pModel = nullptr;
	}
}

//攻撃の当たり判定チェック
bool CFighter::AtkCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos)
{
		/*自分の攻撃当たり判定の左端 <= 相手の左端*/
	if (m_tPos.x - m_tAtkCollision.Width <= InPos.x - (InSize.x / 2)
		&&
		/*自分の攻撃当たり判定の右端 >= 相手の左端*/
		m_tPos.x + m_tAtkCollision.Width >= InPos.x - (InSize.x / 2)
		||
		/*自分の攻撃当たり判定の左端 <= 相手の右端*/
		m_tPos.x - m_tAtkCollision.Width <= InPos.x + (InSize.x / 2)
		&&
		/*自分の攻撃当たり判定の右端 >= 相手の右端*/
		m_tPos.x + m_tAtkCollision.Width >= InPos.x + (InSize.x / 2))
	{
			/*自分の攻撃当たり判定の上端 <= 相手の下端*/
		if (m_tPos.y - m_tAtkCollision.Height <= InPos.y - (InSize.y / 2)
			&&
			/*自分の攻撃当たり判定の下端 >= 相手の下端*/
			m_tPos.y + m_tAtkCollision.Height >= InPos.y - (InSize.y / 2)
			||
			/*自分の攻撃当たり判定の上端 <= 相手の上端*/
			m_tPos.y - m_tAtkCollision.Height <= InPos.y + (InSize.y / 2)
			&&
			/*自分の攻撃当たり判定の下端 >= 相手の上端*/
			m_tPos.y + m_tAtkCollision.Height >= InPos.y + (InSize.y / 2))
		{
			/*自分の攻撃当たり判定の前端 <= 相手の前端*/
			if (m_tPos.z - m_tAtkCollision.Width <= InPos.z - (InSize.z / 2)
				&&
				/*自分の攻撃当たり判定の奥端 >= 相手の前端*/
				m_tPos.z + m_tAtkCollision.Width >= InPos.z - (InSize.z / 2)
				||
				/*自分の攻撃当たり判定の前端 <= 相手の奥端*/
				m_tPos.z - m_tAtkCollision.Width <= InPos.z + (InSize.z / 2)
				&&
				/*自分の攻撃当たり判定の奥端 >= 相手の奥端*/
				m_tPos.z + m_tAtkCollision.Width >= InPos.z + (InSize.z / 2))
			{
				return true;
			}
		}
	}
	return false;
}

//索敵の当たり判定チェック
bool CFighter::SearchCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos)
{
		/*自分の索敵当たり判定の左端 <= 相手の左端*/
	if (m_tPos.x - m_tSearchCollision.Width <= InPos.x - InSize.x
		&&
		/*自分の索敵当たり判定の右端 >= 相手の左端*/
		m_tPos.x + m_tSearchCollision.Width >= InPos.x - InSize.x
		||
		/*自分の索敵当たり判定の左端 <= 相手の右端*/
		m_tPos.x - m_tSearchCollision.Width <= InPos.x + InSize.x
		&&
		/*自分の索敵当たり判定の右端 >= 相手の右端*/
		m_tPos.x + m_tSearchCollision.Width >= InPos.x + InSize.x)
	{
			/*自分の索敵当たり判定の上端 <= 相手の上端*/
		if (m_tPos.y - m_tSearchCollision.Height <= InPos.y - InSize.y
			&&
			/*自分の索敵当たり判定の下端 >= 相手の上端*/
			m_tPos.y + m_tSearchCollision.Height >= InPos.y - InSize.y
			||
			/*自分の索敵当たり判定の上端 <= 相手の下端*/
			m_tPos.y - m_tSearchCollision.Height <= InPos.y + InSize.y
			&&
			/*自分の索敵当たり判定の下端 >= 相手の下端*/
			m_tPos.y + m_tSearchCollision.Height >= InPos.y + InSize.y)
		{
				/*自分の索敵当たり判定の前端 <= 相手の前端*/
			if (m_tPos.z - m_tSearchCollision.Width <= InPos.z - InSize.z
				&&
				/*自分の索敵当たり判定の奥端 >= 相手の前端*/
				m_tPos.z + m_tSearchCollision.Width >= InPos.z - InSize.z
				||
				/*自分の索敵当たり判定の前端 <= 相手の奥端*/
				m_tPos.z - m_tSearchCollision.Width <= InPos.z + InSize.z
				&&
				/*自分の索敵当たり判定の奥端 >= 相手の奥端*/
				m_tPos.z + m_tSearchCollision.Width >= InPos.z + InSize.z)
			{
				return true;
			}
		}
	}
	return false;
}

//重なり判定チェック
bool CFighter::OverlapCheck(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize)
{
	//サイズを重なり用に調整
	DirectX::XMFLOAT3 Size;
	Size.x = m_tSize.x * 0.2f;
	Size.y = m_tSize.y * 0.2f;
	Size.z = m_tSize.z * 0.2f;

	InSize.x = InSize.x * 0.2f;
	InSize.y = InSize.y * 0.2f;
	InSize.z = InSize.z * 0.2f;

		/*自分の左端 <= 相手の左端*/
	if (m_tPos.x - Size.x <= InPos.x - InSize.x	
		&&
		/*自分の右端 >= 相手の左端*/
		m_tPos.x + Size.x >= InPos.x - InSize.x
		||
		/*自分の左端 <= 相手の右端*/
		m_tPos.x - Size.x <= InPos.x + InSize.x
		&&
		/*自分の右端 >= 相手の右端*/
		m_tPos.x + Size.x >= InPos.x + InSize.x)
	{
			/*自分の上端 <= 相手の上端*/
		if (m_tPos.y - Size.y <= InPos.y - InSize.y
			&&
			/*自分の下端 >= 相手の上端*/
			m_tPos.y + Size.y >= InPos.y - InSize.y
			||
			/*自分の上端 <= 相手の下端*/
			m_tPos.y - Size.y <= InPos.y + InSize.y
			&&
			/*自分の下端 >= 相手の下端*/
			m_tPos.y + Size.y >= InPos.y + InSize.y)
		{
				/*自分の前端 <= 相手の前端*/
			if (m_tPos.z - Size.x <= InPos.z - InSize.z
				&&
				/*自分の奥端 >= 相手の前端*/
				m_tPos.z + Size.x >= InPos.z - InSize.z
				||
				/*自分の前端 <= 相手の奥端*/
				m_tPos.z - Size.x <= InPos.z + InSize.z
				&&
				/*自分の奥端 >= 相手の奥端*/
				m_tPos.z + Size.x >= InPos.z + InSize.z)
			{
				return true;
			}
		}
	}
	return false;
}

//ダメージを受ける
void CFighter::Damage(CFighter* pFighter)
{
	//相手の攻撃力分体力を減らす
	m_fHp -= pFighter->GetAtk();
	//当たった判定をオンにする
	m_bIsHit = true;
}

//味方クラスのコンストラクタ
CAlly::CAlly(int InCornerCount)
	:CFighter(InCornerCount)
{
	//初期ステータス決め
	SettingStatus();
	//Hpのポインタの作成
	m_pHpGage = new CHpUI(m_fHp,CHpUI::Ally);
	//角数ごとに同期するモデルポインタを変更
	switch (m_nCornerCount)
	{
	case 3:
		m_pModel = g_pAllyModel[(int)Ally::Ally3];
		break;
	case 4:
		m_pModel = g_pAllyModel[(int)Ally::Ally4];
		break;
	}
	//攻撃エフェクトのポインタ同期
	m_pEffect[(int)FighterEffect::Attack] = g_pCharacterEffects[(int)CharactersEffect::FighterAttack];
}

//味方クラスのデストラクタ
CAlly::~CAlly()
{

}

//味方クラスの更新処理
void CAlly::Update(void)
{
	// 状況に応じて処理を分ける
	switch (m_tStatus)
	{
		//生成更新
	case St_Create:	CreateUpdate();		break;
		//戦闘更新
	case St_Battle:	BattleUpdate();		break;
		//死亡更新
	case St_Death:	DeathUpdate();		break;
	}

	//エフェクトの更新
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
	//行列(Scaling)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x, m_tPos.y, m_tPos.z, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.x, m_tSize.y, m_tSize.z);
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

	//エフェクトの描画
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
		m_pEffect[i]->Draw();
	}
}
//生成更新処理
void CAlly::CreateUpdate(void)
{
	//生成アニメーション

	//生成移動
	
	//生成アニメーションが終わったら
	SetStatus(St_Battle);
	m_bFirstBattlePosSetting = false;
}
//戦闘更新処理
void CAlly::BattleUpdate(void)
 {
	//攻撃していたら
	if (m_bIsAttack)
	{
		//攻撃エフェクト
		if (m_pEffect[(int)FighterEffect::Attack]->GetEffectNum() < MAX_EFFECT)
		{
			//m_pEffect[(int)FighterEffect::Attack]->Play({ m_tPos.x, m_tPos.y, m_tPos.z - m_tSize.z/2}, 60);
		}
		//攻撃音
		if (!m_bTimeSoundStart)
		{
			XAUDIO2_BUFFER buffer = g_AttackSound->GetBuffer(false);
			m_pSourceAttack->SubmitSourceBuffer(&buffer);
			if(m_pSourceAttack)SetVolumeSE(m_pSourceAttack);
			m_pSourceAttack->Start();
			m_bTimeSoundStart = true;
		}
		//攻撃チャージが攻撃アニメーションの時間を引いたMAX数値分たまっているかどうか
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{
			//アニメーション処理

			//アニメーションの時間の加算
			m_fAtkAnimationTime++;
			//アニメーションの時間が最大以上になったら初期化する
			if (m_fAtkAnimationMaxTime <= m_fAtkCharge)m_fAtkAnimationTime = 0;
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
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);
}
//死亡更新処理
void CAlly::DeathUpdate(void)
{
	//死亡アニメーション
	
	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
}

//初期ステータス決め
void CAlly::SettingStatus(void)
{
	//角数別にカウント
	switch (m_nCornerCount)
	{
	default:
		//体力
		m_fHp = 50.0f;
		//攻撃力
		m_fAtk = 1.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);
		//キャラクターの中心からの縦の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);	
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);
		break;
	/*三角形*/
	case Triangle:
		//体力
		m_fHp = 50.0f;
		//攻撃力
		m_fAtk = 1.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);
		//キャラクターの中心からの縦の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);
		break;
	/*四角形*/
	case Square:
		//体力
		m_fHp = 50.0f;
		//攻撃力
		m_fAtk = 1.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);
		//キャラクターの中心からの縦の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);
		break;
	}
}

CEnemy::CEnemy(int InCornerCount)
	:CFighter(InCornerCount)
{
	SettingStatus();
	m_pHpGage = new CHpUI(m_fHp,CHpUI::Enemy);

	switch (m_nCornerCount)
	{
	case 3:
		m_pModel = g_pEnemyModel[(int)Enemy::Enemy1];
		break;
	case 4:
		m_pModel = g_pEnemyModel[(int)Enemy::Enemy2];
		break;
	}
}

CEnemy::~CEnemy()
{

}

void CEnemy::Update(void)
{
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);

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
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x, m_tPos.y, m_tPos.z, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.x, m_tSize.y, m_tSize.z);
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
			if(m_pSourceAttack)SetVolumeSE(m_pSourceAttack);
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
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);
}

void CEnemy::DeathUpdate(void)
{
	//死亡アニメーション
	
	//死亡アニメーションが終わったら
	SetStatus(St_Delete);
	
}

void CEnemy::SettingStatus(void)
{
	switch (m_nCornerCount)
	{
	default:
		m_fHp = 50.0f;
		m_fAtk = 1.0f;
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
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}

CLeader::CLeader(float InSize, DirectX::XMFLOAT3 FirstPos, int InTextureNumber)
	:m_tStatus(St_Create)
	,m_tPos(FirstPos)
	,m_fHp(300.0f)
	,m_nTextureNumber(InTextureNumber)
	, m_pHpGage(nullptr)
	, m_nAnimationFrame(0)
{
	m_tSize.x = InSize;
	m_tSize.y = InSize;
	m_tSize.z = InSize;

	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		m_pModel = g_pLeaderModel[(int)Leader::Linie];
		m_pHpGage = new CHpUI(m_fHp,CHpUI::Player);
		m_nAnimationFrame = 1;
		m_nAnimationX = 8;
		m_nAnimationY = 8;
		break;
	case 1://ボス
		m_pModel = g_pLeaderModel[(int)Leader::Nugar];
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
	m_pHpGage->Update(m_fHp,{m_tPos.x,m_tPos.y,m_tPos.z},m_tSize.y);

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
	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		if (m_pModel)
		{
			SetRender3D();
			DirectX::XMFLOAT4X4 wvp[3];
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x - 2.0f, m_tPos.y, m_tPos.z, 0.0f));
			//拡大縮小行列(Scaling)
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.x, m_tSize.y, m_tSize.z);
			//回転行列(Rotation)
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(85.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
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
		}

		break;
	case 1://ボス
		if (g_pBosCar)
		{
			SetRender3D();
			DirectX::XMFLOAT4X4 wvp[3];
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x - 2.0f, m_tPos.y, m_tPos.z, 0.0f));
			//拡大縮小行列(Scaling)
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f);
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
		if (m_pModel)
		{
			SetRender3D();
			DirectX::XMFLOAT4X4 wvp[3];
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x - 2.0f, m_tPos.y + 15.0f, m_tPos.z, 0.0f));
			//拡大縮小行列(Scaling)
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.x, m_tSize.y, m_tSize.z);
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
		}
		break;
	}
/*Modelテスト用*/
	//SetRender3D();
	//DirectX::XMFLOAT4X4 wvp[3];
	//DirectX::XMMATRIX world;
	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(0.0f, 10.0f, m_tPos.z, 0.0f));
	////拡大縮小行列(Scaling)
	//DirectX::XMMATRIX S = DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f);
	////回転行列(Rotation)
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(180.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	////それぞれの行列を掛け合わせて格納
	//DirectX::XMMATRIX mat = S * R * T;
}

void CLeader::HpDraw(void)
{
	//体力ゲージの描画
	m_pHpGage->Draw();
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
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);
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
		m_fAnchorPoint = 8.0f;
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
		m_tUIPos.x = InPos.x + 8.0f;
		m_tUIPos.y = InPos.y + 20.0f - 34.8f;
		m_tUIPos.z = InPos.z;
		HpRatio = (InHp / m_fFullHp) * 95.0f;
		break;
	case CHpUI::Player:
		m_tUIPos.x = InPos.x - 8.0f;
		m_tUIPos.y = InPos.y + 20.0f - 34.8f;
		m_tUIPos.z = InPos.z;
		HpRatio = (InHp / m_fFullHp) * 95.0f;
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

		break;
	case CHpUI::Enemy:
		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[1][0]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		break;
	case CHpUI::Bos:
		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[1][(int)HpTexture::Gage]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x + (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y - 5.0f ,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		//ベース(Top)の描画
		m_pSprite->SetTexture(g_pHpGageTex[1][(int)HpTexture::Top]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - 40.0f,m_tUIPos.y,m_tUIPos.z }, { 100.0f,15.0f,5.0f }, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		break;

	case CHpUI::Player:
		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[0][(int)HpTexture::Gage]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x - (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y - 5.0f,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		//ベース(Top)の描画
		m_pSprite->SetTexture(g_pHpGageTex[0][(int)HpTexture::Top]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		DrawSetting({ m_tUIPos.x + 40.0f,m_tUIPos.y,m_tUIPos.z }, { 100.0f,15.0f,5.0f }, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		break;
	}

}

void CHpUI::DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize,Sprite* Sprite)
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