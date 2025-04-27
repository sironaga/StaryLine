/*==============================================================
*　
*　File: CharacterManager.cpp
*　
*　編集者：宇留野陸斗｜[作成][管理]
*　
*　編集者：中嶋飛賀　｜[ 音 ]
*　
================================================================*/

#include "CharacterManager.h"
#include "DirectXTex/TextureLoad.h"
#include "FieldVertex.h"
#include "Main.h"
#include "Geometory.h"
#include "ShaderList.h"
#include "Input.h"
#include "StageSelect.h"
#include "Pause.h"

//キャラクターのマネージャークラスのインスタンスの初期化
CCharacterManager* CCharacterManager::m_pInstance = nullptr;

/*
* @brief インスタンスの作成
*/
void CCharacterManager::CreateInstance()
{
	//インスタンスが存在しない場合
	if (m_pInstance == nullptr)
	{
		//インスタンスのポインタを作成
		m_pInstance = new CCharacterManager();
	}
}

/*
* @brief インスタンスの取得
*/
CCharacterManager* CCharacterManager::GetInstance()
{
	//インスタンスが存在する場合
	if (m_pInstance)
	{
		//インスタンスのポインタを返す
		return m_pInstance;
	}
	//インスタンスが存在しない場合
	return nullptr;
}

//事前読み込み用関数
void CCharacterManager::InitCharacterTexture(StageType StageType)
{
	/*＝＝＝味方兵＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/

	//--モデルポインタの作成
	for (int i = 0; i < (int)Ally::MAX; i++)
	{
		for (int j = 0; j < (int)CharacterAnimation::MAX; j++)
		{
			m_pAllyModel[j][i] = new Model();
		}
	}
	
	//--モデルの読み込み
	//味方兵三角タイプのモデル
	m_pAllyModel[(int)CharacterAnimation::None][(int)Ally::Ally3]
		->Load(MODEL_PASS("Character/Triangle/Char_Summonstar_Triangle.fbx"), 0.05f, Model::None);
	//味方兵三角タイプの歩きアニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Walk][(int)Ally::Ally3]
		->Load(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Walk.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Walk][(int)Ally::Ally3] = m_pAllyModel[(int)CharacterAnimation::Walk][(int)Ally::Ally3]
		->AddAnimation(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Walk.fbx"));
	//味方兵三角タイプの優位攻撃アニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Attack_Advantage][(int)Ally::Ally3]
		->Load(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Attack_Ground.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Attack_Advantage][(int)Ally::Ally3] = m_pAllyModel[(int)CharacterAnimation::Attack_Advantage][(int)Ally::Ally3]
		->AddAnimation(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Attack_Ground.fbx"));
	//味方兵三角タイプの劣位攻撃アニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Attack_Disadvantage][(int)Ally::Ally3]
		->Load(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Attack_Ground.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Attack_Disadvantage][(int)Ally::Ally3] = m_pAllyModel[(int)CharacterAnimation::Attack_Disadvantage][(int)Ally::Ally3]
		->AddAnimation(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Attack_Ground.fbx"));
	//味方兵三角タイプの死亡アニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Death][(int)Ally::Ally3]
		->Load(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Death.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Death][(int)Ally::Ally3] = m_pAllyModel[(int)CharacterAnimation::Death][(int)Ally::Ally3]
		->AddAnimation(MODEL_PASS("Character/Triangle/Anim_Summonstar_Triangle_Death.fbx"));
	//味方兵四角タイプのモデル
	m_pAllyModel[(int)CharacterAnimation::None][(int)Ally::Ally4]
		->Load(MODEL_PASS("Character/Square/Char_Summonstar_Square.fbx"), 0.05f, Model::None);
	//味方兵四角タイプの歩きアニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Walk][(int)Ally::Ally4]
		->Load(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Walk.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Walk][(int)Ally::Ally4] = m_pAllyModel[(int)CharacterAnimation::Walk][(int)Ally::Ally4]
		->AddAnimation(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Walk.fbx"));
	//味方兵四角タイプの優位攻撃アニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Attack_Advantage][(int)Ally::Ally4]
		->Load(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Walk.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Attack_Advantage][(int)Ally::Ally4] = m_pAllyModel[(int)CharacterAnimation::Attack_Advantage][(int)Ally::Ally4]
		->AddAnimation(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Walk.fbx"));
	//味方兵四角タイプの劣位攻撃アニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Attack_Disadvantage][(int)Ally::Ally4]
		->Load(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Attack_Ground.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Attack_Disadvantage][(int)Ally::Ally4] = m_pAllyModel[(int)CharacterAnimation::Attack_Disadvantage][(int)Ally::Ally4]
		->AddAnimation(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Attack_Ground.fbx"));
	//味方兵四角タイプの死亡アニメーションモデル
	m_pAllyModel[(int)CharacterAnimation::Death][(int)Ally::Ally4]
		->Load(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Death.fbx"), 0.05f, Model::None);
	m_pAlly_Anima[(int)CharacterAnimation::Death][(int)Ally::Ally4] = m_pAllyModel[(int)CharacterAnimation::Death][(int)Ally::Ally4]
		->AddAnimation(MODEL_PASS("Character/Square/Anim_Summonstar_Square_Death.fbx"));

	//--Hpゲージのテクスチャ
	//味方兵三角タイプのHpゲージのポインタの作成
	m_pHpGageTex[(int)HpTexture::Ally3] = new Texture();
	//味方兵三角タイプのHpゲージのテクスチャ読み込み
	m_pHpGageTex[(int)HpTexture::Ally3]->Create(TEX_PASS("HpGage/Battle_HP_Gage_LinieS.png"));
	//味方兵四角タイプのHpゲージのポインタの作成
	m_pHpGageTex[(int)HpTexture::Ally4] = new Texture();
	//味方兵四角タイプのHpゲージのテクスチャ読み込み
	m_pHpGageTex[(int)HpTexture::Ally4]->Create(TEX_PASS("HpGage/Battle_HP_Gage_LinieS.png"));


	/*＝＝＝プレイヤー＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/

	//Hpゲージのポインタの作成
	m_pHpGageTex[(int)HpTexture::Linie] = new Texture();
	//Hpゲージのテクスチャの読み込み
	m_pHpGageTex[(int)HpTexture::Linie]->Create(TEX_PASS("HpGage/Battle_HP_Gage_Linie.png"));

	//モデルのポインタの作成
	for (int i = 0; i < (int)LinieAnimation::MAX; i++)
	{
		m_pLinieModel[i] = new Model();
	}

	//待機アニメーションモデルの読み込み
	m_pLinieModel[(int)LinieAnimation::Idle]
		->Load(MODEL_PASS("Leader/Linie/Anim_Main_Linie_Idle.fbx"), 1.0f, Model::None);
	m_pLinie_Anima[(int)LinieAnimation::Idle] = m_pLinieModel[(int)LinieAnimation::Idle]
		->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Main_Linie_Idle.fbx"));
	//召喚アニメーションモデルの読み込み
	m_pLinieModel[(int)LinieAnimation::Summon]
		->Load(MODEL_PASS("Leader/Linie/Anim_Main_Linie_Summon.fbx"), 1.0f, Model::None);
	m_pLinie_Anima[(int)LinieAnimation::Summon] = m_pLinieModel[(int)LinieAnimation::Summon]
		->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Main_Linie_Summon.fbx"));
	//被弾アニメーションモデルの読み込み
	m_pLinieModel[(int)LinieAnimation::Damage]
		->Load(MODEL_PASS("Leader/Linie/Anim_Linie_Damage.fbx"), 1.0f, Model::None);
	m_pLinie_Anima[(int)LinieAnimation::Damage] = m_pLinieModel[(int)LinieAnimation::Damage]
		->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Linie_Damage.fbx"));
	//勝利アニメーションモデルの読み込み
	m_pLinieModel[(int)LinieAnimation::Win]
		->Load(MODEL_PASS("Leader/Linie/Anim_Main_Linie_Win.fbx"), 1.0f, Model::None);
	m_pLinie_Anima[(int)LinieAnimation::Win] = m_pLinieModel[(int)LinieAnimation::Win]
		->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Main_Linie_Win.fbx"));

	/*＝＝＝敵兵とボス＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/

	//ステージ別に読み込みを変える
	switch (StageType.StageMainNumber)
	{
	case (int)E_SELECT_STAGETYPE::GRASSLAND://草原
		
		/*＝＝＝＝＝＝敵兵＝＝＝＝＝＝*/
		//モデルポインタの作成
		for (int i = 0; i < (int)Enemy::MAX; i++)
		{
			m_pEnemyModel[i] = new Model();
		}
		//敵キャラクター地上タイプのモデル
		m_pEnemyModel[(int)Enemy::Enemy1]->Load(MODEL_PASS("Character/Idiot1/Stage1/Stage1_Idiot1.fbx"), 0.05f, Model::None);
		m_pEnemy_Anima[(int)CharacterAnimation::None][(int)Enemy::Enemy1] = NULL;
		
		//敵キャラクター飛行タイプのモデル
		m_pEnemyModel[(int)Enemy::Enemy2]->Load(MODEL_PASS("Character/Idiot2/Stage1/Stage1_Idiot2.fbx"), 0.05f, Model::None);
		m_pEnemy_Anima[(int)CharacterAnimation::None][(int)Enemy::Enemy2] = NULL;

		//---Hpゲージ
		//敵キャラクター地上タイプのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Enemy1] = new Texture();
		//敵キャラクター地上タイプのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Enemy1]->Create(TEX_PASS("HpGage/Battle_HP_Gage_QrackerS.png"));
		//敵キャラクター飛行タイプのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Enemy2] = new Texture();
		//敵キャラクター飛行タイプのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Enemy2]->Create(TEX_PASS("HpGage/Battle_HP_Gage_QrackerS.png"));

		/*＝＝＝＝＝＝ボス＝＝＝＝＝＝*/
		//--Hpゲージ
		//ボスのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Boss] = new Texture();
		//ボスのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Boss]->Create(TEX_PASS("HpGage/Battle_HP_Gage_Qracker.png"));
		//ボスのHpゲージのフレームのポインタの作成
		m_pHpGageTex[(int)HpTexture::Frame] = new Texture();
		//ボスのHpゲージのフレームのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Frame]->Create(TEX_PASS("HpGage/Battle_HP_top_Qracker.png"));
		//--モデル
		//モデルポインタの作成
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Quracker] = new Model();
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Quracker] = new Model();
		m_pBossModel[(int)BossAnimation::Idle][1] = nullptr;
		m_pBossModel[(int)BossAnimation::Damage][1] = nullptr;

		//待機アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Quracker]
			->Load(MODEL_PASS("Leader/Qracker/Anim_Boss01_Qracker_Idle.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Idle][(int)Boss::Quracker] = m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Quracker]
			->AddAnimation(MODEL_PASS("Leader/Qracker/Anim_Boss01_Qracker_Idle.fbx"));
		//被弾アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Quracker]
			->Load(MODEL_PASS("Leader/Qracker/Anim_Boss01_Qracker_Damage.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Damage][(int)Boss::Quracker] = m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Quracker]
			->AddAnimation(MODEL_PASS("Leader/Qracker/Anim_Boss01_Qracker_Damage.fbx"));
		
		//--車
		//ステージのサブ番号によって車のモデルを変更
		switch (StageType.StageSubNumber)
		{
		case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE1://草原ステージ1
			//ボスの車(初期段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss01_Chariot.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE2://草原ステージ2
			//ボスの車(中間段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss01_Chariot.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE3://草原ステージ3
			//ボスの車(最終段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss01_Chariot.fbx"), 1.0f, Model::None);
			break;
		}
		break;
	case (int)E_SELECT_STAGETYPE::DESERT://砂漠

		/*＝＝＝＝＝＝敵兵＝＝＝＝＝＝*/
		//モデルポインタの作成
		for (int i = 0; i < (int)Enemy::MAX; i++)
		{
			m_pEnemyModel[i] = new Model();
		}
		//敵キャラクター地上タイプのモデルのテクスチャ読み込み
		m_pEnemyModel[(int)Enemy::Enemy1]->Load(MODEL_PASS("Character/Idiot1/Stage2/Stage2_Idiot1.fbx"), 0.05f, Model::None);
		m_pEnemy_Anima[(int)CharacterAnimation::None][(int)Enemy::Enemy1] = NULL;
		//敵キャラクター飛行タイプのモデルのテクスチャ読み込み
		m_pEnemyModel[(int)Enemy::Enemy2]->Load(MODEL_PASS("Character/Idiot2/Stage2/Stage2_Idiot2.fbx"), 0.05f, Model::None);
		m_pEnemy_Anima[(int)CharacterAnimation::None][(int)Enemy::Enemy2] = NULL;

		//---Hpゲージ
		//敵キャラクター地上タイプのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Enemy1] = new Texture();
		//敵キャラクター地上タイプのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Enemy1]->Create(TEX_PASS("HpGage/Battle_HP_Gage_NugarS.png"));
		//敵キャラクター飛行タイプのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Enemy2] = new Texture();
		//敵キャラクター飛行タイプのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Enemy2]->Create(TEX_PASS("HpGage/Battle_HP_Gage_NugarS.png"));

		/*＝＝＝＝＝＝ボス＝＝＝＝＝＝*/
		//--Hpゲージ
		//ボスのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Boss] = new Texture();
		//ボスのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Boss]->Create(TEX_PASS("HpGage/Battle_HP_Gage_Nugar.png"));
		//ボスのHpゲージのフレームのポインタの作成
		m_pHpGageTex[(int)HpTexture::Frame] = new Texture();
		
		//--モデル
		//ボスモデルのポインタの作成
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Nugar] = new Model();
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Nugar] = new Model();
		m_pBossModel[(int)BossAnimation::Idle][1] = nullptr;
		m_pBossModel[(int)BossAnimation::Damage][1] = nullptr;

		//待機アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Nugar]
			->Load(MODEL_PASS("Leader/Nugar/Anim_Boss02_Nugar_Idle.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Idle][(int)Boss::Nugar] = m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Nugar]
			->AddAnimation(MODEL_PASS("Leader/Nugar/Anim_Boss02_Nugar_Idle.fbx"));
		//被弾アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Nugar]
			->Load(MODEL_PASS("Leader/Nugar/Anim_Boss02_Nugar_Damage.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Damage][(int)Boss::Nugar] = m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Nugar]
			->AddAnimation(MODEL_PASS("Leader/Nugar/Anim_Boss02_Nugar_Damage.fbx"));

		//--車
		//ステージのサブ番号によって車のモデルを変更
		switch (StageType.StageSubNumber)
		{
		case (int)E_SELECT_STAGENUMBER::DESERT_STAGE1://砂漠ステージ1
			//ボスの車(初期段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss02_Car.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::DESERT_STAGE2://砂漠ステージ2
			//ボスの車(中間段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss02_Car.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::DESERT_STAGE3://砂漠ステージ3
			//ボスの車(最終段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss02_Car.fbx"), 1.0f, Model::None);
			break;
		}
		break;
	case (int)E_SELECT_STAGETYPE::SNOWFIELD://雪原
		
		/*＝＝＝＝＝＝敵兵＝＝＝＝＝＝*/
		//モデルポインタの作成
		for (int i = 0; i < (int)Enemy::MAX; i++)
		{
			m_pEnemyModel[i] = new Model();
		}
		//敵キャラクター地上タイプのモデルのテクスチャ読み込み
		m_pEnemyModel[(int)Enemy::Enemy1]->Load(MODEL_PASS("Character/Idiot1/Stage3/Stage3_Idiot1.fbx"), 0.05f, Model::None);
		m_pEnemy_Anima[(int)CharacterAnimation::None][(int)Enemy::Enemy1] = NULL;
		//敵キャラクター飛行タイプのモデルのテクスチャ読み込み
		m_pEnemyModel[(int)Enemy::Enemy2]->Load(MODEL_PASS("Character/Idiot1/Stage3/Stage3_Idiot1.fbx"), 0.05f, Model::None);
		m_pEnemy_Anima[(int)CharacterAnimation::None][(int)Enemy::Enemy1] = NULL;

		//---Hpゲージ
		//敵キャラクター地上タイプのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Enemy1] = new Texture();
		//敵キャラクター地上タイプのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Enemy1]->Create(TEX_PASS("HpGage/Battle_HP_Gage_KanneleS.png"));
		//敵キャラクター飛行タイプのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Enemy2] = new Texture();
		//敵キャラクター飛行タイプのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Enemy2]->Create(TEX_PASS("HpGage/Battle_HP_Gage_BoldowS.png"));

		/*＝＝＝＝＝＝ボス＝＝＝＝＝＝*/
		//--Hpゲージ
		//ボスのHpゲージのポインタの作成
		m_pHpGageTex[(int)HpTexture::Boss] = new Texture();
		//ボスのHpゲージのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Boss]->Create(TEX_PASS("HpGage/Battle_HP_Gage_KanneleBoldow.png"));
		//ボスのHpゲージのフレームのポインタの作成
		m_pHpGageTex[(int)HpTexture::Frame] = new Texture();
		//ボスのHpゲージのフレームのテクスチャ読み込み
		m_pHpGageTex[(int)HpTexture::Frame]->Create(TEX_PASS("HpGage/Battle_HP_top_KanneleBoldow.png"));

		//一体目のモデル
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Kannele] = new Model();
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Kannele] = new Model();

		//一体目の待機アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Kannele]
			->Load(MODEL_PASS("Leader/Kannele/Anim_Boss03_Kannele_Idle.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Idle][(int)Boss::Kannele] = m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Kannele]
			->AddAnimation(MODEL_PASS("Leader/Kannele/Anim_Boss03_Kannele_Idle.fbx"));
		//一体目の被弾アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Kannele]
			->Load(MODEL_PASS("Leader/Kannele/Anim_Boss03_Kannele_Damage.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Damage][(int)Boss::Kannele] = m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Kannele]
			->AddAnimation(MODEL_PASS("Leader/Kannele/Anim_Boss03_Kannele_Damage.fbx"));

		//二体目のモデル
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Boldow] = new Model();
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Boldow] = new Model();

		//二体目の待機アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Boldow]
			->Load(MODEL_PASS("Leader/Boldow/Anim_Boss03_Boldow_Idle.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Idle][(int)Boss::Boldow] = m_pBossModel[(int)BossAnimation::Idle][(int)Boss::Boldow]
			->AddAnimation(MODEL_PASS("Leader/Boldow/Anim_Boss03_Boldow_Idle.fbx"));
		//二体目の被弾アニメーションモデルの読み込み
		m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Boldow]
			->Load(MODEL_PASS("Leader/Boldow/Anim_Boss03_Boldow_Damage.fbx"), 1.0f, Model::None);
		m_pBoss_Anima[(int)BossAnimation::Damage][(int)Boss::Boldow] = m_pBossModel[(int)BossAnimation::Damage][(int)Boss::Boldow]
			->AddAnimation(MODEL_PASS("Leader/Boldow/Anim_Boss03_Boldow_Damage.fbx"));

		//--車
		//ステージのサブ番号によって車のモデルを変更
		switch (StageType.StageSubNumber)
		{
		case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE1://雪原ステージ1
			//ボスの車(初期段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss03_LiveStage.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE2://雪原ステージ2
			//ボスの車(中間段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss03_LiveStage.fbx"), 1.0f, Model::None);
			break;
		case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE3://雪原ステージ3
			//ボスの車(最終段階)
			m_pBosCar = new Model();
			m_pBosCar->Load(MODEL_PASS("Car/Prop_Boss03_LiveStage.fbx"), 1.0f, Model::None);
			break;
		}
		break;
	}

	/*＝＝＝ダメージログ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
	//優位ダメージログのポインタの作成
	m_pDamageTex[(int)DamageDraw::NormalBlue] = new Texture();
	//優位ダメージログのテクスチャの読み込み
	m_pDamageTex[(int)DamageDraw::NormalBlue]->Create(TEX_PASS("DamageLog/Battle_Damage_Blue.png"));
	//劣位ダメージログのポインタの作成
	m_pDamageTex[(int)DamageDraw::NormalRed] = new Texture();
	//劣位ダメージログのテクスチャの読み込み
	m_pDamageTex[(int)DamageDraw::NormalRed]->Create(TEX_PASS("DamageLog/Battle_Damage_Pink.png"));
	//通常ダメージログのポインタの作成
	m_pDamageTex[(int)DamageDraw::NotQuite] = new Texture();
	//通常ダメージログのテクスチャの読み込み
	m_pDamageTex[(int)DamageDraw::NotQuite]->Create(TEX_PASS("DamageLog/Battle_Damage_Gray.png"));


	/*＝＝＝サウンド＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
	m_NormalAttackSound = new CSoundList(SE_NORMALATTACK);
	m_NormalAttackSound->SetMasterVolume();
	m_WeaknessAttackSound = new CSoundList(SE_WEAKNESSATTACK);
	m_WeaknessAttackSound->SetMasterVolume();
	m_SummonSound[0] = new CSoundList(SE_SUMMON);
	m_SummonSound[0]->SetMasterVolume();
	m_SummonSound[1] = new CSoundList(SE_SUMMONENEMY);
	m_SummonSound[1]->SetMasterVolume();
	for (int i = 0; i < 2; i++)
	{
		m_pSourceSummon[i] = m_SummonSound[i]->m_sound->CreateSourceVoice(m_pSourceSummon[i]);
		XAUDIO2_BUFFER buffer = m_SummonSound[i]->GetBuffer(false);
		m_pSourceSummon[i]->SubmitSourceBuffer(&buffer);
	}
	m_wandonoffSound = new CSoundList(SE_WANDONOFF);
	m_wandonoffSound->SetMasterVolume();
	m_pSourceWandonoff = m_wandonoffSound->GetSound(false);


	/*＝＝＝エフェクト＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
	//エフェクトのポインタの作成
	m_pCharacterEffects[(int)CharactersEffect::Aura] = new CEffectManager_sp(EFFECT_PASS("Sprite/Aura.png"), 4, 14, 3.0f);
	m_pCharacterEffects[(int)CharactersEffect::Create] = new CEffectManager_sp(EFFECT_PASS("Sprite/warp.png"), 4, 8, 3.0f);
	m_pCharacterEffects[(int)CharactersEffect::SwordAtk] = new CEffectManager_sp(EFFECT_PASS("Sprite/SwordAtk.png"), 4, 7,1.0f);
	m_pCharacterEffects[(int)CharactersEffect::BowAtk] = new CEffectManager_sp(EFFECT_PASS("Sprite/BowAtk.png"), 4, 6, 1.0f);
	m_pCharacterEffects[(int)CharactersEffect::Death] = new CEffectManager_sp(EFFECT_PASS("Sprite/Death.png"), 4, 8, 3.0f);
}

//事前読み込みで用意したもののポインタ破棄
void CCharacterManager::UnInitCharacterTexture()
{
	//車モデルの破棄
	SAFE_DELETE(m_pBosCar);
	//味方モデルの破棄
	for (int i = 0; i < (int)Ally::MAX; i++)for (int j = 0; j < (int)CharacterAnimation::MAX; j++)SAFE_DELETE(m_pAllyModel[j][i]);
	//敵モデルの破棄
	for (int i = 0; i < (int)Enemy::MAX; i++)SAFE_DELETE(m_pEnemyModel[i]);
	//Linieのモデルの破棄
	for (int j = 0; j < (int)LinieAnimation::MAX; j++)SAFE_DELETE(m_pLinieModel[j]);
	//ボスたちのモデルの破棄
	for (int i = 0; i < (int)Boss::MAX; i++)for (int j = 0; j < (int)BossAnimation::MAX; j++)SAFE_DELETE(m_pBossModel[j][i]);
	//Hpテクスチャの破棄
	for (int i = 0; i < (int)HpTexture::MAX; i++)SAFE_DELETE(m_pHpGageTex[i]);
	//ダメーログジテクスチャの破棄
	for (int i = 0; i < (int)DamageDraw::MAX; i++)SAFE_DELETE(m_pDamageTex[i]);
	//サウンドの破棄
	SAFE_DELETE(m_NormalAttackSound);
	SAFE_DELETE(m_WeaknessAttackSound);
	for (int i = 0; i < 2; i++)
	{
		if (m_pSourceSummon[i])
		{
			m_pSourceSummon[i]->Stop();
			m_pSourceSummon[i]->DestroyVoice();
			m_pSourceSummon[i] = nullptr;
		}
	}
	SAFE_DELETE(m_SummonSound[0]);
	SAFE_DELETE(m_SummonSound[1]);
	SAFE_DELETE(m_wandonoffSound);
	m_pSourceWandonoff = nullptr;
	//エフェクトの破棄
	for (int i = 0; i < (int)CharactersEffect::MAX; i++)SAFE_DELETE(m_pCharacterEffects[i]);

}

/*
* @brief キャラクターのモデルの初期化
* @param StageType ステージの種類
*/
void CCharacterManager::ReLoadCharacterTexture(StageType StageType)
{
	UnInitCharacterTexture();
	InitCharacterTexture(StageType);
}

/*
* @brief キャラクターのモデルの更新
*/
void CCharacterManager::ModelUpDate(void)
{
	//味方兵のモデルのアニメーションを更新する
	for (int i = 0; i < (int)Ally::MAX; i++)
	{
		for (int j = 1; j < (int)CharacterAnimation::MAX; j++)
		{
			if (m_pAllyModel[j][i]->IsAnimePlay(0))
			{
				//アニメーションのステップを0.01f進める
 				m_pAllyModel[j][i]->Step(0.01f);
			}
			else
			{
				//アニメーションの再生
				m_pAllyModel[j][i]->PlayAnime(0, true);
				//アニメーションの時間を0にする
				m_pAllyModel[j][i]->SetAnimeTime(m_pAllyModel[j][i]->GetAnimePlayNo(), 0.0f);
			}
		}
	}

	//プレイヤーのモデルのアニメーションを更新する
	for (int i = 0; i < (int)LinieAnimation::MAX; i++)
	{
		if (i == (int)LinieAnimation::Win)continue;
		if (m_pLinieModel[i]->IsAnimePlay(0))
		{
			//アニメーションのステップを0.01f進める
			m_pLinieModel[i]->Step(0.01f);
		}
		else
		{
			//アニメーションの再生
			m_pLinieModel[i]->PlayAnime(0, true);
			//アニメーションの時間を0にする
			m_pLinieModel[i]->SetAnimeTime(m_pLinieModel[i]->GetAnimePlayNo(), 0.0f);

		}
	}

	//ボスのモデルのアニメーションを更新する
	for (int i = 0; i < (int)BossAnimation::MAX; i++)
	{
		for (int j = 0; j< (int)(int)Boss::MAX; j++)
		{
			if (m_pBossModel[i][j] != nullptr)
			{
				if (m_pBossModel[i][j]->IsAnimePlay(0))
				{
					//アニメーションのステップを0.01f進める
					m_pBossModel[i][j]->Step(0.01f);
				}
				else
				{
					//アニメーションの再生
					m_pBossModel[i][j]->PlayAnime(0, true);
					//アニメーションの時間を0にする
					m_pBossModel[i][j]->SetAnimeTime(m_pBossModel[i][j]->GetAnimePlayNo(), 0.0f);
				}
			}
		}
	}
}

/*
* @brief キャラクターのモデルの勝利アニメーションを更新する
* @param IsStep スアニメーションのテップを進めるかどうか
*/
void CCharacterManager::WinModelUpdate(bool IsStep)
{
	//Linieの勝利アニメーション
	if (m_pLinieModel[(int)LinieAnimation::Win]->IsAnimePlay(0))
	{
		if (IsStep)
		{
			//勝利アニメーションのステップを0.021f進める
			m_pLinieModel[(int)LinieAnimation::Win]->Step(0.02f);
		}
	}
	else
	{
		//勝利アニメーションの再生
		m_pLinieModel[(int)LinieAnimation::Win]->PlayAnime(0, false);
		//勝利アニメーションの時間を0にする
		m_pLinieModel[(int)LinieAnimation::Win]->SetAnimeTime(m_pLinieModel[(int)LinieAnimation::Win]->GetAnimePlayNo(), 0.0f);
		//勝利アニメーションのステップを0.01f進める
		m_pLinieModel[(int)LinieAnimation::Win]->Step(0.01f);
	}
}

/*
* @brief モデルの再読み込み
*/
void CCharacterManager::ReLoadSound()
{
	SAFE_DELETE(m_NormalAttackSound);
	SAFE_DELETE(m_WeaknessAttackSound);
	for (int i = 0; i < 2; i++)
	{
		m_pSourceSummon[i]->Stop();
		m_pSourceSummon[i]->DestroyVoice();
		m_pSourceSummon[i] = nullptr;
	}
	SAFE_DELETE(m_SummonSound[0]);
	SAFE_DELETE(m_SummonSound[1]);
	m_pSourceWandonoff->Stop();
	m_pSourceWandonoff = nullptr;
	SAFE_DELETE(m_wandonoffSound);
	/*サウンドの読み込み*/
	m_NormalAttackSound = new CSoundList(SE_NORMALATTACK);
	m_NormalAttackSound->SetMasterVolume();
	m_WeaknessAttackSound = new CSoundList(SE_WEAKNESSATTACK);
	m_WeaknessAttackSound->SetMasterVolume();
	m_SummonSound[0] = new CSoundList(SE_SUMMON);
	m_SummonSound[0]->SetMasterVolume();
	m_SummonSound[1] = new CSoundList(SE_SUMMONENEMY);
	m_SummonSound[1]->SetMasterVolume();
	for (int i = 0; i < 2; i++)
	{
		m_pSourceSummon[i] = m_SummonSound[i]->m_sound->CreateSourceVoice(m_pSourceSummon[i]);
		XAUDIO2_BUFFER buffer = m_SummonSound[i]->GetBuffer(false);
		m_pSourceSummon[i]->SubmitSourceBuffer(&buffer);
	}
	m_wandonoffSound = new CSoundList(SE_WANDONOFF);
	m_wandonoffSound->SetMasterVolume();
	m_pSourceWandonoff = m_wandonoffSound->GetSound(false);

	for (int i = 0; i < (int)CharactersEffect::MAX; i++)SAFE_DELETE(m_pCharacterEffects[i]);

	m_pCharacterEffects[(int)CharactersEffect::Aura] = new CEffectManager_sp(EFFECT_PASS("Sprite/Aura.png"), 4, 14, 3.0f);
	m_pCharacterEffects[(int)CharactersEffect::Create] = new CEffectManager_sp(EFFECT_PASS("Sprite/warp.png"), 4, 8, 3.0f);
	m_pCharacterEffects[(int)CharactersEffect::SwordAtk] = new CEffectManager_sp(EFFECT_PASS("Sprite/SwordAtk.png"), 4, 7, 1.0f);
	m_pCharacterEffects[(int)CharactersEffect::BowAtk] = new CEffectManager_sp(EFFECT_PASS("Sprite/BowAtk.png"), 4, 6, 1.0f);
	m_pCharacterEffects[(int)CharactersEffect::Death] = new CEffectManager_sp(EFFECT_PASS("Sprite/Death.png"), 4, 8, 3.0f);
}

/*
* @brief キャラクターのマスターボリュームを設定する
*/
void CCharacterManager::SetCharacterMasterVolume()
{
	m_NormalAttackSound->SetMasterVolume();
	m_WeaknessAttackSound->SetMasterVolume();
	m_SummonSound[0]->SetMasterVolume();
	m_SummonSound[1]->SetMasterVolume();
	m_wandonoffSound->SetMasterVolume();
}

/*
* @brief 描画設定
* @param InPos UIの位置
* @param InSize UIのサイズ
* @param Sprite スプライトのポインタ
*/
void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize, Sprite* Sprite)
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		InPos.x,
		InPos.y,
		InPos.z,
		0.0f
	));

	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		0.0f,
		0.0f
	));

	//拡縮行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);

	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	//ワールド行列を設定
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	world = mat;

	//ワールド行列を転置して格納
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	//ビュー行列を設定
	wvp[1] = GetView();
	//プロジェクション行列を設定
	wvp[2] = GetProj();

	//スプライトにワールド行列を設定
	Sprite->SetWorld(wvp[0]);
	//スプライトにビュー行列を設定
	Sprite->SetView(wvp[1]);
	//スプライトにプロジェクション行列を設定
	Sprite->SetProjection(wvp[2]);
}