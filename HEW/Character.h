#pragma once
#define _DIRECTX_
#include "DirectX.h"
#include "Camera.h"
#include "Sprite.h"

//Character.h
//編集者：AT12C05宇留野陸斗
//編集者：AT12A07小川蓮

#define MAX_ALLY  (200)	//味方の最大数
#define MAX_ENEMY (200)	//敵の最大数
#define NORMAL_SIZE (10)//キャラクターの基本サイズ

class CFieldVertex;
void IninCharacterTexture(CFieldVertex* InAddress, int StageNum);	//テクスチャ読み込み

//ステータス情報
enum Status
{
	St_Create, //召喚
	St_Battle,	//戦闘
	St_Death,	//死亡
	St_Delete,	//削除
};

//キャラクター基底クラス
class CFighter
{
	/*＝＝＝＝＝＝＝＝＝＝列挙型＝＝＝＝＝＝＝＝＝＝*/
public:
	enum AtkType
	{
		AT_Physics,
		AT_Magic,
	};
protected:
	//角数
	enum Corner
	{
		Triangle = 3,		//三角形
		Square = 4,			//四角形
		Pentagon = 5,		//五角形
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
	CFighter(int InCornerCount, float InSize, CVector3<float> FirstPos,Camera* InAddress);	//コンストラクタ
	virtual ~CFighter() = 0;						//デストラクタ

	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理

	void CollisionDraw(void);

	bool AtkCollisionCheck(CVector3<float> InSize, CVector3<float> InPos);	//当たり判定の中に敵がいるかの判定
	bool SearchCollisionCheck(CVector3<float> InSize, CVector3<float> InPos);	//当たり判定の中に敵がいるかの判定
	void Damage(CFighter* pFighter);

protected:
	virtual void SettingStatus(void) = 0;		//ステータス決め
	void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize);
	
	/*変数*/
public:
	int m_nTargetNumber;			//標的の選別番号
	bool m_bIsAttack;				//攻撃しているかどうか
	bool m_bFirstBattlePosSetting;	//戦闘シーンの開始位置に移動したかどうか
protected:
	Status m_tStatus;				//ステータス状態
	Collision m_tSearchCollision;	//索敵当たり判定
	Collision m_tAtkCollision;		//攻撃当たり判定
	CVector3<float> m_tPos;			//位置座標
	CVector3<float> m_tSize;		//サイズ
	int nCornerCount;				//属性
	float m_fHp;					//体力
	float m_fShield;				//シールド値
	float m_fAtk;					//攻撃力
	AtkType m_tAtkType;				//攻撃タイプ
	float m_fAtkCharge;				//攻撃チャージ(チャージがたまっていたら攻撃可能)
	float m_fAtkChargeMax;			//攻撃チャージの到達値
	float m_fAtkAnimationTime;		//攻撃アニメーションの時間
	float m_fAtkAnimationMaxTime;	//攻撃アニメーションの最大時間
	bool m_bCreateInit;				//生成状態の初期化済みかどうか

	Camera* m_pCamera;
	Sprite* m_pSprite;
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
	//位置X座標のSet
	void SetPosX(float InPosX) { m_tPos.X = InPosX; }
	//位置Y座標のSet
	void SetPosY(float InPosY) { m_tPos.Y = InPosY; }
	//位置Z座標のSet
	void SetPosZ(float InPosZ) { m_tPos.Z = InPosZ; }

	//位置座標のGet
	CVector3<float> GetPos(void) { return m_tPos; }

	//サイズのGet
	CVector3<float> GetSize(void) { return m_tSize; }

	//属性のGet
	int GetCornerCount(void) { return nCornerCount; }

	//体力のGet
	float GetHp(void) { return m_fHp; }

	//シールドのGet
	float GetShield(void) { return m_fShield; }
	void SetShield(float InShield) { m_fShield = InShield; }

	//攻撃力のSet
	void SetAtk(float InAtk) { m_fAtk = InAtk; }
	//攻撃力のGet
	float GetAtk(void) { return m_fAtk; }
	//攻撃力のAdd
	void AddAtk(float InAtk) { m_fAtk += InAtk; }

	//攻撃タイプのGet
	AtkType GetAtkType(void) { return m_tAtkType; }

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
	CAlly(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress);		//コンストラクタ
	~CAlly();						//デストラクタ

	void Update(void)	override;	//更新処理
	void Draw(void)		override;	//描画処理

private:
	void CreateUpdate(void) override;
	void BattleUpdate(void) override;
	void DeathUpdate(void)  override;
	
	void SettingStatus(void) override;
};

class CAllyPlayer
{
public:
	CAllyPlayer(float InSize, CVector3<float>FirstPos, Camera* InAddress);
	~CAllyPlayer();

	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	void CreateUpdate(void);
	void BattleUpdate(void);
	void DeathUpdate(void);

	void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize);
private:
	Status m_tStatus;				//ステータス状態
	CVector3<float> m_tPos;			//位置座標
	CVector3<float> m_tSize;		//サイズ
	float m_fHp;					//体力

	Camera* m_pCamera;
	Sprite* m_pSprite;

	/*＝＝＝＝＝プレイヤーアニメーション系＝＝＝＝＝*/
private:
	CVector2<float> m_tTexPos;
public:
	CVector2<float> GetPosTex(int nSplitX, int nSplitY, int nAnimationSwap);
	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/

public:
	//ステータスのSet
	void SetStatus(Status InStatus) { m_tStatus = InStatus; }
	//ステータスのget
	Status GetStatus(void) { return m_tStatus; }

	//位置座標のGet
	CVector3<float> GetPos(void) { return m_tPos; }

	//サイズのGet
	CVector3<float> GetSize(void) { return m_tSize; }

	float GetHp(void) { return m_fHp; }

	//カメラのアドレス設定
	void SetCameraAddress(Camera* InAddress) { m_pCamera = InAddress; }
};

//敵キャラクタークラス
class CEnemy : public CFighter
{
public:
	CEnemy(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress);		//コンストラクタ
	~CEnemy();						//デストラクタ

	void Update(void)	override;	//更新処理
	void Draw(void)		override;	//描画処理

private:
	void CreateUpdate(void) override;
	void BattleUpdate(void) override;
	void DeathUpdate(void)  override;

	void SettingStatus(void) override;
};

class CEnemyBoss : public CFighter
{
private:
	enum BossType
	{
		Type1,
		Type2,
		Type3,
	};
public:
	CEnemyBoss(int BossTypeNum, float InSize, CVector3<float> FirstPos, Camera* InAddress);	//コンストラクタ
	~CEnemyBoss();
	
	void Update(void)	override;	//更新処理
	void Draw(void)		override;	//描画処理
private:
	void CreateUpdate(void) override;
	void BattleUpdate(void) override;
	void DeathUpdate(void)  override;

	void SettingStatus(void) override;

};

//味方バッファークラス
class CAllyBuffer
{
public:
	enum BuffType
	{
		BT_Shield,
		BT_Attack,
		BT_ReSummon,
	};
private:
	enum Corner
	{
		Hexagon = 6,		//六角形
		Heptagon = 7,		//七角形
		Octagon = 8,		//八角形
	};
public:
	CAllyBuffer(int InCornerCount, float InSize, CVector3<float> FirstPos, Camera* InAddress);		//コンストラクタ
	~CAllyBuffer();						//デストラクタ

	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	void CreateUpdate(void);
	void BattleUpdate(void);
	void DeathUpdate(void);

	void SettingStatus(void);
	void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize);
public:
	bool IsBuff;					//Buffをかける処理をもう行ったかどうか
private:
	Status m_tStatus;				//ステータス状態
	BuffType m_tBuff;				//バフの種類	
	Corner nCornerCount;			//属性
	CVector3<float> m_tPos;			//位置座標
	CVector3<float> m_tSize;		//サイズ

	Camera* m_pCamera;
	Sprite* m_pSprite;
public:
	//ステータスのSet
	void SetStatus(Status InStatus) { m_tStatus = InStatus; }
	//ステータスのget
	Status GetStatus(void) { return m_tStatus; }

	//属性のGet
	int GetCornerCount(void) { return nCornerCount; }

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

	BuffType GetBuffType(void) { return m_tBuff; }

	//カメラのアドレス設定
	void SetCameraAddress(Camera* InAddress) { m_pCamera = InAddress; }
};