/*==============================================================
* 
*  File：HpUI.h
* 
*  編集者：宇留野陸斗｜[作成][管理]
* 
================================================================*/
#pragma once

#include "CharacterManager.h"


//キャラクターのHPUIクラス
class CHpUI
{
public:
	//UIのタイプ番号
	enum HpUINumber
	{
		Ally,//味方
		Enemy,//敵
		Bos,//ボス
		Player,//プレイヤー
	};
public:
	//コンストラクタ
	CHpUI(float FullHp, HpUINumber Number);
	//デストラクタ
	~CHpUI();
	//更新処理
	void Update(float InHp, DirectX::XMFLOAT3 InPos, float InSizeY);
	//描画処理
	void Draw(int nCornerCount = 0);
private:
	//Hpの現在値
	float m_fNowHp;
	//Hpの最大値
	float m_fFullHp;
	//HpUI時の位置調整
	float m_fAnchorPoint;
	//スプライトのポインタ
	Sprite* m_pSprite;
	//UIのタイプ
	HpUINumber m_tNumber;
	//位置情報
	DirectX::XMFLOAT3 m_tUIPos;
	//サイズ情報
	DirectX::XMFLOAT3 m_tUIScale;
};
