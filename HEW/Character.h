//Character.h
//編集者：AT12C05宇留野陸斗
//編集者：AT12A07小川蓮

#pragma once
#define _DIRECTX_
#include "DirectX.h"
#include "Sprite.h"
#include "SoundList.h"
#include "Model.h"
#include "EffectManager_sp.h"
#include "Main.h"

#define MAX_ALLY  (200)	//味方情報の最大数
//#define MAX_ENEMY (200)	//敵の最大数
//#define NORMAL_SIZE (10)//キャラクターの基本サイズ
enum class MODEL_DEFAULTSIZE
{
	Ally = 13,
	Enemy = 13,
};
#define LeaderSize (0.8)

enum class CharacterAnimation
{
	None,
	Walk,
	Attack_Advantage,
	Attack_Disadvantage,
	Death,
	MAX,
};

void InitCharacterTexture(StageType StageType);	//テクスチャ読み込み
void UnInitCharacterTexture();//テクスチャの終了処理
void ReLoadCharacterTexture(StageType StageType);//テクスチャの再読み込み
void ReLoadSound();//音の再読み込み
void SetCharacterMasterVolume();//マスターヴォリュームの設定

//ステータス情報
enum Status
{
	St_Create, //召喚
	St_Battle,	//戦闘
	St_Death,	//死亡
	St_Delete,	//削除
};

class CHpUI
{
public:
	enum HpUINumber
	{
		Ally,
		Enemy,
		Bos,
		Player,
	};
public:
	CHpUI(float FullHp, HpUINumber Number);
	~CHpUI();
	void Update(float InHp, DirectX::XMFLOAT3 InPos,float InSizeY);
	void Draw(int nCornerCount = 0);
private:
	float m_fNowHp;
	float m_fFullHp;
	float m_fAnchorPoint;
	Sprite* m_pSprite;
	HpUINumber m_tNumber;

	DirectX::XMFLOAT3 m_tUIPos;
	DirectX::XMFLOAT3 m_tUIScale;
};

//キャラクター基底クラス
class CFighter
{
	/*＝＝＝＝＝＝＝＝＝＝列挙型＝＝＝＝＝＝＝＝＝＝*/
public:
	enum class FighterEffect
	{
		Aura,
		Create,
		Attack,
		Death,
		MAX,
	};
protected:
	//角数
	enum Corner
	{
		Triangle = 3,		//三角形
		Square = 4,			//四角形
	};

	/*構造体*/
protected:
	//当たり判定
	struct Collision
	{
		float Width;	//幅
		float Height;	//高さ
	};
	//エフェクト関係
	//struct EffectInfo
	//{
	//	CEffectManager* m_pEffect; //エフェクト関係
	//	float m_fEffectTimer;
	//	bool m_bEffectPlay;//攻撃エフェクトのPlayを実行したかどうか
	//};

	/*関数*/
protected:
	//ステータス別の更新処理
	virtual void CreateUpdate(void) = 0;
	virtual void BattleUpdate(void) = 0;
	virtual void DeathUpdate(void) = 0;

	
public:
	CFighter(int InCornerCount, bool IsStellaBuff = false);	//コンストラクタ
	virtual ~CFighter();						//デストラクタ

	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理

	bool AtkCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos);	//当たり判定の中に敵がいるかの判定
	bool SearchCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos);	//当たり判定の中に敵がいるかの判定
	bool OverlapCheck(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize);	//重なっていないか判定
	void Damage(CFighter* pFighter);
protected:
	void DamageLogDraw(int nCornerCount, bool Type);
	virtual void SettingStatus(void) = 0;		//ステータス決め
	/*変数*/
public:
	int m_nTargetNumber;			//標的の選別番号
	bool m_bIsAttack;				//攻撃しているかどうか
	bool m_bTypeAttack;				//攻撃対象の相性がいいかどうか
	bool m_bFirstBattlePosSetting;	//戦闘シーンの開始位置に移動したかどうか

	bool m_bIsAnimationFlag[(int)CharacterAnimation::MAX];
	/*ダメージログ関係*/
	bool m_bDamageLogDraw[3];		//ダメージログ描画関連のフラグ　[0]..描画フラグ,[1]..移動変数初期化フラグ,[2]..相性のフラグ
	DirectX::XMFLOAT3 m_tDamageLogPos;
	/*================*/
protected:
	Status m_tStatus;				//ステータス状態
	Collision m_tSearchCollision;	//索敵当たり判定
	Collision m_tAtkCollision;		//攻撃当たり判定
	DirectX::XMFLOAT3 m_tPos;		//位置座標
	DirectX::XMFLOAT3 m_tFirstPos;	//初期位置座標
	DirectX::XMFLOAT3 m_tTargetPos;	//標的の位置
	DirectX::XMFLOAT3 m_tSize;		//サイズ
	int m_nCornerCount;				//属性
	bool m_bStellaBuff;				//ステラ判定
	float m_fHp;					//体力
	float m_fAtk;					//攻撃力
	float m_fAtkCharge;				//攻撃チャージ(チャージがたまっていたら攻撃可能)
	float m_fAtkChargeMax;			//攻撃チャージの到達値
	float m_fAtkAnimationTime;		//攻撃アニメーションの時間
	float m_fAtkAnimationMaxTime;	//攻撃アニメーションの最大時間
	bool m_bIsHit;					//攻撃を受けたかの判定

	float m_fDamageLogMoveY;

	bool m_bMoveUp;					//上移動フラグ
	bool m_bMoveFlag;				//移動したか
	DirectX::XMFLOAT3 m_tDestinationPos;//目的地

	CHpUI* m_pHpGage;				//体力ゲージ
	Sprite* m_pSprite;


protected:
	Model* m_pModel;
	IXAudio2SourceVoice* m_pSourceNormalAttack;//スピーカー
	IXAudio2SourceVoice* m_pSourceWeaknessAttack;//スピーカー
	
	float m_fTimeSound;
	bool m_bTimeSoundStart;

	bool m_bReLoadFlag;
	//EffectInfo m_tEffect[(int)FighterEffect::MAX];
	CEffectManager_sp* m_pEffect[(int)FighterEffect::MAX];
	bool IsCreateEffectPlay;
	bool IsAttackEffectPlay;
	bool IsDeathEffectPlay;

	/*変数のSet&Get*/
public:
	//モデル読み込みフラグの切り替え
	void SetReLoadFlag(void) { m_bReLoadFlag = true; }

	//ステータスのSet
	void SetStatus(Status InStatus) { m_tStatus = InStatus; }
	//ステータスのget
	Status GetStatus(void) { return m_tStatus; }

	//ステラ判定のSet
	void SetStellaBuffFlag(bool IsBuff) { m_bStellaBuff = IsBuff; }
	//死亡エフェクトの再生
	//void PlayDeathEffect(void);

	//索敵当たり判定のGet
	Collision GetSearchCollision(void) { return m_tSearchCollision; }
	//攻撃当たり判定のGet
	Collision GetAtkCollision(void) { return m_tAtkCollision; }

	//X座標の加算
	void AddPosX(float fAdd) { m_tPos.x += fAdd; }
	//Y座標の加算
	void AddPosY(float fAdd) { m_tPos.y += fAdd; }
	//Z座標の加算
	void AddPosZ(float fAdd) { m_tPos.z += fAdd; }
	//位置座標のSet
	void SetPos(DirectX::XMFLOAT3 InPos) { m_tPos = InPos; }
	//位置X座標のSet
	void SetPosX(float InPosX) { m_tPos.x = InPosX; }
	//位置Y座標のSet
	void SetPosY(float InPosY) { m_tPos.y = InPosY; }
	//位置Z座標のSet
	void SetPosZ(float InPosZ) { m_tPos.z = InPosZ; }
	//位置座標のGet
	DirectX::XMFLOAT3 GetPos(void) { return m_tPos; }

	//標的の位置のSet
	void SetTargetPos(DirectX::XMFLOAT3 InPos) { m_tTargetPos = InPos; }
	//標的の位置のGet
	DirectX::XMFLOAT3 GetTargetPos(void) { return m_tTargetPos; }

	//初期位置のGet
	DirectX::XMFLOAT3 GetFirstPos(void) { return m_tFirstPos; }
	//初期位置のSet
	void SetFirstPos(DirectX::XMFLOAT3 InPos) { m_tFirstPos = InPos; }

	//移動フラグのSet
	void SetMoveFlag(bool flag) { m_bMoveFlag = flag; }
	//移動フラグのGet
	bool GetMoveFlag(void) { return m_bMoveFlag; }

	//目的地のSet
	void SetDestinationPos(DirectX::XMFLOAT3 pos) { m_tDestinationPos = pos; }
	//目的地のGet
	DirectX::XMFLOAT3 GetDestinationPos(void) { return m_tDestinationPos; }

	//サイズのGet
	DirectX::XMFLOAT3 GetSize(void) { return m_tSize; }

	//属性のGet
	int GetCornerCount(void) { return m_nCornerCount; }

	//体力のGet
	float GetHp(void) { return m_fHp; }

	//攻撃力のSet
	void SetAtk(float InAtk) { m_fAtk = InAtk; }
	//攻撃力のGet
	float GetAtk(void) { return m_fAtk; }
	//攻撃力のAdd
	void AddAtk(float InAtk) { m_fAtk += InAtk; }

	//攻撃チャージ到達値のGet
	float GetCoolTime(void) { return m_fAtkChargeMax; }

	//攻撃チャージの加算
	void AddAtkCharge(float AddFloat = 1.0f) { m_fAtkCharge += AddFloat; }
	//攻撃チャージのGet
	float GetAtkCharge(void) { return m_fAtkCharge; }
	//攻撃チャージのリセット
	void ChargeReset(void) { m_fAtkCharge = 0; }
};

//味方キャラクタークラス
class CAlly : public CFighter
{
public:
	CAlly(int InCornerCount, bool IsStellaBuff);		//コンストラクタ
	~CAlly();						//デストラクタ

	void Update(void)	override;	//更新処理
	void Draw(void)		override;	//描画処理

private:
	void CreateUpdate(void) override;
	void BattleUpdate(void) override;
	void DeathUpdate(void)  override;
	
	void SettingStatus(void) override;

};

//敵キャラクタークラス
class CEnemy : public CFighter
{
public:
	CEnemy(int InCornerCount);		//コンストラクタ
	~CEnemy();						//デストラクタ

	void Update(void)	override;	//更新処理
	void Draw(void)		override;	//描画処理

private:
	void CreateUpdate(void) override;
	void BattleUpdate(void) override;
	void DeathUpdate(void)  override;

	void SettingStatus(void) override;

};

//リーダークラス
class CLeader
{
public:
	CLeader(float InSize, DirectX::XMFLOAT3 FirstPos, int InTextureNumber,bool SubModelCreate = false);
	~CLeader();

	void Update(bool IsStart = false,bool IsEnd = false);	//更新処理
	void Draw(int StageNum = 0);	//描画処理
	void HpDraw(void);	//Hpの描画処理

	void Damage(CFighter* pFighter);
private:
	void CreateUpdate(void);
	void BattleUpdate(bool IsStart = false, bool IsEnd = false);
	void DeathUpdate(void);

private:
	bool m_bReLoadFlag;

	Status m_tStatus;				//ステータス状態
	Model* m_pModel;
	DirectX::XMFLOAT3 m_tPos;		//位置座標
	DirectX::XMFLOAT3 m_tSize;		//サイズ
	bool m_bSubModelCreate;

	Model* m_pSubModel;
	DirectX::XMFLOAT3 m_tSubPos;		//位置座標
	DirectX::XMFLOAT3 m_tSubSize;		//サイズ

	float m_fHp;					//体力
	float m_fMaxHp;					//最大体力

	bool m_bAnimationPlay;
	//int m_nStatusMode;
	int m_nTextureNumber;
	CHpUI* m_pHpGage;
public:
	//モデル読み込みフラグの切り替え
	void SetReLoadFlag(void) { m_bReLoadFlag = true; }
	//ステータスのSet
	void SetStatus(Status InStatus) { m_tStatus = InStatus; }
	//ステータスのget
	Status GetStatus(void) { return m_tStatus; }

	//位置座標のGet
	DirectX::XMFLOAT3 GetPos(void) { return m_tPos; }

	//サイズのGet
	DirectX::XMFLOAT3 GetSize(void) { return m_tSize; }

	//現在HpのGet
	float GetHp(void) { return m_fHp; }
	//最大HpのGet
	float GetMaxHp(void) { return m_fMaxHp; }

	void SetAnimationPlayFlag(bool IsFlag) { m_bAnimationPlay = IsFlag; }
	//void ChangePlayerMode(void) { m_nStatusMode ^= m_nStatusMode; }//プレイヤーのアニメーションの切り替え
};