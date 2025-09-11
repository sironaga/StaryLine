/*==============================================================
* 
*  File：Fighter.cpp
* 
*  編集者：宇留野陸斗｜[作成][管理]
* 　
*　編集者：中嶋飛賀　｜[ 音 ]
*　
================================================================*/

#include "Fighter.h"


/*
* @brief コンストラクタ
* @param InCornerCount 角数
* @param IsStellaBuff ステラバフの有無
*/
CFighter::CFighter(int InCornerCount, bool IsStellaBuff)
	: m_tStatus(Status:: Create)			// ステータスの初期化
	, m_tSearchCollision{ 0.0f,0.0f }		// 索敵当たり判定の初期化
	, m_tAtkCollision{ 0.0f,0.0f }			// 攻撃当たり判定の初期化
	, m_tPos{ 0.0f, 0.0f, 0.0f }			// 位置座標の初期化
	, m_tFirstPos{ 0.0f,0.0f,0.0f }			// 初期位置座標の初期化
	, m_tSize{ 0.0f,0.0f,0.0f }				// サイズの初期化
	, m_nCornerCount(InCornerCount)			// 角数の初期化
	, m_fHp(0.0f)							// 体力の初期化
	, m_fAtk(0.0f)							// 攻撃力の初期化
	, m_fAtkCharge(0.0f)					// 攻撃チャージの初期化
	, m_fAtkChargeMax(0.0f)					// 攻撃チャージの最大値の初期化
	, m_fAtkAnimationTime(0.0f)				// 攻撃アニメーションの時間の初期化
	, m_fAtkAnimationMaxTime(0.0f)			// 攻撃アニメーションの最大時間の初期化
	, m_bIsHit(false)						// 被弾フラグの初期化
	, m_pHpGage{}							// 体力ゲージのポインタの初期化
	, m_nTargetNumber(-1)					// 標的の選別番号の初期化
	, m_bIsAttack(false)					// 攻撃フラグの初期化
	, m_bFirstBattlePosSetting(false)		// 戦闘シーンの開始位置に移動したかどうかの初期化
	, m_bMoveFlag(false)					// 移動フラグの初期化
	, m_pSourceNormalAttack(nullptr)		// 攻撃音のポインタの初期化
	, m_fTimeSound(0)						// 攻撃音の再生時間の初期化
	, m_bTimeSoundStart(false)				// 攻撃音の再生フラグの初期化
	, m_pModel{ nullptr }					// モデルのポインタの初期化
	, m_tDestinationPos()					// 目的地の初期化
	, m_bReLoadFlag(false)					// モデルの再読み込みフラグの初期化
	, m_pEffect{ nullptr }					// エフェクトのポインタの初期化
	, IsCreateEffectPlay(false)				// 生成エフェクトの再生フラグの初期化
	, IsAttackEffectPlay(false)				// 攻撃エフェクトの再生フラグの初期化
	, IsDeathEffectPlay(false)				// 死亡エフェクトの再生フラグの初期化
	, m_tTargetPos()						// 標的の位置の初期化
	, m_bMoveUp(false)						// 上移動フラグの初期化
	, m_bStellaBuff(IsStellaBuff)			// ステラバフの初期化
	, m_bDamageLogDraw{ false }				// ダメージログ描画関連のフラグの初期化
	, m_eModelNo(CharacterAnimation::None)	// モデルの初期化
	, m_bIsDeath(false)						// 死亡フラグの初期化
{
	//サウンドの設定
	m_pSourceNormalAttack = CCharacterManager::GetInstance()->GetNormalAttackSound()->m_sound->CreateSourceVoice(m_pSourceNormalAttack);
	XAUDIO2_BUFFER buffer = CCharacterManager::GetInstance()->GetNormalAttackSound()->GetBuffer(false);
	m_pSourceNormalAttack->SubmitSourceBuffer(&buffer);
	m_pSourceWeaknessAttack = CCharacterManager::GetInstance()->GetWeaknessAttackSound()->m_sound->CreateSourceVoice(m_pSourceWeaknessAttack);
	XAUDIO2_BUFFER buffer2 = CCharacterManager::GetInstance()->GetWeaknessAttackSound()->GetBuffer(false);
	m_pSourceWeaknessAttack->SubmitSourceBuffer(&buffer2);
}

/*
* @brief デストラクタ
*/
CFighter::~CFighter()
{
	//Hpクラスポインタの破棄
	SAFE_DELETE(m_pHpGage);
	//サウンドの破棄
	if (m_pSourceNormalAttack)
	{
		m_pSourceNormalAttack->ExitLoop();
		m_pSourceNormalAttack->Stop();
		m_pSourceNormalAttack->DestroyVoice();
		m_pSourceNormalAttack = nullptr;
	}
	if (m_pSourceWeaknessAttack)
	{
		m_pSourceWeaknessAttack->ExitLoop();
		m_pSourceWeaknessAttack->Stop();
		m_pSourceWeaknessAttack->DestroyVoice();
		m_pSourceWeaknessAttack = nullptr;
	}

	//モデルの破棄
	for (int i = 0; i < (int)CharacterAnimation::MAX; i++)
	{
		if (m_pModel)
		{
			m_pModel[i] = nullptr;
		}
	}
	//エフェクトの破棄
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
		{
			m_pEffect[i] = nullptr;
		}
	}
}

/*
* @brief 更新処理
*/
void CFighter::Update(void)
{
	// 状況に応じて処理を分ける
	switch (m_tStatus)
	{
		//生成更新
	case Status::Create:CreateUpdate();		break;
		//戦闘更新
	case Status::Battle:BattleUpdate();		break;
		//死亡更新
	case Status::Death:	DeathUpdate();		break;
	}

	//エフェクトの更新
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
		{
			//エフェクトの更新
			m_pEffect[i]->Update();
		}
	}
}

/*
* @brief 描画処理
*/
void CFighter::Draw(void)
{
	//ダメージログの描画
	if (m_bDamageLogDraw[0])
	{
		if (m_bDamageLogDraw[1])
		{
			//ダメージログの移動
			m_fDamageLogMoveY = 0.0f;
			//ダメージログの移動フラグをfalse
			m_bDamageLogDraw[1] = false;
		}
		//ダメージログの描画
		DamageLogDraw(m_nCornerCount, m_bDamageLogDraw[2]);
		//ダメージログのY軸が5.0f未満だったら
		if (m_fDamageLogMoveY < 5.0f)
		{
			//ダメージログのY軸の位置を更新
			m_fDamageLogMoveY += 5.0f / 120.0f;
		}
		//ダメージログのY軸が5.0以上だったら
		else
		{
			//ダメージログの描画フラグをfalse
			m_bDamageLogDraw[0] = false;
			m_bDamageLogDraw[1] = false;
			m_bDamageLogDraw[2] = false;
		}
	}

	//体力ゲージの描画
	m_pHpGage->Draw(m_nCornerCount);

	//エフェクトの描画
	for (int i = 0; i < (int)FighterEffect::MAX; i++)
	{
		if (m_pEffect[i])
		{
			//エフェクトの描画
			m_pEffect[i]->Draw(true);
		}
	}
}

/*
* @brief 生成更新処理
*/
void CFighter::CreateUpdate(void)
{
	//生成アニメーションが終わったら
	if (!m_pEffect[(int)FighterEffect::Create]->IsPlay() && IsCreateEffectPlay && m_bFirstBattlePosSetting)
	{
		SetStatus(Status::Battle);
	}
}

/*
* @brief 戦闘更新処理
*/
void CFighter::BattleUpdate(void)
{
	//エフェクトのRe再生処理
	if (IsAttackEffectPlay)
	{
		if (!m_pEffect[(int)FighterEffect::Attack]->IsPlay())
		{
			//攻撃エフェクトの再生フラグをfalse
			IsAttackEffectPlay = false;
		}
	}
	//攻撃音の再生時間
	if (m_bTimeSoundStart)
	{
		//攻撃音の再生時間の加算
		m_fTimeSound++;
	}
	//攻撃音の再生時間が30を超えたら
	if (m_fTimeSound > 30)
	{
		//攻撃音の再生を止める
		m_pSourceNormalAttack->Stop();
		//攻撃音の再生フラグをfalse
		m_bTimeSoundStart = false;
		//攻撃音の再生時間を初期化
		m_fTimeSound = 0;
	}

	//HPUIの更新処理
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);
}

/*
* @brief 死亡更新処理
*/
void CFighter::DeathUpdate(void)
{
	//死亡エフェクト
	if (!m_pEffect[(int)FighterEffect::Death]->IsPlay() && !IsDeathEffectPlay)
	{
		//死亡エフェクトの位置を設定
		m_pEffect[(int)FighterEffect::Death]->SetPos(m_tPos);
		//死亡エフェクトの回転を設定
		m_pEffect[(int)FighterEffect::Death]->SetRotate({ 0.0f,0.0f,0.0f });
		//死亡エフェクトのサイズを設定
		m_pEffect[(int)FighterEffect::Death]->SetSize({ 20.0f,20.0f,20.0f });
		//死亡エフェクトの再生
		m_pEffect[(int)FighterEffect::Death]->Play();
		//死亡エフェクトの再生フラグを立てる
		IsDeathEffectPlay = true;
	}

	//死亡アニメーションが終わったら
	if (!m_pEffect[(int)FighterEffect::Death]->IsPlay() && IsDeathEffectPlay)
	{
		//ステータスを削除に変更
		SetStatus(Status::Delete);
	}
}


/*
* @brief 攻撃当たり判定のチェック
* @param InSize 相手のサイズ
* @param InPos 相手の位置
*/
bool CFighter::AtkCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos)
{
	/*自分の攻撃当たり判定の左端 <= 相手の左端*/
	if (m_tPos.x - m_tAtkCollision.Width <= InPos.x - (InSize.x / 2)
		&&
		/*自分の攻撃当たり判定の右端 >= 相手の左端*/
		m_tPos.x + m_tAtkCollision.Width >= InPos.x - (InSize.x / 2)
		||
		/*自分の攻撃当たり判定の左端 <= 相手の右端*/
		m_tPos.x - m_tAtkCollision.Width <= InPos.x + (InSize.x / 2)
		&&
		/*自分の攻撃当たり判定の右端 >= 相手の右端*/
		m_tPos.x + m_tAtkCollision.Width >= InPos.x + (InSize.x / 2))
	{
		/*自分の攻撃当たり判定の上端 <= 相手の下端*/
		if (m_tPos.y - m_tAtkCollision.Height <= InPos.y - (InSize.y / 2)
			&&
			/*自分の攻撃当たり判定の下端 >= 相手の下端*/
			m_tPos.y + m_tAtkCollision.Height >= InPos.y - (InSize.y / 2)
			||
			/*自分の攻撃当たり判定の上端 <= 相手の上端*/
			m_tPos.y - m_tAtkCollision.Height <= InPos.y + (InSize.y / 2)
			&&
			/*自分の攻撃当たり判定の下端 >= 相手の上端*/
			m_tPos.y + m_tAtkCollision.Height >= InPos.y + (InSize.y / 2))
		{
			/*自分の攻撃当たり判定の前端 <= 相手の前端*/
			if (m_tPos.z - m_tAtkCollision.Width <= InPos.z - (InSize.z / 2)
				&&
				/*自分の攻撃当たり判定の奥端 >= 相手の前端*/
				m_tPos.z + m_tAtkCollision.Width >= InPos.z - (InSize.z / 2)
				||
				/*自分の攻撃当たり判定の前端 <= 相手の奥端*/
				m_tPos.z - m_tAtkCollision.Width <= InPos.z + (InSize.z / 2)
				&&
				/*自分の攻撃当たり判定の奥端 >= 相手の奥端*/
				m_tPos.z + m_tAtkCollision.Width >= InPos.z + (InSize.z / 2))
			{
				return true;
			}
		}
	}
	return false;
}

/*
* @brief 索敵当たり判定のチェック
* @param InSize 相手のサイズ
* @param InPos 相手の位置
*/
bool CFighter::SearchCollisionCheck(DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InPos)
{
	/*自分の索敵当たり判定の左端 <= 相手の左端*/
	if (m_tPos.x - m_tSearchCollision.Width <= InPos.x - InSize.x
		&&
		/*自分の索敵当たり判定の右端 >= 相手の左端*/
		m_tPos.x + m_tSearchCollision.Width >= InPos.x - InSize.x
		||
		/*自分の索敵当たり判定の左端 <= 相手の右端*/
		m_tPos.x - m_tSearchCollision.Width <= InPos.x + InSize.x
		&&
		/*自分の索敵当たり判定の右端 >= 相手の右端*/
		m_tPos.x + m_tSearchCollision.Width >= InPos.x + InSize.x)
	{
		/*自分の索敵当たり判定の上端 <= 相手の上端*/
		if (m_tPos.y - m_tSearchCollision.Height <= InPos.y - InSize.y
			&&
			/*自分の索敵当たり判定の下端 >= 相手の上端*/
			m_tPos.y + m_tSearchCollision.Height >= InPos.y - InSize.y
			||
			/*自分の索敵当たり判定の上端 <= 相手の下端*/
			m_tPos.y - m_tSearchCollision.Height <= InPos.y + InSize.y
			&&
			/*自分の索敵当たり判定の下端 >= 相手の下端*/
			m_tPos.y + m_tSearchCollision.Height >= InPos.y + InSize.y)
		{
			/*自分の索敵当たり判定の前端 <= 相手の前端*/
			if (m_tPos.z - m_tSearchCollision.Width <= InPos.z - InSize.z
				&&
				/*自分の索敵当たり判定の奥端 >= 相手の前端*/
				m_tPos.z + m_tSearchCollision.Width >= InPos.z - InSize.z
				||
				/*自分の索敵当たり判定の前端 <= 相手の奥端*/
				m_tPos.z - m_tSearchCollision.Width <= InPos.z + InSize.z
				&&
				/*自分の索敵当たり判定の奥端 >= 相手の奥端*/
				m_tPos.z + m_tSearchCollision.Width >= InPos.z + InSize.z)
			{
				return true;
			}
		}
	}
	return false;
}

/*
* @brief 重なり判定
* @param InPos 相手の位置
* @param InSize 相手のサイズ
*/
bool CFighter::OverlapCheck(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize)
{
	//サイズを重なり用に調整
	DirectX::XMFLOAT3 Size;
	Size.x = m_tSize.x * 1.5f;
	Size.y = m_tSize.y * 1.5f;
	Size.z = m_tSize.z * 1.5f;

	//相手のサイズを重なり用に調整
	InSize.x = InSize.x * 1.5f;
	InSize.y = InSize.y * 1.5f;
	InSize.z = InSize.z * 1.5f;

	/*自分の左端 <= 相手の左端*/
	if (m_tPos.x - Size.x <= InPos.x - InSize.x
		&&
		/*自分の右端 >= 相手の左端*/
		m_tPos.x + Size.x >= InPos.x - InSize.x
		||
		/*自分の左端 <= 相手の右端*/
		m_tPos.x - Size.x <= InPos.x + InSize.x
		&&
		/*自分の右端 >= 相手の右端*/
		m_tPos.x + Size.x >= InPos.x + InSize.x)
	{
		/*自分の上端 <= 相手の上端*/
		if (m_tPos.y - Size.y <= InPos.y - InSize.y
			&&
			/*自分の下端 >= 相手の上端*/
			m_tPos.y + Size.y >= InPos.y - InSize.y
			||
			/*自分の上端 <= 相手の下端*/
			m_tPos.y - Size.y <= InPos.y + InSize.y
			&&
			/*自分の下端 >= 相手の下端*/
			m_tPos.y + Size.y >= InPos.y + InSize.y)
		{
			/*自分の前端 <= 相手の前端*/
			if (m_tPos.z - Size.x <= InPos.z - InSize.z
				&&
				/*自分の奥端 >= 相手の前端*/
				m_tPos.z + Size.x >= InPos.z - InSize.z
				||
				/*自分の前端 <= 相手の奥端*/
				m_tPos.z - Size.x <= InPos.z + InSize.z
				&&
				/*自分の奥端 >= 相手の奥端*/
				m_tPos.z + Size.x >= InPos.z + InSize.z)
			{
				return true;
			}
		}
	}
	return false;
}

/*
* @brief 攻撃を受けた時の処理
* @param pFighter 攻撃者のポインタ
*/
void CFighter::Damage(CFighter* pFighter)
{
	//相手の角数が自分と同じだったら
	if (pFighter->GetCornerCount() == m_nCornerCount)
	{
		//相手の攻撃力分体力を減らす
		m_fHp -= pFighter->GetAtk();
		//当たった判定をオンにする
		m_bIsHit = true;
		//ダメージログの描画フラグをオンにする
		pFighter->m_bDamageLogDraw[0] = true;
		pFighter->m_bDamageLogDraw[1] = true;
		pFighter->m_bDamageLogDraw[2] = true;
		//ダメージログの位置を設定
		pFighter->m_tDamageLogPos.x = m_tPos.x;
		pFighter->m_tDamageLogPos.y = m_tPos.y - 1.0f;
		pFighter->m_tDamageLogPos.z = m_tPos.z;
	}
	else
	{
		//相手の攻撃力分体力を減らす
		m_fHp -= pFighter->GetAtk() * 0.3f;
		//当たった判定をオンにする
		m_bIsHit = true;
		//ダメージログの描画フラグをオンにする
		pFighter->m_bDamageLogDraw[0] = true;
		pFighter->m_bDamageLogDraw[1] = true;
		pFighter->m_bDamageLogDraw[2] = false;
		//ダメージログの位置を設定
		pFighter->m_tDamageLogPos.x = m_tPos.x;
		pFighter->m_tDamageLogPos.y = m_tPos.y - 1.0f;
		pFighter->m_tDamageLogPos.z = m_tPos.z;
	}
}

/*
* @brief ダメージログの描画
* @param nCornerCount 角数
* @param Type ダメージログの種類
*/
void CFighter::DamageLogDraw(int nCornerCount, bool Type)
{
	SetRender2D();
	//タイプ別にテクスチャを変更
	if (Type)
	{
		switch (nCornerCount)
		{
		case 3:
			m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetDamageTex((int)DamageDraw::NormalRed));
			break;
		case 4:
			m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetDamageTex((int)DamageDraw::NormalBlue));
			break;
		}
	}
	else
	{
		m_pSprite->SetTexture(CCharacterManager::GetInstance()->GetDamageTex((int)DamageDraw::NotQuite));
	}

	//色を設定
	m_pSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	//UV位置を設定
	m_pSprite->SetUVPos({ 0.0f, 0.0f });

	//UVサイズを設定
	m_pSprite->SetUVScale({ 1.0f ,1.0f });

	//描画設定
	DrawSetting({ m_tDamageLogPos.x ,m_tDamageLogPos.y + m_fDamageLogMoveY,m_tDamageLogPos.z + 0.2f }, { 5.0f,5.0f,5.0f }, m_pSprite);

	//描画
	m_pSprite->Draw();

	//設定を初期化
	m_pSprite->ReSetSprite();

}

/*
* @brief ステータスの設定
*/
void CFighter::SettingStatus(void)
{
	//角数別に分岐
	switch (m_nCornerCount)
	{
	default:
		//体力
		m_fHp = 2500.0f;
		//攻撃力
		m_fAtk = 150.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 10.0f + m_fAtkAnimationMaxTime;
		//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);
		//キャラクターの中心からの縦の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);
		break;
		/*三角形*/
	case (int)Corner::Triangle:
		//体力
		m_fHp = 2500.0f;
		//攻撃力
		m_fAtk = 150.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 30.0f + m_fAtkAnimationMaxTime;
		//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);
		//キャラクターの中心からの縦の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);
		break;
		/*四角形*/
	case (int)Corner::Square:
		//体力
		m_fHp = 2500.0f;
		//攻撃力
		m_fAtk = 150.0f;
		//攻撃アニメーションの時間
		m_fAtkAnimationMaxTime = 0.0f;
		//攻撃チャージの最大値をアニメーションの時間を加算した値にする
		m_fAtkChargeMax = 30.0f + m_fAtkAnimationMaxTime;
		//キャラクターの中心からの横の攻撃当たり判定
		m_tAtkCollision.Width = MAX_CHARACTER_ATK_COLLISION_WIDTH(1);
		//キャラクターの中心からの縦の攻撃当たり判定
		m_tAtkCollision.Height = MAX_CHARACTER_ATK_COLLISION_HEIGHT(1);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Width = MAX_CHARACTER_SEARCH_COLLISION_WIDTH(10);
		//キャラクターの中心からの横の索敵当たり判定
		m_tSearchCollision.Height = MAX_CHARACTER_SEARCH_COLLISION_HEIGHT(10);
		break;
	}
}
