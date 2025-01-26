#include "Character.h"
#include "DirectXTex/TextureLoad.h"
#include "FieldVertex.h"
#include "Main.h"
#include "Geometory.h"
#include "ShaderList.h"
#include "Input.h"
#include "StageSelect.h"

//キャラクターの横の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_WIDTH(Num)  ((m_tSize.x *(Num / 2)) + (m_tSize.x))
//キャラクターの縦の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(Num) ((m_tSize.y *(Num / 2)) + (m_tSize.y))
//キャラクターの横の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_WIDTH(Num)  ((m_tSize.x * (Num / 2)) + (m_tSize.x))
//キャラクターの縦の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_HEIGHT(Num) ((m_tSize.y * (Num / 2)) + (m_tSize.y))

//モデル表示でバック用変数
float Y = 0.0f;

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
	Quracker = 1,
	Nugar = 1,
	Kannele = 1,
	Boldow = 2,
	MAX,
};

//Hpゲージの列挙型
enum class HpTexture
{
	Linie,
	Boss,
	Frame,
	Ally1,
	Ally2,
	Enemy1,
	Enemy2,
	MAX,
};

//Characterのエフェクトの列挙型
enum class CharactersEffect
{
	Aura,
	Create,
	SwordAtk,
	BowAtk,
	Death,
	MAX,
};

enum class DamageDraw
{
	NormalBlue,
	NormalRed,
	NotQuite,
	MAX,
};

/*事前読み込み用のポインタ*/
//味方
Model* g_pAllyModel[(int)Ally::MAX];
//敵
Model* g_pEnemyModel[(int)Enemy::MAX];
//リーダーたち
Model* g_pLeaderModel[(int)Leader::MAX];
//リーダー用アニメーション
Model::AnimeNo g_pLeader_Anima;
//ボスの車
Model* g_pBosCar;

//キャラクターのエフェクト
CEffectManager_sp* g_pCharacterEffects[(int)CharactersEffect::MAX];

//Hpのテクスチャ
Texture* g_pHpGageTex[(int)HpTexture::MAX];

//ダメージログのテクスチャ
Texture* g_pDamageTex[(int)DamageDraw::MAX];

//攻撃音
CSoundList* g_NormalAttackSound;
CSoundList* g_WeaknessAttackSound;
CSoundList* g_SummonSound;
CSoundList* g_wandonoffSound;
IXAudio2SourceVoice* g_pSourceSummon[2];//スピーカー
IXAudio2SourceVoice* g_pSourceWandonoff;//スピーカー

void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize, Sprite* Sprite);

//事前読み込み用関数
void InitCharacterTexture(StageType StageType)
{
	g_pLeaderModel[0] = nullptr;
	g_pLeaderModel[1] = nullptr;
	g_pLeaderModel[2] = nullptr;
	/*味方キャラクターのModel読み込み*/
	for (int i = 0; i < (int)Ally::MAX; i++)
	{
		g_pAllyModel[i] = new Model();
	}
	g_pAllyModel[(int)Ally::Ally3]->Load(MODEL_PASS("Character/Triangle/Triangle.fbx"), 0.05f, Model::None);
	g_pAllyModel[(int)Ally::Ally4]->Load(MODEL_PASS("Character/Square/Square.fbx"), 0.05f, Model::None);
	g_pHpGageTex[(int)HpTexture::Ally1] = new Texture();
	g_pHpGageTex[(int)HpTexture::Ally1]->Create(TEX_PASS("HpGage/Battle_HP_Gage_LinieS.png"));
	g_pHpGageTex[(int)HpTexture::Ally2] = new Texture();
	g_pHpGageTex[(int)HpTexture::Ally2]->Create(TEX_PASS("HpGage/Battle_HP_Gage_LinieS.png"));
	/*Linie関連のModel読み込み*/
	//Hpゲージ
	g_pHpGageTex[(int)HpTexture::Linie] = new Texture();
	g_pHpGageTex[(int)HpTexture::Linie]->Create(TEX_PASS("HpGage/Battle_HP_Gage_Linie.png"));

	//g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Gage] = new Texture();
	//g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Top] = new Texture();
	//g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Gage]->Create(TEX_PASS("HpGage/UI_HP_Gage_Linie.png"));
	//g_pHpGageTex[(int)Leader::Linie][(int)HpTexture::Top]->Create(TEX_PASS("HpGage/UI_HP_top_Linie.png"));
	//モデル
	g_pLeaderModel[(int)Leader::Linie] = new Model();
	g_pLeaderModel[(int)Leader::Linie]->Load(MODEL_PASS("Leader/Linie/Anim_Char_Main_Linie_WandON.fbx"), 1.0f, Model::None);
	g_pLeader_Anima = g_pLeaderModel[(int)Leader::Linie]->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Char_Main_Linie_WandON.fbx"));

	g_pDamageTex[(int)DamageDraw::NormalBlue] = new Texture();
	g_pDamageTex[(int)DamageDraw::NormalBlue]->Create(TEX_PASS("DamageLog/Battle_Damage_Blue.png"));
	g_pDamageTex[(int)DamageDraw::NormalRed] = new Texture();
	g_pDamageTex[(int)DamageDraw::NormalRed]->Create(TEX_PASS("DamageLog/Battle_Damage_Pink.png"));
	g_pDamageTex[(int)DamageDraw::NotQuite] = new Texture();
	g_pDamageTex[(int)DamageDraw::NotQuite]->Create(TEX_PASS("DamageLog/Battle_Damage_Gray.png"));

	//ステージ別に読み込みを変える
	switch (StageType.StageMainNumber)
	{
	case (int)E_SELECT_STAGETYPE::GRASSLAND://草原
		
		/*敵キャラクターのModel読み込み*/
		for (int i = 0; i < (int)Enemy::MAX; i++)
		{
			g_pEnemyModel[i] = new Model();
		}
		g_pEnemyModel[(int)Enemy::Enemy1]->Load(MODEL_PASS("Character/Idiot1/Stage1/Stage1_Idiot1.fbx"), 0.05f, Model::None);
		g_pEnemyModel[(int)Enemy::Enemy2]->Load(MODEL_PASS("Character/Idiot2/Stage1/Stage1_Idiot2.fbx"), 0.05f, Model::None);
		g_pHpGageTex[(int)HpTexture::Enemy1] = new Texture();
		g_pHpGageTex[(int)HpTexture::Enemy1]->Create(TEX_PASS("HpGage/Battle_HP_Gage_QrackerS.png"));
		g_pHpGageTex[(int)HpTexture::Enemy2] = new Texture();
		g_pHpGageTex[(int)HpTexture::Enemy2]->Create(TEX_PASS("HpGage/Battle_HP_Gage_QrackerS.png"));

		/*ボスたちのModel読み込み*/
		//Hpゲージ
		g_pHpGageTex[(int)HpTexture::Boss] = new Texture();
		g_pHpGageTex[(int)HpTexture::Boss]->Create(TEX_PASS("HpGage/Battle_HP_Gage_Qracker.png"));
		g_pHpGageTex[(int)HpTexture::Frame] = new Texture();
		g_pHpGageTex[(int)HpTexture::Frame]->Create(TEX_PASS("HpGage/Battle_HP_top_Qracker.png"));
		//g_pHpGageTex[(int)Leader::Quracker][(int)HpTexture::Gage]  = new Texture();
		//g_pHpGageTex[(int)Leader::Quracker][(int)HpTexture::Top]  = new Texture();
		//g_pHpGageTex[(int)Leader::Quracker][(int)HpTexture::Gage] ->Create(TEX_PASS("HpGage/UI_HP_Gage_Nugar.png"));
		//g_pHpGageTex[(int)Leader::Quracker][(int)HpTexture::Top]  ->Create(TEX_PASS("HpGage/UI_HP_top_Nugar.png"));
		//モデル
		g_pLeaderModel[(int)Leader::Quracker] = new Model();
		g_pLeaderModel[(int)Leader::Quracker]->Load(MODEL_PASS("Leader/Qracker/Char_Boss01_Qracker.fbx"), 1.0f, Model::None);
		g_pLeaderModel[2] = nullptr;

		switch (StageType.StageSubNumber)
		{
		case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE1:
			//ボスの車(初期段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss01_Chariot.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE2:
			//ボスの車(中間段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss01_Chariot.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE3:
			//ボスの車(最終段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss01_Chariot.fbx"), 1.0f, Model::None);
			break;
		}
		break;
	case (int)E_SELECT_STAGETYPE::DESERT://砂漠

		/*敵キャラクターのModel読み込み*/
		for (int i = 0; i < (int)Enemy::MAX; i++)
		{
			g_pEnemyModel[i] = new Model();
		}
		g_pEnemyModel[(int)Enemy::Enemy1]->Load(MODEL_PASS("Character/Idiot1/Stage2/Stage2_Idiot1.fbx"), 0.05f, Model::None);
		g_pEnemyModel[(int)Enemy::Enemy2]->Load(MODEL_PASS("Character/Idiot2/Stage2/Stage2_Idiot2.fbx"), 0.05f, Model::None);
		g_pHpGageTex[(int)HpTexture::Enemy1] = new Texture();
		g_pHpGageTex[(int)HpTexture::Enemy1]->Create(TEX_PASS("HpGage/Battle_HP_Gage_NugarS.png"));
		g_pHpGageTex[(int)HpTexture::Enemy2] = new Texture();
		g_pHpGageTex[(int)HpTexture::Enemy2]->Create(TEX_PASS("HpGage/Battle_HP_Gage_NugarS.png"));

		/*ボスたちのModel読み込み*/
		//Hpゲージ
		g_pHpGageTex[(int)HpTexture::Boss] = new Texture();
		g_pHpGageTex[(int)HpTexture::Boss]->Create(TEX_PASS("HpGage/Battle_HP_Gage_Nugar.png"));
		g_pHpGageTex[(int)HpTexture::Frame] = new Texture();
		g_pHpGageTex[(int)HpTexture::Frame]->Create(TEX_PASS("HpGage/Battle_HP_top_Nugar.png"));
		//g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Gage] = new Texture();
		//g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Top] = new Texture();
		//g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Gage]->Create(TEX_PASS("HpGage/UI_HP_Gage_Nugar.png"));
		//g_pHpGageTex[(int)Leader::Nugar][(int)HpTexture::Top]->Create(TEX_PASS("HpGage/UI_HP_top_Nugar.png"));
		
		//モデル
		g_pLeaderModel[(int)Leader::Nugar] = new Model();
		g_pLeaderModel[(int)Leader::Nugar]->Load(MODEL_PASS("Leader/Nugar/Char_Boss02_Nugar.fbx"), 1.0f, Model::None);
		g_pLeaderModel[2] = nullptr;

		switch (StageType.StageSubNumber)
		{
		case (int)E_SELECT_STAGENUMBER::DESERT_STAGE1:
			//ボスの車(初期段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss02_Car.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::DESERT_STAGE2:
			//ボスの車(中間段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss02_Car.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::DESERT_STAGE3:
			//ボスの車(最終段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss02_Car.fbx"), 1.0f, Model::None);
			break;
		}
		break;
	case (int)E_SELECT_STAGETYPE::SNOWFIELD://雪原
		
		/*敵キャラクターのModel読み込み*/
		for (int i = 0; i < (int)Enemy::MAX; i++)
		{
			g_pEnemyModel[i] = new Model();
		}
		g_pEnemyModel[(int)Enemy::Enemy1]->Load(MODEL_PASS("Character/Idiot1/Stage3/Stage3_Idiot1.fbx"), 0.05f, Model::None);
		g_pEnemyModel[(int)Enemy::Enemy2]->Load(MODEL_PASS("Character/Idiot2/Stage3/Stage3_Idiot2.fbx"), 0.05f, Model::None);
		g_pHpGageTex[(int)HpTexture::Enemy1] = new Texture();
		g_pHpGageTex[(int)HpTexture::Enemy1]->Create(TEX_PASS("HpGage/Battle_HP_Gage_KanneleS.png"));
		g_pHpGageTex[(int)HpTexture::Enemy2] = new Texture();
		g_pHpGageTex[(int)HpTexture::Enemy2]->Create(TEX_PASS("HpGage/Battle_HP_Gage_BoldowS.png"));

		/*ボスたちのModel読み込み*/
		//Hpゲージ
		g_pHpGageTex[(int)HpTexture::Boss] = new Texture();
		g_pHpGageTex[(int)HpTexture::Boss]->Create(TEX_PASS("HpGage/Battle_HP_Gage_KanneleBoldow.png"));
		g_pHpGageTex[(int)HpTexture::Frame] = new Texture();
		g_pHpGageTex[(int)HpTexture::Frame]->Create(TEX_PASS("HpGage/Battle_HP_top_KanneleBoldow.png"));
		//g_pHpGageTex[(int)Leader::Kannele][(int)HpTexture::Gage] = new Texture();
		//g_pHpGageTex[(int)Leader::Kannele][(int)HpTexture::Top] = new Texture();
		//g_pHpGageTex[(int)Leader::Kannele][(int)HpTexture::Gage]->Create(TEX_PASS("HpGage/UI_HP_Gage_Nugar.png"));
		//g_pHpGageTex[(int)Leader::Kannele][(int)HpTexture::Top]->Create(TEX_PASS("HpGage/UI_HP_top_Nugar.png"));
		//モデル
		g_pLeaderModel[(int)Leader::Kannele] = new Model();
		g_pLeaderModel[(int)Leader::Kannele]->Load(MODEL_PASS("Leader/Kannele/Char_Boss03_Kannele.fbx"), 1.0f, Model::None);
		g_pLeaderModel[(int)Leader::Boldow] = new Model();
		g_pLeaderModel[(int)Leader::Boldow]->Load(MODEL_PASS("Leader/Boldow/Char_Boss03_Boldow.fbx"), 1.0f, Model::None);
		
		switch (StageType.StageSubNumber)
		{
		case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE1:
			//ボスの車(初期段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss03_LiveStage.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE2:
			//ボスの車(中間段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss03_LiveStage.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE3:
			//ボスの車(最終段階)
			g_pBosCar = new Model();
			g_pBosCar->Load(MODEL_PASS("Car/Prop_Boss03_LiveStage.fbx"), 1.0f, Model::None);
			break;
		}
		break;
	}

	/*サウンドの読み込み*/
	g_NormalAttackSound = new CSoundList(SE_NORMALATTACK);
	g_NormalAttackSound->SetMasterVolume();
	g_WeaknessAttackSound = new CSoundList(SE_WEAKNESSATTACK);
	g_WeaknessAttackSound->SetMasterVolume();
	g_SummonSound = new CSoundList(SE_SUMMON);
	g_SummonSound->SetMasterVolume();
	for (int i = 0; i < 2; i++)
	{
		g_pSourceSummon[i] = g_SummonSound->m_sound->CreateSourceVoice(g_pSourceSummon[i]);
		XAUDIO2_BUFFER buffer = g_SummonSound->GetBuffer(false);
		g_pSourceSummon[i]->SubmitSourceBuffer(&buffer);
	}
	g_wandonoffSound = new CSoundList(SE_WANDONOFF);
	g_wandonoffSound->SetMasterVolume();
	g_pSourceWandonoff = g_wandonoffSound->GetSound(false);
	/*エフェクトの読み込み*/
	//g_pCharacterEffects[(int)CharactersEffect::SwordAtk] = new CEffectManager(EFFECT_PASS("SwordAtk.efkefc"));
	////g_pCharacterEffects[(int)CharactersEffect::SwordAtk] = new CEffectManager(EFFECT_PASS("fire.efk"));
	//g_pCharacterEffects[(int)CharactersEffect::BowAtk] = new CEffectManager(EFFECT_PASS("Yumi.efkefc"));
	//g_pCharacterEffects[(int)CharactersEffect::Move] = new CEffectManager(EFFECT_PASS("MoveChara.efkefc"));
	//g_pCharacterEffects[(int)CharactersEffect::Death] = new CEffectManager(EFFECT_PASS("Death.efkefc"));
	////g_pCharacterEffects[(int)CharactersEffect::Death] = new CEffectManager(EFFECT_PASS("fire.efk"));
	g_pCharacterEffects[(int)CharactersEffect::Aura] = new CEffectManager_sp(EFFECT_PASS("Sprite/Aura.png"), 4, 14, 3.0f);
	g_pCharacterEffects[(int)CharactersEffect::Create] = new CEffectManager_sp(EFFECT_PASS("Sprite/warp.png"), 4, 8, 3.0f);
	g_pCharacterEffects[(int)CharactersEffect::SwordAtk] = new CEffectManager_sp(EFFECT_PASS("Sprite/SwordAtk.png"), 4, 7,1.0f);
	g_pCharacterEffects[(int)CharactersEffect::BowAtk] = new CEffectManager_sp(EFFECT_PASS("Sprite/BowAtk.png"), 4, 6, 1.0f);
	g_pCharacterEffects[(int)CharactersEffect::Death] = new CEffectManager_sp(EFFECT_PASS("Sprite/Death.png"), 4, 8, 3.0f);
}

//事前読み込みで用意したもののポインタ破棄
void UnInitCharacterTexture()
{
	//車モデルの破棄
	SAFE_DELETE(g_pBosCar);
	//味方モデルの破棄
	for (int i = 0; i < (int)Ally::MAX; i++)SAFE_DELETE(g_pAllyModel[i]);
	//敵モデルの破棄
	for (int i = 0; i < (int)Enemy::MAX; i++)SAFE_DELETE(g_pEnemyModel[i]);
	//リーダーたちのモデルの破棄
	for (int i = 0; i < (int)Leader::MAX; i++)SAFE_DELETE(g_pLeaderModel[i]);
	//Hpテクスチャの破棄
	for (int i = 0; i < (int)HpTexture::MAX; i++)SAFE_DELETE(g_pHpGageTex[i]);
	//ダメーログジテクスチャの破棄
	for (int i = 0; i < (int)DamageDraw::MAX; i++)SAFE_DELETE(g_pDamageTex[i]);
	//サウンドの破棄
	SAFE_DELETE(g_NormalAttackSound);
	SAFE_DELETE(g_WeaknessAttackSound);
	for (int i = 0; i < 2; i++)
	{
		if (g_pSourceSummon[i])
		{
			g_pSourceSummon[i]->Stop();
			g_pSourceSummon[i]->DestroyVoice();
			g_pSourceSummon[i] = nullptr;
		}
	}
	SAFE_DELETE(g_SummonSound);
	
	SAFE_DELETE(g_wandonoffSound);
	g_pSourceWandonoff = nullptr;
	//エフェクトの破棄
	for (int i = 0; i < (int)CharactersEffect::MAX; i++)SAFE_DELETE(g_pCharacterEffects[i]);
}

void ReLoadCharacterTexture(StageType StageType)
{
	UnInitCharacterTexture();
	InitCharacterTexture(StageType);
}

void ReLoadSound()
{
	SAFE_DELETE(g_NormalAttackSound);
	SAFE_DELETE(g_WeaknessAttackSound);
	for (int i = 0; i < 2; i++)
	{
		g_pSourceSummon[i]->Stop();
		g_pSourceSummon[i]->DestroyVoice();
		g_pSourceSummon[i] = nullptr;
	}
	SAFE_DELETE(g_SummonSound);
	g_pSourceWandonoff->Stop();
	g_pSourceWandonoff = nullptr;
	SAFE_DELETE(g_wandonoffSound);
	/*サウンドの読み込み*/
	g_NormalAttackSound = new CSoundList(SE_NORMALATTACK);
	g_NormalAttackSound->SetMasterVolume();
	g_WeaknessAttackSound = new CSoundList(SE_WEAKNESSATTACK);
	g_WeaknessAttackSound->SetMasterVolume();
	g_SummonSound = new CSoundList(SE_SUMMON);
	g_SummonSound->SetMasterVolume();
	for (int i = 0; i < 2; i++)
	{
		g_pSourceSummon[i] = g_SummonSound->m_sound->CreateSourceVoice(g_pSourceSummon[i]);
		XAUDIO2_BUFFER buffer = g_SummonSound->GetBuffer(false);
		g_pSourceSummon[i]->SubmitSourceBuffer(&buffer);
	}
	g_wandonoffSound = new CSoundList(SE_WANDONOFF);
	g_wandonoffSound->SetMasterVolume();
	g_pSourceWandonoff = g_wandonoffSound->GetSound(false);
}

//キャラクターの基底クラスのコンストラクタ
CFighter::CFighter(int InCornerCount, bool IsStellaBuff)
	: m_tStatus(St_Create)
	, m_tSearchCollision{ 0.0f,0.0f }
	, m_tAtkCollision{ 0.0f,0.0f }
	, m_tPos{ 0.0f, 0.0f, 0.0f }
	, m_tFirstPos{ 0.0f,0.0f,0.0f }
	, m_tSize{ 0.0f,0.0f,0.0f }
	, m_nCornerCount(InCornerCount)
	, m_fHp(0.0f)
	, m_fAtk(0.0f)
	, m_fAtkCharge(0.0f)
	, m_fAtkChargeMax(0.0f)
	, m_fAtkAnimationTime(0.0f)
	, m_fAtkAnimationMaxTime(0.0f)
	, m_bIsHit(false)
	, m_pHpGage{}
	, m_nTargetNumber(-1)
	, m_bIsAttack(false)
	, m_bFirstBattlePosSetting(false)
	, m_bMoveFlag(false)
	, m_pSourceNormalAttack(nullptr)
	, m_fTimeSound(0)
	, m_bTimeSoundStart(false)
	, m_pModel(nullptr)
	, m_tDestinationPos()
	, m_bReLoadFlag(false)
	, m_pEffect{ nullptr }
	, IsCreateEffectPlay(false)
	, IsAttackEffectPlay(false)
	, IsDeathEffectPlay(false)
	, m_tTargetPos()
	, m_bMoveUp(false)
	, m_bStellaBuff(IsStellaBuff)
	, m_bDamageLogDraw{false}
{
	//サウンドの設定
	m_pSourceNormalAttack = g_NormalAttackSound->m_sound->CreateSourceVoice(m_pSourceNormalAttack);
	XAUDIO2_BUFFER buffer = g_NormalAttackSound->GetBuffer(false);
	m_pSourceNormalAttack->SubmitSourceBuffer(&buffer);
	m_pSourceWeaknessAttack = g_WeaknessAttackSound->m_sound->CreateSourceVoice(m_pSourceWeaknessAttack);
	XAUDIO2_BUFFER buffer2 = g_WeaknessAttackSound->GetBuffer(false);
	m_pSourceWeaknessAttack->SubmitSourceBuffer(&buffer2);
}

//キャラクターの基底クラスのデストラクタ
CFighter::~CFighter()
{
	//Hpクラスポインタの破棄
	if (m_pHpGage)
	{
		delete m_pHpGage;
		m_pHpGage = nullptr;
	}

	//サウンドの破棄
	if (m_pSourceNormalAttack)
	{
		m_pSourceNormalAttack->ExitLoop();
		m_pSourceNormalAttack->Stop();
		m_pSourceNormalAttack->DestroyVoice();
		m_pSourceNormalAttack = nullptr;
	}
	if (m_pSourceWeaknessAttack)
	{
		m_pSourceWeaknessAttack->ExitLoop();
		m_pSourceWeaknessAttack->Stop();
		m_pSourceWeaknessAttack->DestroyVoice();
		m_pSourceWeaknessAttack = nullptr;
	}

	//モデルの破棄
	if (m_pModel)
	{
		m_pModel = nullptr;
	}

	//エフェクトの破棄
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
		{
			m_pEffect[i] = nullptr;
		}
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
	Size.x = m_tSize.x * 1.5f;
	Size.y = m_tSize.y * 1.5f;
	Size.z = m_tSize.z * 1.5f;

	InSize.x = InSize.x * 1.5f;
	InSize.y = InSize.y * 1.5f;
	InSize.z = InSize.z * 1.5f;

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
	//相手の角数が自分と同じだったら
	if (pFighter->GetCornerCount() == m_nCornerCount)
	{
		//相手の攻撃力分体力を減らす
		m_fHp -= pFighter->GetAtk();
		//当たった判定をオンにする
		m_bIsHit = true;
		pFighter->m_bDamageLogDraw[0] = true;
		pFighter->m_bDamageLogDraw[1] = true;
		pFighter->m_bDamageLogDraw[2] = true;
		pFighter->m_tDamageLogPos.x = m_tPos.x;
		pFighter->m_tDamageLogPos.y = m_tPos.y - 1.0f;
		pFighter->m_tDamageLogPos.z = m_tPos.z;
	}
	else
	{
		//相手の攻撃力分体力を減らす
		m_fHp -= pFighter->GetAtk() * 0.3f;
		//当たった判定をオンにする
		m_bIsHit = true;
		pFighter->m_bDamageLogDraw[0] = true;
		pFighter->m_bDamageLogDraw[1] = true;
		pFighter->m_bDamageLogDraw[2] = false;
		pFighter->m_tDamageLogPos.x = m_tPos.x;
		pFighter->m_tDamageLogPos.y = m_tPos.y - 1.0f;
		pFighter->m_tDamageLogPos.z = m_tPos.z;
	}
}

void CFighter::DamageLogDraw(int nCornerCount,bool Type)
{
	SetRender2D();

	if (Type)
	{
		switch (nCornerCount)
		{
		case 3:
			m_pSprite->SetTexture(g_pDamageTex[(int)DamageDraw::NormalRed]);
			break;
		case 4:
			m_pSprite->SetTexture(g_pDamageTex[(int)DamageDraw::NormalBlue]);
			break;
		}
	}
	else
		m_pSprite->SetTexture(g_pDamageTex[(int)DamageDraw::NotQuite]);

	m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	m_pSprite->SetUVPos({ 0.0f, 0.0f });
	m_pSprite->SetUVScale({ 1.0f ,1.0f }); 

	DrawSetting({ m_tDamageLogPos.x ,m_tDamageLogPos.y + m_fDamageLogMoveY,m_tDamageLogPos.z + 0.2f }, { 5.0f,5.0f,5.0f }, m_pSprite);

	m_pSprite->Draw();

	m_pSprite->ReSetSprite();

}

//void CFighter::PlayDeathEffect(void)
//{
//	m_tEffect[(int)FighterEffect::Death].m_pEffect->Play({ m_tPos.x, m_tPos.y, m_tPos.z - m_tSize.z / 2 }, 60);
//	m_tEffect[(int)FighterEffect::Death].m_bEffectPlay = true;
//}

//味方クラスのコンストラクタ
CAlly::CAlly(int InCornerCount,bool IsStellaBuff)
	:CFighter(InCornerCount, IsStellaBuff)
{
	m_tSize.x = (float)MODEL_DEFAULTSIZE::Ally;
	m_tSize.y = (float)MODEL_DEFAULTSIZE::Ally;
	m_tSize.z = (float)MODEL_DEFAULTSIZE::Ally;

	//初期ステータス決め
	SettingStatus();
	//Hpのポインタの作成
	m_pHpGage = new CHpUI(m_fHp,CHpUI::Ally);
	//角数ごとに同期するポインタを変更
	switch (m_nCornerCount)
	{
	case 3:
		//モデル
		m_pModel = g_pAllyModel[(int)Ally::Ally3];
		//攻撃エフェクトのポインタ同期
		m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::SwordAtk]);
		break;
	case 4:
		//モデル
		m_pModel = g_pAllyModel[(int)Ally::Ally4];
		//攻撃エフェクトのポインタ同期
		m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::BowAtk]);
		break;
	}
	m_pEffect[(int)FighterEffect::Create] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::Create]);
	m_pEffect[(int)FighterEffect::Death] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::Death]);
	if (m_bStellaBuff)
	{
		m_pEffect[(int)FighterEffect::Aura] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::Aura]);
	}
	else
	{
		m_pEffect[(int)FighterEffect::Aura] = nullptr;
	}

}

//味方クラスのデストラクタ
CAlly::~CAlly()
{

}

//味方クラスの更新処理
void CAlly::Update(void)
{
	if (m_pEffect[(int)FighterEffect::Aura])
	{
		m_pEffect[(int)FighterEffect::Aura]->SetPos({ m_tPos.x,m_tPos.y,m_tPos.z });
		if (!m_pEffect[(int)FighterEffect::Aura]->IsPlay())
		{
			m_pEffect[(int)FighterEffect::Aura]->SetRotate({ 0.0f,0.0f,0.0f });
			m_pEffect[(int)FighterEffect::Aura]->SetSize({ 15.0f,10.0f,10.0f });
			m_pEffect[(int)FighterEffect::Aura]->Play(true);
		}
	}

	//モデルの再読み込みフラグが立っていたら読み込む
	if (m_bReLoadFlag)
	{
		if (m_pModel)m_pModel = nullptr;
		switch (m_nCornerCount)
		{
		case 3:
			//モデル
			m_pModel = g_pAllyModel[(int)Ally::Ally3];
			break;
		case 4:
			//モデル
			m_pModel = g_pAllyModel[(int)Ally::Ally4];
			break;
		}
		m_bReLoadFlag = false;
	}
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
		if (m_pEffect[i])
		{
			m_pEffect[i]->Update();
		}
	}
}

void CAlly::Draw(void)
{
	//ダメージログの描画
	if (m_bDamageLogDraw[0])
	{
		if (m_bDamageLogDraw[1])
		{
			m_fDamageLogMoveY = 0.0f;
			m_bDamageLogDraw[1] = false;
		}
		DamageLogDraw(m_nCornerCount, m_bDamageLogDraw[2]);
		if (m_fDamageLogMoveY <  5.0f)
		{
			m_fDamageLogMoveY += 5.0f / 120.0f;
		}
		else
		{
			m_bDamageLogDraw[0] = false;
			m_bDamageLogDraw[1] = false;
			m_bDamageLogDraw[2] = false;
		}
	}

	//体力ゲージの描画
	m_pHpGage->Draw(m_nCornerCount);

	//エフェクトの描画
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
		{
			m_pEffect[i]->Draw(true);
		}
	}

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
		Model::Material material = *m_pModel->GetMaterial(m_pModel->GetMesh(i)->materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		if (m_pModel) {
			m_pModel->Draw(i);
		}
	}
}
//生成更新処理
void CAlly::CreateUpdate(void)
{
	//生成アニメーション

	//生成エフェクト
	if (!m_pEffect[(int)FighterEffect::Create]->IsPlay() && !IsCreateEffectPlay && m_bFirstBattlePosSetting)
	{
		m_pEffect[(int)FighterEffect::Create]->SetPos({ m_tPos.x,m_tPos.y,m_tPos.z + m_tSize.z / 2 });
		m_pEffect[(int)FighterEffect::Create]->SetSize({ 15.0f,15.0f,15.0f });
		m_pEffect[(int)FighterEffect::Create]->SetRotate({ 0.0f,0.0f,0.0f });
		m_pEffect[(int)FighterEffect::Create]->Play();
		g_pSourceSummon[0]->FlushSourceBuffers();
		XAUDIO2_BUFFER buffer;
		buffer = g_SummonSound->GetBuffer(false);
		g_pSourceSummon[0]->SubmitSourceBuffer(&buffer);
		SetVolumeSE(g_pSourceSummon[0]);
		g_pSourceSummon[0]->Start();
		IsCreateEffectPlay = true;
	}
	//生成アニメーションが終わったら
	if (!m_pEffect[(int)FighterEffect::Create]->IsPlay() && IsCreateEffectPlay && m_bFirstBattlePosSetting)
	{
		SetStatus(St_Battle);
	}
}
//戦闘更新処理
void CAlly::BattleUpdate(void)
{
	//攻撃していたら
	if (m_bIsAttack)
	{
		//攻撃チャージが攻撃アニメーションの時間を引いたMAX数値分たまっているかどうか
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{
			//アニメーション処理

			//アニメーションの時間の加算
			m_fAtkAnimationTime++;
			//アニメーションの時間が最大以上になったら初期化する
			if (m_fAtkAnimationMaxTime <= m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
		//攻撃エフェクト
		if (!m_pEffect[(int)FighterEffect::Attack]->IsPlay() && !IsAttackEffectPlay)
		{
			m_pEffect[(int)FighterEffect::Attack]->SetPos({ m_tTargetPos.x ,m_tTargetPos.y,m_tTargetPos.z });
			if (m_nCornerCount == 3)
				m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,TORAD(180.0f),0.0f });
			else
				m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,0.0f,0.0f });
			m_pEffect[(int)FighterEffect::Attack]->SetSize({ 10.0f,10.0f,10.0f });
			m_pEffect[(int)FighterEffect::Attack]->Play();
			IsAttackEffectPlay = true;
		}
		//攻撃音
		if (!m_bTimeSoundStart)
		{
			if (m_bTypeAttack)
			{
				XAUDIO2_BUFFER buffer = g_NormalAttackSound->GetBuffer(false);
				m_pSourceNormalAttack->FlushSourceBuffers();
				m_pSourceNormalAttack->SubmitSourceBuffer(&buffer);
				if (m_pSourceNormalAttack)SetVolumeSE(m_pSourceNormalAttack);
				m_pSourceNormalAttack->Start();
				m_bTimeSoundStart = true;
			}
			else
			{
				XAUDIO2_BUFFER buffer = g_WeaknessAttackSound->GetBuffer(false);
				m_pSourceWeaknessAttack->FlushSourceBuffers();
				m_pSourceWeaknessAttack->SubmitSourceBuffer(&buffer);
				if (m_pSourceWeaknessAttack)SetVolumeSE(m_pSourceWeaknessAttack);
				m_pSourceWeaknessAttack->Start();
				m_bTimeSoundStart = true;
			}
		}
	}
	//エフェクトのRe再生処理
	if (IsAttackEffectPlay)
	{
		if (!m_pEffect[(int)FighterEffect::Attack]->IsPlay())
		{
			IsAttackEffectPlay = false;
		}
	}
	//攻撃音の再生時間
	if (m_bTimeSoundStart)
	{
		m_fTimeSound++;
	}
	if (m_fTimeSound>30)
	{
		m_pSourceNormalAttack->Stop();
		m_bTimeSoundStart = false;
		m_fTimeSound = 0;
	}

	//戦闘アニメーション(攻撃範囲内に敵がいたら)
	
	//移動アニメーション(移動していたら)
	if (m_bMoveUp)
	{
		m_tPos.y += 1.0f;
	}
	else
	{

	}

	//待機アニメーション(移動していなかったら)

	//HPUIの更新処理
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);
}
//死亡更新処理
void CAlly::DeathUpdate(void)
{
	//死亡アニメーション
	
	//死亡エフェクトの再生時間
	if (!m_pEffect[(int)FighterEffect::Death]->IsPlay() && !IsDeathEffectPlay)
	{
		m_pEffect[(int)FighterEffect::Death]->SetPos(m_tPos);
		m_pEffect[(int)FighterEffect::Death]->SetRotate({ 0.0f,0.0f,0.0f });
		m_pEffect[(int)FighterEffect::Death]->SetSize({ 20.0f,20.0f,20.0f });
		m_pEffect[(int)FighterEffect::Death]->Play();
		IsDeathEffectPlay = true;
	}


	//死亡アニメーションが終わったら
	if (!m_pEffect[(int)FighterEffect::Death]->IsPlay() && IsDeathEffectPlay)
	{
		SetStatus(St_Delete);
	}
}

//初期ステータス決め
void CAlly::SettingStatus(void)
{
	//角数別にカウント
	switch (m_nCornerCount)
	{
	default:
		//体力
		m_fHp = 2500.0f;
		//攻撃力
		m_fAtk = 150.0f;
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
		m_fHp = 2500.0f;
		//攻撃力
		m_fAtk = 150.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 30.0f + m_fAtkAnimationMaxTime;
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
		m_fHp = 2500.0f;
		//攻撃力
		m_fAtk = 150.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 30.0f + m_fAtkAnimationMaxTime;
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
	m_tSize.x = (float)MODEL_DEFAULTSIZE::Enemy;
	m_tSize.y = (float)MODEL_DEFAULTSIZE::Enemy;
	m_tSize.z = (float)MODEL_DEFAULTSIZE::Enemy;

	SettingStatus();
	m_pHpGage = new CHpUI(m_fHp,CHpUI::Enemy);

	switch (m_nCornerCount)
	{
	case 3:
		//モデル
		m_pModel = g_pEnemyModel[(int)Enemy::Enemy1];
		//攻撃エフェクトのポインタ同期
		m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::SwordAtk]);
		break;
	case 4:
		//モデル
		m_pModel = g_pEnemyModel[(int)Enemy::Enemy2];
		//攻撃エフェクトのポインタ同期
		m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::BowAtk]);
		break;
	}
	m_pEffect[(int)FighterEffect::Create] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::Create]);
	m_pEffect[(int)FighterEffect::Death] = new CEffectManager_sp(g_pCharacterEffects[(int)CharactersEffect::Death]);
}

CEnemy::~CEnemy()
{

}

void CEnemy::Update(void)
{
	//モデルの再読み込みフラグが立っていたら読み込む
	if (m_bReLoadFlag)
	{
		if (m_pModel)m_pModel = nullptr;
		switch (m_nCornerCount)
		{
		case 3:
			//モデル
			m_pModel = g_pEnemyModel[(int)Enemy::Enemy1];
			break;
		case 4:
			//モデル
			m_pModel = g_pEnemyModel[(int)Enemy::Enemy2];
			break;
		}
		m_bReLoadFlag = false;
	}

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

	//エフェクトの更新
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
			m_pEffect[i]->Update();
	}
}

void CEnemy::Draw(void)
{
	//ダメージログの描画
	if (m_bDamageLogDraw[0])
	{
		if (m_bDamageLogDraw[1])
		{
			m_fDamageLogMoveY = 0.0f;
			m_bDamageLogDraw[1] = false;
		}
		DamageLogDraw(m_nCornerCount, m_bDamageLogDraw[2]);
		if (m_fDamageLogMoveY < 5.0f)
		{
			m_fDamageLogMoveY += 5.0f / 120.0f;
		}
		else
		{
			m_bDamageLogDraw[0] = false;
			m_bDamageLogDraw[1] = false;
			m_bDamageLogDraw[2] = false;
		}
	}

	//体力ゲージの描画
	m_pHpGage->Draw(m_nCornerCount);

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
		Model::Material material = *m_pModel->GetMaterial(m_pModel->GetMesh(i)->materialID);
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
		{
			m_pEffect[i]->Draw(true);
		}
	}
}

void CEnemy::CreateUpdate(void)
{
	//生成アニメーション

	//生成エフェクト
	if (!m_pEffect[(int)FighterEffect::Create]->IsPlay() && !IsCreateEffectPlay && m_bFirstBattlePosSetting)
	{
		m_pEffect[(int)FighterEffect::Create]->SetPos(m_tPos);
		m_pEffect[(int)FighterEffect::Create]->SetSize({ 15.0f,15.0f,15.0f });
		m_pEffect[(int)FighterEffect::Create]->SetRotate({ 0.0f,0.0f,0.0f });
		m_pEffect[(int)FighterEffect::Create]->Play();
		g_pSourceSummon[1]->FlushSourceBuffers();
		XAUDIO2_BUFFER buffer;
		buffer = g_SummonSound->GetBuffer(false);
		g_pSourceSummon[1]->SubmitSourceBuffer(&buffer);
		SetVolumeSE(g_pSourceSummon[1]);
		g_pSourceSummon[1]->Start();
		IsCreateEffectPlay = true;
	}
	//生成アニメーションが終わったら
	if (!m_pEffect[(int)FighterEffect::Create]->IsPlay() && IsCreateEffectPlay && m_bFirstBattlePosSetting)
	{
		SetStatus(St_Battle);
	}
}

void CEnemy::BattleUpdate(void)
{
	if (m_bIsAttack)
	{
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{

			//アニメーション処理

			m_fAtkAnimationTime++;
			if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
		//攻撃エフェクト
		if (!m_pEffect[(int)FighterEffect::Attack]->IsPlay() && !IsAttackEffectPlay)
		{
			m_pEffect[(int)FighterEffect::Attack]->SetPos({ m_tTargetPos.x ,m_tTargetPos.y,m_tTargetPos.z });
			if (m_nCornerCount == 3)
				m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,0.0f,0.0f });
			else
				m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,0.0f,0.0f });
			m_pEffect[(int)FighterEffect::Attack]->SetSize({ 10.0f,10.0f,10.0f });
			m_pEffect[(int)FighterEffect::Attack]->Play();
			IsAttackEffectPlay = true;
		}
		//攻撃音
		if (!m_bTimeSoundStart)
		{
			if (m_bTypeAttack)
			{
				XAUDIO2_BUFFER buffer = g_NormalAttackSound->GetBuffer(false);
				m_pSourceNormalAttack->FlushSourceBuffers();
				m_pSourceNormalAttack->SubmitSourceBuffer(&buffer);
				if (m_pSourceNormalAttack)SetVolumeSE(m_pSourceNormalAttack);
				m_pSourceNormalAttack->SetVolume(1.0f);
				m_pSourceNormalAttack->Start();
				m_bTimeSoundStart = true;
			}
			else
			{
				XAUDIO2_BUFFER buffer = g_WeaknessAttackSound->GetBuffer(false);
				m_pSourceWeaknessAttack->FlushSourceBuffers();
				m_pSourceWeaknessAttack->SubmitSourceBuffer(&buffer);
				if (m_pSourceWeaknessAttack)SetVolumeSE(m_pSourceWeaknessAttack);
				m_pSourceWeaknessAttack->Start();
				m_bTimeSoundStart = true;
			}
		}
	}

	//エフェクトのRe再生処理
	if (IsAttackEffectPlay)
	{
		if (!m_pEffect[(int)FighterEffect::Attack]->IsPlay())
		{
			IsAttackEffectPlay = false;
		}
	}
	//攻撃音の再生時間
	if(m_bTimeSoundStart)
	{
		m_fTimeSound++;
	}
	if(m_fTimeSound > 30)
	{
		m_pSourceNormalAttack->Stop();
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

	//死亡エフェクト
	if (!m_pEffect[(int)FighterEffect::Death]->IsPlay() && !IsDeathEffectPlay)
	{
		m_pEffect[(int)FighterEffect::Death]->SetPos(m_tPos);
		m_pEffect[(int)FighterEffect::Death]->SetRotate({ 0.0f,0.0f,0.0f });
		m_pEffect[(int)FighterEffect::Death]->SetSize({ 20.0f,20.0f,20.0f });

		m_pEffect[(int)FighterEffect::Death]->Play();
		IsDeathEffectPlay = true;
	}

	//死亡アニメーションが終わったら
	if (!m_pEffect[(int)FighterEffect::Death]->IsPlay() && IsDeathEffectPlay)
	{
		SetStatus(St_Delete);
	}
}

void CEnemy::SettingStatus(void)
{
	switch (m_nCornerCount)
	{
	default:
		m_fHp = 2500.0f;
		m_fAtk = 150.0f;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 30.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Triangle:
		m_fHp = 2500.0f;
		m_fAtk = 150.0f;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 30.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	case Square:
		m_fHp = 2500.0f;
		m_fAtk = 150.0f;
		m_fAtkAnimationMaxTime = 0.0f;
		m_fAtkChargeMax = 30.0f + m_fAtkAnimationMaxTime;
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);			//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);			//キャラクターの中心からの縦の攻撃当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);	//キャラクターの中心からの横の索敵当たり判定
		break;
	}
}

CLeader::CLeader(float InSize, DirectX::XMFLOAT3 FirstPos, int InTextureNumber, bool SubModelCreate)
	:m_tStatus(St_Create)
	, m_pModel(nullptr)
	, m_tPos()
	, m_tSize()
	, m_pSubModel(nullptr)
	, m_tSubPos()
	, m_tSubSize()
	, m_fHp(15000.0f)
	, m_fMaxHp(m_fHp)
	, m_nTextureNumber(InTextureNumber)
	, m_pHpGage(nullptr)
	, m_bSubModelCreate(SubModelCreate)
	, m_bAnimationPlay(false)
{
	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		m_pModel = g_pLeaderModel[(int)Leader::Linie];
		m_pHpGage = new CHpUI(m_fHp, CHpUI::Player);
		break;
	case 1://ボス
		m_pModel = g_pLeaderModel[1];
		if (m_bSubModelCreate)
		{
			m_pSubModel = g_pLeaderModel[2];
		}
		m_pHpGage = new CHpUI(m_fHp, CHpUI::Bos);
		break;

	}
	//メインモデルのサイズと位置
	if (m_pModel)
	{
		switch (m_nTextureNumber)
		{
		case 0:
			m_tPos.x = FirstPos.x;
			m_tPos.y = FirstPos.y;
			m_tPos.z = FirstPos.z - 5.0f;
			m_tSize.x = InSize;
			m_tSize.y = InSize;
			m_tSize.z = InSize;
			break;
		case 1:
			m_tPos.x = FirstPos.x - 12.0f;
			m_tPos.y = FirstPos.y;
			m_tPos.z = FirstPos.z - 5.0f;
			m_tSize.x = InSize;
			m_tSize.y = InSize;
			m_tSize.z = InSize;
			break;
		}
	}
	if (m_pSubModel)
	{
		m_tSubPos.x = FirstPos.x - 15.0f;
		m_tSubPos.y = FirstPos.y + 0.5f;
		m_tSubPos.z = FirstPos.z + 5.0f;
		m_tSubSize.x = InSize;
		m_tSubSize.y = InSize;
		m_tSubSize.z = InSize;
	}
}
CLeader::~CLeader()
{
	if (m_pHpGage)
	{
		delete m_pHpGage;
		m_pHpGage = nullptr;
	}
	if (m_pModel)
	{
		m_pModel = nullptr;
	}
	if (m_pSubModel)
	{
		m_pSubModel = nullptr;
	}
}

void CLeader::Update(bool IsStart, bool IsEnd)
{
	if (m_bReLoadFlag)
	{
		switch (m_nTextureNumber)
		{
		case 0://プレイヤー
			m_pModel = g_pLeaderModel[(int)Leader::Linie];
			m_pHpGage = new CHpUI(m_fHp, CHpUI::Player);
			break;
		case 1://ボス
			m_pModel = g_pLeaderModel[1];
			if (m_bSubModelCreate)
			{
				m_pSubModel = g_pLeaderModel[2];
			}
			m_pHpGage = new CHpUI(m_fHp, CHpUI::Bos);
			break;

		}
		m_bReLoadFlag = false;
	}
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);

	switch (m_tStatus)
	{
	case St_Create:CreateUpdate();
		break;
	case St_Battle:BattleUpdate(IsStart, IsEnd);
		break;
	case St_Death:DeathUpdate();
		break;
	}
}

void CLeader::Draw(int StageNum)
{
	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		if (m_pModel)
		{
			SetRender3D();
			DirectX::XMFLOAT4X4 wvp[3];
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x , m_tPos.y, m_tPos.z, 0.0f));
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

			m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
			m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

			for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
			{
				Model::Mesh tMesh = * m_pModel->GetMesh(i);
				Model::Material material = *m_pModel->GetMaterial(tMesh.materialID);
				material.ambient.x = 0.85f; // x (r) 
				material.ambient.y = 0.85f; // y (g) 
				material.ambient.z = 0.85f; // z (b) 
				ShaderList::SetMaterial(material);

				// ボーンの情報をシェーダーに送る
				//DirectX::XMFLOAT4X4 bones[200];

				//for (int i = 0; i < tMesh.bones.size(); ++i) {
				//	DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
				//		tMesh.bones[i].invOffset * m_pModel->GetBoneMatrix(tMesh.bones[i].nodeIndex)
				//	));
				//}

				DirectX::XMFLOAT4X4 bones[200];
				for (int j = 0; j < tMesh.bones.size(); ++j)
				{
					DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
						tMesh.bones[j].invOffset * m_pModel->GetBoneMatrix(tMesh.bones[j].nodeIndex)
					));
					ShaderList::SetBones(bones);
				}


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
			DirectX::XMMATRIX T, S, R;
			
			switch (StageNum)
			{
			case 0:
				T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x - 8.0f, m_tPos.y - 10.0f, m_tPos.z, 0.0f));
				//拡大縮小行列(Scaling)
				S = DirectX::XMMatrixScaling(0.4f, 0.4f, 0.4f);
				//回転行列(Rotation)
				R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
				break;
			case 1:
				 T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x - 2.0f, m_tPos.y - 17.0f, m_tPos.z + 6.0f, 0.0f));
				//拡大縮小行列(Scaling)
				 S = DirectX::XMMatrixScaling(0.4f, 0.4f, 0.4f);
				//回転行列(Rotation)
				 R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
				break;
			case 2:
				 T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x + 2.0f, m_tPos.y - 5.0f, m_tPos.z, 0.0f));
				//拡大縮小行列(Scaling)
				 S = DirectX::XMMatrixScaling(0.35f, 0.35f, 0.35f);
				//回転行列(Rotation)
				 R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(1.0f), DirectX::XMConvertToRadians(170.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
				break;
			}
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
				Model::Material material = *g_pBosCar->GetMaterial(m_pModel->GetMesh(i)->materialID);
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
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x , m_tPos.y, m_tPos.z, 0.0f));
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
				Model::Material material = *m_pModel->GetMaterial(m_pModel->GetMesh(i)->materialID);
				material.ambient.x = 0.85f; // x (r) 
				material.ambient.y = 0.85f; // y (g) 
				material.ambient.z = 0.85f; // z (b) 
				ShaderList::SetMaterial(material);

				if (m_pModel) {
					m_pModel->Draw(i);
				}
			}
		}
		if (m_pSubModel)
		{
			SetRender3D();
			DirectX::XMFLOAT4X4 wvp[3];
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tSubPos.x, m_tSubPos.y, m_tSubPos.z , 0.0f));
			//拡大縮小行列(Scaling)
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSubSize.x, m_tSubSize.y, m_tSubSize.z);
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

			m_pSubModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
			m_pSubModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

			for (int i = 0; i < m_pSubModel->GetMeshNum(); ++i)
			{
				Model::Material material = *m_pSubModel->GetMaterial(m_pSubModel->GetMesh(i)->materialID);
				material.ambient.x = 0.85f; // x (r) 
				material.ambient.y = 0.85f; // y (g) 
				material.ambient.z = 0.85f; // z (b) 
				ShaderList::SetMaterial(material);

				if (m_pSubModel) {
					m_pSubModel->Draw(i);
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
	//if (IsKeyPress('K'))Y -= 1.0f;
	//if (IsKeyPress('L'))Y += 1.0f;
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(Y), DirectX::XMConvertToRadians(0.0f), 0.0f));
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
	pFighter->m_bDamageLogDraw[0] = true;
	pFighter->m_bDamageLogDraw[1] = true;
	pFighter->m_bDamageLogDraw[2] = false;
	pFighter->m_tDamageLogPos.x = m_tPos.x;
	pFighter->m_tDamageLogPos.y = m_tPos.y - 1.0f;
	pFighter->m_tDamageLogPos.z = m_tPos.z;
}

void CLeader::CreateUpdate(void)
{
	//生成アニメーション

	//生成アニメーションが終わったら
	SetStatus(St_Battle);

}

void CLeader::BattleUpdate(bool IsStart, bool IsEnd)
{
	//待機アニメーション
	switch (m_nTextureNumber)
	{
	case 0:
		//筆を離す
		if (IsStart)
		{
			if (m_pModel->IsAnimePlay(m_pModel->GetAnimePlayNo()))
			{
				m_pModel->Step(-0.01f);
			}
			else if (!m_bAnimationPlay)
			{
				g_pSourceWandonoff->Stop();
				m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
				m_pModel->PlayAnime(g_pLeader_Anima, false);
				m_pModel->SetAnimeTime(m_pModel->GetAnimePlayNo(), m_pModel->GetPlayAnimeInfo()->totalTime);
				g_pSourceWandonoff->FlushSourceBuffers();
				XAUDIO2_BUFFER buffer;
				buffer = g_wandonoffSound->GetBuffer(false);
				g_pSourceWandonoff->SubmitSourceBuffer(&buffer);
				SetVolumeSE(g_pSourceWandonoff);
				g_pSourceWandonoff->Start();
				m_bAnimationPlay = true;
			}
		}
		//筆を持つ
		if (IsEnd)
		{
			if (m_pModel->IsAnimePlay(m_pModel->GetAnimePlayNo()))
			{
				m_pModel->Step(0.01f);
			}
			else if(!m_bAnimationPlay)
			{
				g_pSourceWandonoff->Stop();
				m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
				m_pModel->PlayAnime(g_pLeader_Anima, false);
				m_pModel->SetAnimeTime(m_pModel->GetAnimePlayNo(), 0.0f);
				g_pSourceWandonoff->FlushSourceBuffers();
				XAUDIO2_BUFFER buffer;
				buffer = g_wandonoffSound->GetBuffer(false);
				g_pSourceWandonoff->SubmitSourceBuffer(&buffer);
				SetVolumeSE(g_pSourceWandonoff);
				g_pSourceWandonoff->Start();
				m_bAnimationPlay = true;
			}
		}
		break;
	}
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
	:m_fNowHp(FullHp)
	,m_fFullHp(FullHp)
	,m_tUIPos()
	,m_tUIScale()
	,m_tNumber(Number)
	,m_fAnchorPoint()

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
		m_tUIScale.y = 4.0f;
		m_tUIScale.z = 1.0f;
		m_fAnchorPoint = 8.0f;
		break;
	}
}

CHpUI::~CHpUI()
{
	if (m_pSprite)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
}

void CHpUI::Update(float InHp,DirectX::XMFLOAT3 InPos, float InSizeY)
{
int HpRatio = 0;
	m_fNowHp = InHp;

	switch (m_tNumber)
	{
	case CHpUI::Ally:
		HpRatio =  4.0f;
		m_tUIScale.x = HpRatio;
		m_tUIPos.x = InPos.x - m_tUIScale.x + (m_fNowHp / m_fFullHp) * m_tUIScale.x;
		m_tUIPos.y = InPos.y + InSizeY - 1.0f;
		m_tUIPos.z = InPos.z;
		//HpRatio = (m_fNowHp / m_fFullHp) * 4.0f;
		break;
	case CHpUI::Enemy:
		HpRatio =  4.0f;
		m_tUIScale.x = HpRatio;
		m_tUIPos.x = InPos.x + m_tUIScale.x - (m_fNowHp / m_fFullHp) * m_tUIScale.x;
		m_tUIPos.y = InPos.y + InSizeY - 1.0f;
		m_tUIPos.z = InPos.z;
		//HpRatio = (m_fNowHp / m_fFullHp) * 4.0f;
		break;
	case CHpUI::Bos:
		HpRatio =  102.0f;
		m_tUIScale.x = HpRatio;
		m_tUIPos.x = ( (InPos.x + 20.6f) + m_tUIScale.x - (m_fNowHp / m_fFullHp) * m_tUIScale.x) - 47.0f;
		//m_tUIPos.x = (InPos.x + 8.6f) - 44.0f;
		m_tUIPos.y = -5.0;
		m_tUIPos.z = InPos.z;
		//HpRatio = (m_fNowHp / m_fFullHp) * 106.0f;
		break;
	case CHpUI::Player:
		HpRatio =  102.0f;
		m_tUIScale.x = HpRatio;
		m_tUIPos.x = ((InPos.x - 8.6f) + (m_tUIScale.x - (m_fNowHp / m_fFullHp) * m_tUIScale.x) * -1) + 47.0f;
		//m_tUIPos.x = (InPos.x - 8.6f) + 44.0f;
		m_tUIPos.y = -5.0;
		m_tUIPos.z = InPos.z;
		//HpRatio = (m_fNowHp / m_fFullHp) * 106.0f;
		break;
	}

}

void CHpUI::Draw(int nCornerCount)
{
	SetRender2D();
	switch (m_tNumber)
	{
	case CHpUI::Ally:
		if (m_fFullHp != m_fNowHp)
		{
			//ゲージの描画
			if (nCornerCount == 3)
			{
				m_pSprite->SetTexture(g_pHpGageTex[(int)HpTexture::Ally1]);
			}
			else if (nCornerCount == 4)
			{
				m_pSprite->SetTexture(g_pHpGageTex[(int)HpTexture::Ally2]);
			}
			m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

			m_pSprite->SetUVPos({ 1.0f - (m_fNowHp / m_fFullHp),0.0f });
			m_pSprite->SetUVScale({ 2.0f ,1.0f });

			DrawSetting(/*{ m_tUIPos.x ,m_tUIPos.y,m_tUIPos.z + 0.1f }*/m_tUIPos, m_tUIScale, m_pSprite);

			m_pSprite->Draw();

			m_pSprite->ReSetSprite();
		}
		break;
	case CHpUI::Enemy:
		if (m_fFullHp != m_fNowHp)
		{
			//ゲージの描画
			if (nCornerCount == 3)
			{
				m_pSprite->SetTexture(g_pHpGageTex[(int)HpTexture::Enemy1]);
			}
			else if (nCornerCount == 4)
			{
				m_pSprite->SetTexture(g_pHpGageTex[(int)HpTexture::Enemy2]);
			}

			m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

			m_pSprite->SetUVPos({ 1.0f - (m_fNowHp / m_fFullHp),0.0f });
			m_pSprite->SetUVScale({ 2.0f ,1.0f });

			//DrawSetting({ m_tUIPos.x - (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);
			DrawSetting(/*{ m_tUIPos.x,m_tUIPos.y,m_tUIPos.z + 0.1f }*/m_tUIPos, m_tUIScale, m_pSprite);

			m_pSprite->Draw();

			m_pSprite->ReSetSprite();
		}
		break;
	case CHpUI::Bos:
		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[(int)HpTexture::Boss]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		m_pSprite->SetUVPos({ 1.0f - (m_fNowHp / m_fFullHp),0.0f });
		m_pSprite->SetUVScale({ 1.0f ,1.0f });

		//DrawSetting({ m_tUIPos.x + (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y - 5.0f ,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);
		DrawSetting({ m_tUIPos.x,m_tUIPos.y - 5.0f ,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		//ベース(Top)の描画
		m_pSprite->SetTexture(g_pHpGageTex[(int)HpTexture::Frame]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		m_pSprite->SetUVPos({ 0.0f,0.0f });
		m_pSprite->SetUVScale({ 1.0f ,1.0f });

		DrawSetting({ 0.0f ,-5.0f,0.0f }, { 216.0f,21.0f,10.0f }, m_pSprite);

		m_pSprite->Draw();

		m_pSprite->ReSetSprite();

		break;

	case CHpUI::Player:
		//ゲージの描画
		m_pSprite->SetTexture(g_pHpGageTex[(int)HpTexture::Linie]);

		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		m_pSprite->SetUVPos({ -1.0f + (m_fNowHp / m_fFullHp),0.0f });
		//m_pSprite->SetUVPos({ 0.0f ,0.0f });
		m_pSprite->SetUVScale({ 1.0f ,1.0f });

		//DrawSetting({ m_tUIPos.x - (m_fAnchorPoint - (m_tUIScale.x / 2)),m_tUIPos.y - 5.0f,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);
		DrawSetting({ m_tUIPos.x ,m_tUIPos.y - 5.0f,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);

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