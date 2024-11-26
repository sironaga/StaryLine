#pragma once
#define _DIRECTX_
#include "DirectX.h"
#undef max

//Character.h
//編集者：AT12C05宇留野陸斗
//編集者：AT12A07小川蓮

#define MAX_ALLY  (200)	//味方の最大数
#define MAX_ENEMY (200)	//敵の最大数


class CFieldVertex;
void IninCharacterTexture(CFieldVertex* InAddress);	//テクスチャ読み込み

//属性用構造体


//キャラクター基底クラス
class CFighter
{
	/*＝＝＝＝＝＝＝＝＝＝列挙型＝＝＝＝＝＝＝＝＝＝*/
public:
	//ステータス情報
	enum Status
	{
		Create, //召喚
		Battle,	//戦闘
		Death,	//死亡
		Delete,	//削除
	};
protected:
	//角数
	enum Corner
	{
		Triangle = 3,		//三角形
		Square = 4,			//四角形
		Pentagon = 5,		//五角形
		Hexagon = 6,		//六角形
		Heptagon = 7,		//七角形
		Octagon = 8,		//八角形
	};

	/*構造体*/
protected:
	//当たり判定
	struct Collision
	{
		float Width;	//幅
		float Height;	//高さ
	};

	/*関数*/
protected:
	//ステータス別の更新処理
	virtual void CreateUpdate(void) = 0;
	virtual void BattleUpdate(void) = 0;
	virtual void DeathUpdate(void) = 0;

	
public:
	CFighter(int InCornerCount, float InSize, CVector3<float> FirstPos);	//コンストラクタ
	virtual ~CFighter() = 0;						//デストラクタ

	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理

	void CollisionDraw(void);

	bool AtkCollisionCheck(CVector3<float> InSize, CVector3<float> InPos);	//当たり判定の中に敵がいるかの判定
	bool SearchCollisionCheck(CVector3<float> InSize, CVector3<float> InPos);	//当たり判定の中に敵がいるかの判定
	void Damage(float InDamage);

protected:
	virtual void SettingStatus(void) = 0;		//ステータス決め
	
	/*変数*/
public:
	int m_nTargetNumber;			//標的の選別番号
	bool m_bIsAttack;				//攻撃しているかどうか
protected:
	Status m_tStatus;				//ステータス状態
	Collision m_tSearchCollision;	//索敵当たり判定
	Collision m_tAtkCollision;		//攻撃当たり判定
	CVector3<float> m_tPos;			//位置座標
	CVector3<float> m_tSize;		//サイズ
	int nCornerCount;				//属性
	float m_fHp;					//体力
	float m_fPhysicsAtk;			//物理攻撃
	float m_fMagicAtk;				//魔法攻撃
	float m_fAtkCharge;				//攻撃チャージ(チャージがたまっていたら攻撃可能)
	float m_fAtkChargeMax;			//攻撃チャージの到達値
	bool m_bCreateInit;				//生成状態の初期化済みかどうか
	ID3D11Buffer* m_pVtx;			//画像の頂点座標を取得

	/*変数のSet&Get*/
public:
	//ステータスのSet
	void SetStatus(Status InStatus) { m_tStatus = InStatus; }
	//ステータスのget
	Status GetStatus(void) { return m_tStatus; }

	//索敵当たり判定のGet
	Collision GetSearchCollision(void) { return m_tSearchCollision; }
	//攻撃当たり判定のGet
	Collision GetAtkCollision(void) { return m_tAtkCollision; }

	//X座標の加算
	void AddPosX(float fAdd) { m_tPos.X += fAdd; }
	//Y座標の加算
	void AddPosY(float fAdd) { m_tPos.Y += fAdd; }
	//Z座標の加算
	void AddPosZ(float fAdd) { m_tPos.Z += fAdd; }
	//位置座標のSet
	void SetPos(CVector3<float> InPos) { m_tPos = InPos; }
	//位置座標のGet
	CVector3<float> GetPos(void) { return m_tPos; }

	//サイズのGet
	CVector3<float> GetSize(void) { return m_tSize; }

	//属性のGet
	int GetCornerCount(void) { return nCornerCount; }

	//体力のGet
	float GetHp(void) { return m_fHp; }

	//物理攻撃力のGet
	float GetPhysicsAtk(void) { return m_fPhysicsAtk; }
	//魔法攻撃力のGet
	float GetMagicsAtk(void) { return m_fMagicAtk; }

	//自分の攻撃に使う攻撃力のGet
	float GetAtk(void) {
		return std::max(m_fPhysicsAtk, m_fMagicAtk);
		//if (m_fPhysicsAtk > m_fMagicAtk)return m_fPhysicsAtk; 
		//else return m_fMagicAtk;
	}

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
	CAlly(int InCornerCount, float InSize, CVector3<float> FirstPos);		//コンストラクタ
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
	CEnemy(int InCornerCount, float InSize, CVector3<float> FirstPos);		//コンストラクタ
	~CEnemy();						//デストラクタ

	void Update(void)	override;	//更新処理
	void Draw(void)		override;	//描画処理

private:
	void CreateUpdate(void) override;
	void BattleUpdate(void) override;
	void DeathUpdate(void)  override;

	void SettingStatus(void) override;
};