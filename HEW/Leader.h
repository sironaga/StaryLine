/*==============================================================
* 
*  File：Leader.h
* 
*  編集者：宇留野陸斗｜[作成][管理]
* 
================================================================*/


#pragma once
#include "CharacterManager.h"
#include "Fighter.h"
#include "HpUI.h"

//リーダークラス
class CLeader
{
public:
	//コンストラクタ
	CLeader(float InSize, DirectX::XMFLOAT3 FirstPos, int InTextureNumber, bool SubModelCreate = false);

	//デストラクタ
	~CLeader();

	//更新処理
	void Update(bool IsStart = false, bool IsEnd = false);

	//描画処理
	void Draw(int StageNum = 0);

	//Hpの更新処理
	void HpDraw(void);

	//ダメージを受けた時の処理
	void Damage(CFighter* pFighter);
private:
	//生成モードの更新処理
	void CreateUpdate(void);
	//戦闘モードの更新処理
	void BattleUpdate(bool IsStart = false, bool IsEnd = false);
	//死亡モードの更新処理
	void DeathUpdate(void);

private:
	//勝利フラグ
	bool m_bWin;

	//召喚フラグ
	bool m_bSummon;

	//ダメージフラグ
	bool m_bDamage;

	//ダメージタイマー
	float m_fDamageTimer;

	//勝利タイマー
	float m_fWinTimer;

	//再読み込みフラグ
	bool m_bReLoadFlag;

	//アニメーションモデル番号
	int m_nModelNo;

	//ステータス状態
	Status m_tStatus;

	//モデルポインタ
	std::vector <Model*> m_pModel;

	//位置座標
	DirectX::XMFLOAT3 m_tPos;

	//サイズ
	DirectX::XMFLOAT3 m_tSize;

	//回転
	DirectX::XMFLOAT3 m_tRotate;

	//サブモデルの生成フラグ
	bool m_bSubModelCreate;

	//サブモデルのポインタ
	std::vector <Model*> m_pSubModel;

	//サブモデルの位置
	DirectX::XMFLOAT3 m_tSubPos;

	//サブモデルのサイズ
	DirectX::XMFLOAT3 m_tSubSize;

	//体力
	float m_fHp;	

	//最大体力
	float m_fMaxHp;

	//アニメーションの再生フラグ
	bool m_bAnimationPlay;

	//テクスチャ番号
	int m_nTextureNumber;

	//体力ゲージのポインタ
	CHpUI* m_pHpGage;

public:
	//勝利アニメーションタイマーのGet
	inline float GetWinTImer(void) { return m_fWinTimer; }

	//勝利アニメーションフラグのSet
	inline void SetWinFlag(bool Flag) { m_bWin = Flag; }

	//勝利アニメーションフラグのGet
	inline bool GetWinFlag(void) { return m_bWin; }

	//被弾アニメーションフラグのSet
	inline void SetDamageFlag(bool Flag) { m_bDamage = Flag; }

	//被弾アニメーションフラグのGet
	inline bool GetDamageFlag(void) { return m_bDamage; }

	//召喚アニメーションフラグのSet
	inline void SetSummonFlag(bool Flag) { m_bSummon = Flag; }

	//召喚アニメーションフラグのGet
	inline bool GetSummonFlag(void) { return m_bSummon; }

	//モデル読み込みフラグの切り替え
	inline void SetReLoadFlag(void) { m_bReLoadFlag = true; }

	//ステータスのSet
	inline void SetStatus(Status InStatus) { m_tStatus = InStatus; }

	//ステータスのget
	inline Status GetStatus(void) { return m_tStatus; }

	//位置座標のGet
	inline DirectX::XMFLOAT3 GetPos(void) { return m_tPos; }

	//サイズのGet
	inline DirectX::XMFLOAT3 GetSize(void) { return m_tSize; }

	//回転のSet
	inline void SetRotate(DirectX::XMFLOAT3 InRotate) { m_tRotate = InRotate; }

	//回転のGet
	inline DirectX::XMFLOAT3 GetRotate(void) { return m_tRotate; }

	//現在HpのGet
	inline float GetHp(void) { return m_fHp; }

	//最大HpのGet
	inline float GetMaxHp(void) { return m_fMaxHp; }

	//アニメーションの再生フラグのGet
	inline void SetAnimationPlayFlag(bool IsFlag) { m_bAnimationPlay = IsFlag; }
};