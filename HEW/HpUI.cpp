/*==============================================================
* 
*  File:：HpUI.cpp
* 
*  編集者：宇留野陸斗｜[作成][管理]
* 
================================================================*/

#include "HpUI.h"


/*
* @brief コンストラクタ
* @param FullHp 体力の最大値
* @param Number UIのタイプ番号
*/
CHpUI::CHpUI(float FullHp, HpUINumber Number)
	:m_fNowHp(FullHp)
	, m_fFullHp(FullHp)
	, m_tUIPos()
	, m_tUIScale()
	, m_tNumber(Number)
	, m_fAnchorPoint()

{
	//スプライトを作成
	m_pSprite = new Sprite();

	//タイプ番号別に処理
	switch (m_tNumber)
	{
	case CHpUI::Ally://味方兵
		m_tUIScale.y = 0.8f;  //Y軸サイズ
		m_tUIScale.z = 0.5f;  //Z軸サイズ
		m_fAnchorPoint = 2.5f; //位置調整設定
		break;
	case CHpUI::Enemy://敵兵
		m_tUIScale.y = 0.8f;  //Y軸サイズ
		m_tUIScale.z = 0.5f;  //Z軸サイズ
		m_fAnchorPoint = 4.0f; //位置調整設定
		break;
	case CHpUI::Bos: //ボス
	case CHpUI::Player: //プレイヤー
		m_tUIScale.y = 4.0f; //Y軸サイズ
		m_tUIScale.z = 1.0f; //Z軸サイズ
		m_fAnchorPoint = 8.0f; //位置調整
		break;
	}
}

/*
* @brief デストラクタ
*/
CHpUI::~CHpUI()
{
	//スプライトの破棄
	SAFE_DELETE(m_pSprite);
}

/*
* @brief 更新処理
* @param InHp 体力
* @param InPos UIの位置
* @param InSizeY UIのY軸サイズ
*/
void CHpUI::Update(float InHp, DirectX::XMFLOAT3 InPos, float InSizeY)
{
	int HpRatio = 0;
	//Hpの値を更新
	m_fNowHp = InHp;

	//UIタイプ番号別に処理
	switch (m_tNumber)
	{
	case CHpUI::Ally://味方兵
		HpRatio = 4.0f;
		//味方兵のHPゲージのサイズを設定
		m_tUIScale.x = HpRatio;
		//味方兵のHPゲージの位置を設定
		m_tUIPos.x = InPos.x - m_tUIScale.x + (m_fNowHp / m_fFullHp) * m_tUIScale.x;
		m_tUIPos.y = InPos.y + InSizeY - 1.0f;
		m_tUIPos.z = InPos.z;
		break;
	case CHpUI::Enemy://敵兵
		HpRatio = 4.0f;
		//敵兵のHPゲージのサイズを設定
		m_tUIScale.x = HpRatio;
		//敵兵のHPゲージの位置を設定
		m_tUIPos.x = InPos.x + m_tUIScale.x - (m_fNowHp / m_fFullHp) * m_tUIScale.x;
		m_tUIPos.y = InPos.y + InSizeY - 1.0f;
		m_tUIPos.z = InPos.z;
		break;
	case CHpUI::Bos://ボス
		HpRatio = 102.0f;
		//ボスのHPゲージのサイズを設定
		m_tUIScale.x = HpRatio;
		//ボスのHPゲージの位置を設定
		m_tUIPos.x = ((InPos.x + 20.6f) + m_tUIScale.x - (m_fNowHp / m_fFullHp) * m_tUIScale.x) - 47.0f;
		m_tUIPos.y = -5.0;
		m_tUIPos.z = InPos.z;
		break;
	case CHpUI::Player://プレイヤー
		HpRatio = 102.0f;
		//プレイヤーのHPゲージのサイズを設定
		m_tUIScale.x = HpRatio;
		//プレイヤーのHPゲージの位置を設定
		m_tUIPos.x = ((InPos.x - 8.6f) + (m_tUIScale.x - (m_fNowHp / m_fFullHp) * m_tUIScale.x) * -1) + 47.0f;
		m_tUIPos.y = -5.0;
		m_tUIPos.z = InPos.z;
		break;
	}

}

/*
* @brief 描画処理
* @param nCornerCount 画数の数
*/
void CHpUI::Draw(int nCornerCount)
{
	SetRender2D();
	//UIのタイプ番号別に処理
	switch (m_tNumber)
	{
	case CHpUI::Ally://味方兵
		//現在のHPが最大HPと異なる場合
		if (m_fFullHp != m_fNowHp)
		{
			//ゲージの描画
			if (nCornerCount == 3)
			{
				//テクスチャを設定
				m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetHpGageTex((int)HpTexture::Ally3));
			}
			else if (nCornerCount == 4)
			{
				//テクスチャを設定
				m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetHpGageTex((int)HpTexture::Ally4));
			}
			//色を設定
			m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			//UV位置を設定
			m_pSprite->SetUVPos({ 1.0f - (m_fNowHp / m_fFullHp),0.0f });
			//UVサイズを設定
			m_pSprite->SetUVScale({ 2.0f ,1.0f });
			//描画設定
			DrawSetting(m_tUIPos, m_tUIScale, m_pSprite);
			//描画
			m_pSprite->Draw();
			//設定を初期化
			m_pSprite->ReSetSprite();
		}
		break;
	case CHpUI::Enemy:
		//現在のHPが最大HPと異なる場合
		if (m_fFullHp != m_fNowHp)
		{
			//ゲージの描画
			if (nCornerCount == 3)
			{
				//テクスチャを設定
				m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetHpGageTex((int)HpTexture::Enemy1));
			}
			else if (nCornerCount == 4)
			{
				//テクスチャを設定
				m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetHpGageTex((int)HpTexture::Enemy2));
			}

			//色を設定
			m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

			//UV位置を設定
			m_pSprite->SetUVPos({ 1.0f - (m_fNowHp / m_fFullHp),0.0f });
			//UVサイズを設定
			m_pSprite->SetUVScale({ 2.0f ,1.0f });
			//描画設定
			DrawSetting(m_tUIPos, m_tUIScale, m_pSprite);
			//描画
			m_pSprite->Draw();
			//設定を初期化
			m_pSprite->ReSetSprite();
		}
		break;
	case CHpUI::Bos:
		//ーーゲージの描画
		//テクスチャの設定
		m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetHpGageTex((int)HpTexture::Boss));
		//色を設定
		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		//UV位置を設定
		m_pSprite->SetUVPos({ 1.0f - (m_fNowHp / m_fFullHp),0.0f });
		//UVサイズを設定
		m_pSprite->SetUVScale({ 1.0f ,1.0f });
		//描画設定
		DrawSetting({ m_tUIPos.x,m_tUIPos.y - 5.0f ,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);
		//描画
		m_pSprite->Draw();
		//設定を初期化
		m_pSprite->ReSetSprite();

		//ーー枠の描画
		//テクスチャの設定
		m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetHpGageTex((int)HpTexture::Frame));
		//色を設定
		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		//UV位置を設定
		m_pSprite->SetUVPos({ 0.0f,0.0f });
		//UVサイズを設定
		m_pSprite->SetUVScale({ 1.0f ,1.0f });
		//描画設定
		DrawSetting({ 0.0f ,-5.0f,0.0f }, { 216.0f,21.0f,10.0f }, m_pSprite);
		//描画
		m_pSprite->Draw();
		//設定を初期化
		m_pSprite->ReSetSprite();

		break;

	case CHpUI::Player:
		//ーーゲージの描画
		//テクスチャの設定
		m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetHpGageTex((int)HpTexture::Linie));
		//色を設定
		m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		//UV位置を設定
		m_pSprite->SetUVPos({ -1.0f + (m_fNowHp / m_fFullHp),0.0f });
		//UVサイズを設定
		m_pSprite->SetUVScale({ 1.0f ,1.0f });
		//描画設定
		DrawSetting({ m_tUIPos.x ,m_tUIPos.y - 5.0f,m_tUIPos.z + 0.1f }, m_tUIScale, m_pSprite);
		//描画
		m_pSprite->Draw();
		//設定を初期化
		m_pSprite->ReSetSprite();

		break;
	}
}