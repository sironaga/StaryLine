/*==============================================================
* 
*  File：Fighter.h
* 
*  編集者：宇留野陸斗｜[作成][管理]
* 　
*　編集者：中嶋飛賀　｜[ 音 ]
*　
================================================================*/
#pragma once

#include "CharacterManager.h"
#include "HpUI.h"

// 兵士のタイプ
enum class FighterType
{
	Enemy,//敵
	Ally,//味方
};

//兵士基底クラス
class CFighter
{
private:
	//当たり判定
	struct Collision
	{
		float Width;	//幅
		float Height;	//高さ
	};

protected:
	//ステータス別の更新処理
	//生成モードの更新処理
	virtual void CreateUpdate(void);

	//戦闘モードの更新処理
	virtual void BattleUpdate(void);

	//死亡モードの更新処理
	virtual void DeathUpdate(void);


public:
	//コンストラクタ
	CFighter(int InCornerCount, bool IsStellaBuff = false);

	//デストラクタ
	virtual ~CFighter();					

	//更新処理
	virtual void Update(void);

	//描画処理
	virtual void Draw(void);

	//攻撃当たり判定
	bool AtkCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos);

	//索敵当たり判定
	bool SearchCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos);

	//重なり判定
	bool OverlapCheck(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize);

	//攻撃を受けた時の処理
	void Damage(CFighter* pFighter);

protected:
	//ダメージログの描画
	void DamageLogDraw(int nCornerCount, bool Type);

	//ステータス決め
	virtual void SettingStatus(void);	

public:
	// 標的の選別番号
	int m_nTargetNumber;

	//攻撃フラグ
	bool m_bIsAttack;

	//死亡フラグ
	bool m_bIsDeath;

	//攻撃対象の相性フラグ
	bool m_bTypeAttack;

	//戦闘シーンの開始位置に移動したかの判定フラグ
	bool m_bFirstBattlePosSetting;

	//ダメージログ描画関連のフラグ　[0]..描画フラグ,[1]..移動変数初期化フラグ,[2]..相性のフラグ
	bool m_bDamageLogDraw[3];

	//ダメージログの移動フラグ
	DirectX::XMFLOAT3 m_tDamageLogPos;

protected:
	//ステータス状態
	Status m_tStatus;	

	//索敵当たり判定
	Collision m_tSearchCollision;

	//攻撃当たり判定
	Collision m_tAtkCollision;

	//位置座標
	DirectX::XMFLOAT3 m_tPos;

	//初期位置座標
	DirectX::XMFLOAT3 m_tFirstPos;

	//標的の位置
	DirectX::XMFLOAT3 m_tTargetPos;

	//サイズ
	DirectX::XMFLOAT3 m_tSize;

	//兵士のタイプ
	FighterType m_eFighterType;

	//角数
	int m_nCornerCount;

	//ステラバフ判定
	bool m_bStellaBuff;

	//体力
	float m_fHp;

	//攻撃力
	float m_fAtk;

	//攻撃チャージ(チャージがたまっていたら攻撃可能)
	float m_fAtkCharge;

	//攻撃チャージの到達値
	float m_fAtkChargeMax;

	//攻撃アニメーションの時間
	float m_fAtkAnimationTime;

	//攻撃アニメーションの最大時間
	float m_fAtkAnimationMaxTime;

	//被弾フラグ
	bool m_bIsHit;		

	//ダメージログのY軸の移動
	float m_fDamageLogMoveY;

	//上移動フラグ
	bool m_bMoveUp;

	//移動フラグ
	bool m_bMoveFlag;

	//目的地
	DirectX::XMFLOAT3 m_tDestinationPos;

	//体力ゲージのポインタ
	CHpUI* m_pHpGage;

	//スプライトのポインタ
	Sprite* m_pSprite;


	//モデルのポインタ
	Model* m_pModel[(int)CharacterAnimation::MAX];

	//モデルのアニメーションモデル番号
	CharacterAnimation m_eModelNo;

	//スピーカー
	IXAudio2SourceVoice* m_pSourceNormalAttack;
	IXAudio2SourceVoice* m_pSourceWeaknessAttack;

	//サウンドのポインタ
	float m_fTimeSound;

	//サウンドの再生フラグ
	bool m_bTimeSoundStart;

	//再読み込みフラグ
	bool m_bReLoadFlag;

	//エフェクトのポインタ
	CEffectManager_sp* m_pEffect[(int)FighterEffect::MAX];

	//-エフェクトの再生フラグ
	//生成
	bool IsCreateEffectPlay;

	//攻撃
	bool IsAttackEffectPlay;

	//死亡
	bool IsDeathEffectPlay;

public:
	//モデル読み込みフラグの切り替え
	inline void SetReLoadFlag(void) { m_bReLoadFlag = true; }

	//ステータスのSet
	inline void SetStatus(Status InStatus) { m_tStatus = InStatus; }

	//ステータスのget
	inline Status GetStatus(void) { return m_tStatus; }

	//ステラ判定のSet
	inline void SetStellaBuffFlag(bool IsBuff) { m_bStellaBuff = IsBuff; }

	//モデル番号のSet
	inline void SetModelNo(CharacterAnimation No) { m_eModelNo = No; }

	//索敵当たり判定のGet
	inline Collision GetSearchCollision(void) { return m_tSearchCollision; }

	//攻撃当たり判定のGet
	inline Collision GetAtkCollision(void) { return m_tAtkCollision; }

	//X座標の加算
	inline void AddPosX(float fAdd) { m_tPos.x += fAdd; }

	//Y座標の加算
	inline void AddPosY(float fAdd) { m_tPos.y += fAdd; }

	//Z座標の加算
	inline void AddPosZ(float fAdd) { m_tPos.z += fAdd; }

	//位置座標のSet
	inline void SetPos(DirectX::XMFLOAT3 InPos) { m_tPos = InPos; }

	//位置X座標のSet
	inline void SetPosX(float InPosX) { m_tPos.x = InPosX; }

	//位置Y座標のSet
	inline void SetPosY(float InPosY) { m_tPos.y = InPosY; }

	//位置Z座標のSet
	inline void SetPosZ(float InPosZ) { m_tPos.z = InPosZ; }

	//位置座標のGet
	inline DirectX::XMFLOAT3 GetPos(void) { return m_tPos; }

	//標的の位置のSet
	inline void SetTargetPos(DirectX::XMFLOAT3 InPos) { m_tTargetPos = InPos; }

	//標的の位置のGet
	inline DirectX::XMFLOAT3 GetTargetPos(void) { return m_tTargetPos; }

	//初期位置のGet
	inline DirectX::XMFLOAT3 GetFirstPos(void) { return m_tFirstPos; }

	//初期位置のSet
	inline void SetFirstPos(DirectX::XMFLOAT3 InPos) { m_tFirstPos = InPos; }

	//移動フラグのSet
	inline void SetMoveFlag(bool flag) { m_bMoveFlag = flag; }

	//移動フラグのGet
	inline bool GetMoveFlag(void) { return m_bMoveFlag; }

	//目的地のSet
	inline void SetDestinationPos(DirectX::XMFLOAT3 pos) { m_tDestinationPos = pos; }

	//目的地のGet
	inline DirectX::XMFLOAT3 GetDestinationPos(void) { return m_tDestinationPos; }

	//サイズのGet
	inline DirectX::XMFLOAT3 GetSize(void) { return m_tSize; }

	//兵士のタイプのGet
	inline FighterType GetFighterType(void) { return m_eFighterType; }

	//属性のGet
	inline int GetCornerCount(void) { return m_nCornerCount; }

	//体力のGet
	inline float GetHp(void) { return m_fHp; }

	//攻撃力のSet
	inline void SetAtk(float InAtk) { m_fAtk = InAtk; }

	//攻撃力のGet
	inline float GetAtk(void) { return m_fAtk; }

	//攻撃力のAdd
	inline void AddAtk(float InAtk) { m_fAtk += InAtk; }

	//攻撃チャージ到達値のGet
	inline float GetCoolTime(void) { return m_fAtkChargeMax; }

	//攻撃チャージの加算
	inline void AddAtkCharge(float AddFloat = 1.0f) { m_fAtkCharge += AddFloat; }

	//攻撃チャージのGet
	inline float GetAtkCharge(void) { return m_fAtkCharge; }

	//攻撃チャージのリセット
	inline void ChargeReset(void) { m_fAtkCharge = 0; }
};
