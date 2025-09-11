/*==============================================================
*　
*　File: CharacterManager.h
*　
*　編集者：宇留野陸斗｜[作成][管理]
*　
*　編集者：中嶋飛賀　｜[ 音 ]
*　
================================================================*/

#pragma once
#define _DIRECTX_
#include "DirectX.h"
#include "Sprite.h"
#include "SoundList.h"
#include "Model.h"
#include "EffectManager_sp.h"
#include "Main.h"
#include "ShaderList.h"
#include "Geometory.h"
#include <memory>

//味方の最大数
#define MAX_ALLY (200)
//プレイヤーの勝利アニメーションの時間
#define LINIEWINTIME (3.4f)
//リーダーのサイズ
#define LeaderSize (0.8)

//キャラクターの横の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_WIDTH(Num)  ((m_tSize.x *(Num / 2)) + (m_tSize.x))
//キャラクターの縦の索敵当たり判定(索敵範囲)
#define MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(Num) ((m_tSize.y *(Num / 2)) + (m_tSize.y))
//キャラクターの横の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_WIDTH(Num)  ((m_tSize.x * (Num / 2)) + (m_tSize.x))
//キャラクターの縦の攻撃当たり判定(相手の人数)
#define MAX_CHARACTER_ATK_COLLISION_HEIGHT(Num) ((m_tSize.y * (Num / 2)) + (m_tSize.y))

//モデルの初期サイズ
enum class MODEL_DEFAULTSIZE
{
	Ally = 13,
	Enemy = 13,
};

//プレイヤーのアニメーションの種類
enum class LinieAnimation
{
	Idle,
	Summon,
	Win,
	Damage,
	MAX,
};

//ボスのアニメーションの種類
enum class BossAnimation
{
	Idle,
	Damage,
	MAX,
};

//キャラクターのアニメーション種類
enum class CharacterAnimation
{
	None,
	Walk,
	Attack_Advantage,
	Attack_Disadvantage,
	Death,
	MAX,
};

//キャラクターのエフェクトの列挙型
enum class CharactersEffect
{
	Aura,
	Create,
	SwordAtk,
	BowAtk,
	Death,
	MAX,
};


//キャラクターの状態
enum class Status
{
	Create, //召喚
	Battle,	//戦闘
	Death,	//死亡
	Delete,	//削除
};

//Hpゲージのテクスチャの種類
enum class HpTexture
{
	Linie,
	Boss,
	Frame,
	Ally3,
	Ally4,
	Enemy1,
	Enemy2,
	MAX,
};

//味方兵の種類
enum class Ally
{
	Ally3,
	Ally4,
	MAX,
};

//敵兵の種類
enum class Enemy
{
	Enemy1,
	Enemy2,
	MAX,
};

//ボスの種類
enum class Boss
{
	Quracker = 0,
	Nugar = 0,
	Kannele = 0,
	Boldow = 1,
	MAX,
};

//ダメージテクスチャの種類
enum class DamageDraw
{
	NormalBlue,
	NormalRed,
	NotQuite,
	MAX,
};

//兵士のエフェクトの種類
enum class FighterEffect
{
	Aura,
	Create,
	Attack,
	Death,
	MAX,
};

//角数
enum class Corner
{
	Triangle = 3,		//三角形
	Square = 4,			//四角形
};


//描画設定
void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize, Sprite* Sprite);


//キャラクターの管理クラス
class CCharacterManager
{
public:
	//インスタンスの取得
	static CCharacterManager* GetInstance();
private:
	//インスタンスのポインタ
	static std::unique_ptr<CCharacterManager> m_pInstance;
public:
	//テクスチャ読み込み
	void InitCharacterTexture(StageType StageType);

	//テクスチャの終了処理
	void UnInitCharacterTexture();

	//テクスチャの再読み込み
	void ReLoadCharacterTexture(StageType StageType);

	//モデルの更新
	void ModelUpDate(void);

	//勝利モデルの更新
	void WinModelUpdate(bool IsStep);

	//音の再読み込み
	void ReLoadSound();

	//マスターボリュームの設定
	void SetCharacterMasterVolume();
private:
	/*事前読み込み用のポインタ*/

	//味方のモデル
	Model* m_pAllyModel[(int)CharacterAnimation::MAX][(int)Ally::MAX];

	//味方用アニメーション
	Model::AnimeNo m_pAlly_Anima[(int)CharacterAnimation::MAX][(int)Ally::MAX];

	//敵のモデル
	Model* m_pEnemyModel[(int)Enemy::MAX];

	//敵用アニメーション
	Model::AnimeNo m_pEnemy_Anima[(int)CharacterAnimation::MAX][(int)Enemy::MAX];

	//プレイヤーのモデル
	Model* m_pLinieModel[(int)LinieAnimation::MAX];

	//プレイヤー用アニメーション
	Model::AnimeNo m_pLinie_Anima[(int)LinieAnimation::MAX];

	//敵のボスのモデル
	Model* m_pBossModel[(int)BossAnimation::MAX][(int)Boss::MAX];

	//敵のボス用アニメーション
	Model::AnimeNo m_pBoss_Anima[(int)BossAnimation::MAX][(int)Boss::MAX];

	//敵のボスの車のモデル
	Model* m_pBosCar;

	//Hpのテクスチャ
	Texture* m_pHpGageTex[(int)HpTexture::MAX];

	//ダメージログのテクスチャ
	Texture* m_pDamageTex[(int)DamageDraw::MAX];

	//キャラクターのエフェクト
	CEffectManager_sp* m_pCharacterEffects[(int)CharactersEffect::MAX];

	//攻撃音
	CSoundList* m_NormalAttackSound;
	CSoundList* m_WeaknessAttackSound;
	CSoundList* m_SummonSound[2];
	CSoundList* m_wandonoffSound;
	IXAudio2SourceVoice* m_pSourceSummon[2];//スピーカー
	IXAudio2SourceVoice* m_pSourceWandonoff;//スピーカー

public://アクセサ
	//味方のモデルのポインタを取得
	inline Model* GetAllyModel(int InNum, int InAnimeNum) { return m_pAllyModel[InAnimeNum][InNum]; }

	//味方のアニメーションのポインタを取得
	inline Model::AnimeNo GetAllyAnime(int InNum, int InAnimeNum) { return m_pAlly_Anima[InAnimeNum][InNum]; }

	//敵のモデルのポインタを取得
	inline Model* GetEnemyModel(int InNum) { return m_pEnemyModel[InNum]; }

	//敵のアニメーションのポインタを取得
	inline Model::AnimeNo GetEnemyAnime(int InNum, int InAnimeNum) { return m_pEnemy_Anima[InAnimeNum][InNum]; }

	//リーダーのモデルのポインタを取得
	inline Model* GetLinieModel(int InNum) { return m_pLinieModel[InNum]; }

	//リーダーのアニメーションのポインタを取得
	inline Model::AnimeNo GetLinieAnime(int InNum) { return m_pLinie_Anima[InNum]; }

	//ボスのモデルのポインタを取得
	inline Model* GetBossModel(int InNum, int InAnimeNum) { return m_pBossModel[InAnimeNum][InNum]; }

	//ボスのアニメーションのポインタを取得
	inline Model::AnimeNo GetBossAnime(int InNum, int InAnimeNum) { return m_pBoss_Anima[InAnimeNum][InNum]; }

	//ボスの車のポインタを取得
	inline Model* GetBosCar(void) { return m_pBosCar; }

	//Hpゲージのテクスチャのポインタを取得
	inline Texture* GetHpGageTex(int InNum) { return m_pHpGageTex[InNum]; }

	//ダメージログのテクスチャのポインタを取得
	inline Texture* GetDamageTex(int InNum) { return m_pDamageTex[InNum]; }

	//キャラクターのエフェクトのポインタを取得
	inline CEffectManager_sp* GetCharacterEffect(int InNum) { return m_pCharacterEffects[InNum]; }

	//攻撃音のポインタを取得
	inline CSoundList* GetNormalAttackSound(void) { return m_NormalAttackSound; }
	inline CSoundList* GetWeaknessAttackSound(void) { return m_WeaknessAttackSound; }
	inline CSoundList* GetSummonSound(int InNum) { return m_SummonSound[InNum]; }
	inline IXAudio2SourceVoice* GetSourceSummon(int InNum) { return m_pSourceSummon[InNum]; }
	inline IXAudio2SourceVoice* GetSourceWandonoff(void) { return m_pSourceWandonoff; }
};