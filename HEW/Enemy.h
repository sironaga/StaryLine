/*==============================================================
* 
*  File：Enemy.h
* 
*  編集者：宇留野陸斗｜[管理]
* 
*  編集者：小川蓮	 ｜[作成] 
* 
================================================================*/

#pragma once
#include "CharacterManager.h"
#include "Fighter.h"

//敵キャラクタークラス
class CEnemy : public CFighter
{
public:
	//コンストラクタ
	CEnemy(int InCornerCount);

	//デストラクタ
	~CEnemy();

	//更新処理
	void Update(void)	override;

	//描画処理
	void Draw(void)		override;

private:
	//生成モードの更新処理
	void CreateUpdate(void) override;

	//戦闘モードの更新処理
	void BattleUpdate(void) override;

	//死亡モードの更新処理
	void DeathUpdate(void)  override;
};
