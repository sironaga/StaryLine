/*==============================================================
* 
*  File：Ally.h
* 
*  編集者：宇留野陸斗｜[管理]
* 
*  編集者：小川蓮　　｜[作成]
*  
*  編集者：中嶋飛賀　｜[ 音 ]
*
================================================================*/

#pragma once
#include "CharacterManager.h"
#include "Fighter.h"


//味方キャラクタークラス
class CAlly : public CFighter
{
public:
	//コンストラクタ
	CAlly(int InCornerCount, bool IsStellaBuff);

	//デストラクタ		
	~CAlly();

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
