/*==============================================================
* 
*  File：Enemy.cpp
* 
*  編集者：宇留野陸斗｜[管理]
* 
*  編集者：小川蓮　　｜[作成]
* 　
*　編集者：中嶋飛賀　｜[ 音 ]
*　
================================================================*/

#include "Enemy.h"

/*
* @brief コンストラクタ
* @param InCornerCount 角数
*/
CEnemy::CEnemy(int InCornerCount)
	:CFighter(InCornerCount)
{
	// 兵士タイプの設定
	m_eFighterType = FighterType::Enemy;

	//サイズの初期化
	m_tSize.x = (float)MODEL_DEFAULTSIZE::Enemy;
	m_tSize.y = (float)MODEL_DEFAULTSIZE::Enemy;
	m_tSize.z = (float)MODEL_DEFAULTSIZE::Enemy;
	//初期ステータス決め
	SettingStatus();
	//Hpのポインタの作成
	m_pHpGage = new CHpUI(m_fHp, CHpUI::Enemy);
	//角数ごとに同期するポインタを変更
	switch (m_nCornerCount)
	{
	case 3:
		//モデル
		m_pModel[0] = CCharacterManager::GetInstance()->GetEnemyModel((int)Enemy::Enemy1);

		//攻撃エフェクトのポインタ同期
		m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::SwordAtk));
		break;
	case 4:
		//モデル
		m_pModel[0] = CCharacterManager::GetInstance()->GetEnemyModel((int)Enemy::Enemy2);

		//攻撃エフェクトのポインタ同期
		m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::BowAtk));
		break;
	}
	//--エフェクトのポインタ同期
	//生成
	m_pEffect[(int)FighterEffect::Create] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::Create));
	//死亡
	m_pEffect[(int)FighterEffect::Death] = new CEffectManager_sp( CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::Death));
}

/*
* @brief デストラクタ
*/
CEnemy::~CEnemy()
{

}

/*
* @brief 更新処理
*/
void CEnemy::Update(void)
{
	//モデルの再読み込みフラグが立っていたら読み込む
	if (m_bReLoadFlag)
	{
		//モデルの破棄
		if (m_pModel[0])m_pModel[0] = nullptr;
		//エフェクトの破棄
		for (int i = 0; i < (int)FighterEffect::MAX; i++)
		{
			if (m_pEffect[i])
			{
				m_pEffect[i] = nullptr;
			}
		}
		//角数別にモデルのポインタを同期
		switch (m_nCornerCount)
		{
		case 3:
			//モデル
			m_pModel[0] = CCharacterManager::GetInstance()->GetEnemyModel((int)Enemy::Enemy1);

			//攻撃エフェクトのポインタ同期
			m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::SwordAtk));
			break;
		case 4:
			//モデル
			m_pModel[0] = CCharacterManager::GetInstance()->GetEnemyModel((int)Enemy::Enemy2);

			//攻撃エフェクトのポインタ同期
			m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::BowAtk));
			break;
		}
		//--エフェクトのポインタ同期
		//生成
		m_pEffect[(int)FighterEffect::Create] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::Create));
		//死亡
		m_pEffect[(int)FighterEffect::Death] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::Death));
		//オーラ
		if (m_bStellaBuff)
		{
			m_pEffect[(int)FighterEffect::Aura] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::Aura));
		}
		else
		{
			m_pEffect[(int)FighterEffect::Aura] = nullptr;
		}
		//モデルの再読み込みフラグをfalse
		m_bReLoadFlag = false;
	}

	// 基底クラスの更新処理(共通処理)
	CFighter::Update();
}

/*
* @brief 描画処理
*/
void CEnemy::Draw(void)
{
	// 基底クラスの描画処理(共通処理)
	CFighter::Draw();

	//キャラクターの描画
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x, m_tPos.y, m_tPos.z, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.x, m_tSize.y, m_tSize.z);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;
	//ワールド行列を転置して格納
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	//ワールド行列を設定
	Geometory::SetView(wvp[1]);
	//ビュー行列を設定
	Geometory::SetProjection(wvp[2]);
	//スプライトにワールド行列を設定
	ShaderList::SetWVP(wvp);
	//頂点シェーダーを設定
	m_pModel[0]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	//ピクセルシェーダーを設定
	m_pModel[0]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	//モデルのメッシュのポインタ
	for (int i = 0; i < m_pModel[0]->GetMeshNum(); ++i)
	{
		//メッシュのポインタを取得
		Model::Material material = *m_pModel[0]->GetMaterial(m_pModel[0]->GetMesh(i)->materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		//シェーダーにマテリアルを設定
		ShaderList::SetMaterial(material);

		if (m_pModel[0]) {
			//モデルの描画
			m_pModel[0]->Draw(i);
		}
	}
}

/*
* @brief 生成更新処理
*/
void CEnemy::CreateUpdate(void)
{
	//生成エフェクト
	if (!m_pEffect[(int)FighterEffect::Create]->IsPlay() && !IsCreateEffectPlay && m_bFirstBattlePosSetting)
	{
		//エフェクトの位置を設定
		m_pEffect[(int)FighterEffect::Create]->SetPos(m_tPos);
		//エフェクトのサイズを設定
		m_pEffect[(int)FighterEffect::Create]->SetSize({ 15.0f,15.0f,15.0f });
		//エフェクトの回転を設定
		m_pEffect[(int)FighterEffect::Create]->SetRotate({ 0.0f,0.0f,0.0f });
		//エフェクトの再生
		m_pEffect[(int)FighterEffect::Create]->Play();

		CCharacterManager::GetInstance()->GetSourceSummon(1)->FlushSourceBuffers();
		XAUDIO2_BUFFER buffer;
		buffer = CCharacterManager::GetInstance()->GetSummonSound(1)->GetBuffer(false);
		CCharacterManager::GetInstance()->GetSourceSummon(1)->SubmitSourceBuffer(&buffer);
		SetVolumeSE(CCharacterManager::GetInstance()->GetSourceSummon(1));
		CCharacterManager::GetInstance()->GetSourceSummon(1)->Start();
		//エフェクトの再生フラグを立てる
		IsCreateEffectPlay = true;
	}

	// 基底クラスの生成更新処理(共通処理)
	CFighter::CreateUpdate();
}

/*
* @brief 戦闘更新処理
*/
void CEnemy::BattleUpdate(void)
{
	//攻撃フラグが立っていたら
	if (m_bIsAttack)
	{
		//攻撃チャージが攻撃アニメーションの時間を引いたMAX数値分たまっているかどうか
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{
			//アニメーションの時間の加算
			m_fAtkAnimationTime++;
			//アニメーションの時間が最大以上になったら初期化する
			if (m_fAtkAnimationMaxTime == m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
		//攻撃エフェクト
		if (!m_pEffect[(int)FighterEffect::Attack]->IsPlay() && !IsAttackEffectPlay)
		{
			if (m_tTargetPos.x != 0.0f && m_tTargetPos.y != 0.0f && m_tTargetPos.z != 0.0f)
			{
				//攻撃エフェクトの位置を設定
				m_pEffect[(int)FighterEffect::Attack]->SetPos({ m_tTargetPos.x ,m_tTargetPos.y,m_tTargetPos.z });
				//角数別に回転を設定
				if (m_nCornerCount == 3)
					m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,0.0f,0.0f });
				else
					m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,0.0f,0.0f });
				//攻撃エフェクトのサイズを設定
				m_pEffect[(int)FighterEffect::Attack]->SetSize({ 10.0f,10.0f,10.0f });
				//攻撃エフェクトの再生
				m_pEffect[(int)FighterEffect::Attack]->Play();
				//攻撃エフェクトの再生フラグを立てる
				IsAttackEffectPlay = true;
			}
		}
		//攻撃音
		if (!m_bTimeSoundStart)
		{
			//攻撃音の再生フラグが立っていたら
			if (m_bTypeAttack)
			{
				XAUDIO2_BUFFER buffer = CCharacterManager::GetInstance()->GetNormalAttackSound()->GetBuffer(false);
				m_pSourceNormalAttack->FlushSourceBuffers();
				m_pSourceNormalAttack->SubmitSourceBuffer(&buffer);
				if (m_pSourceNormalAttack)SetVolumeSE(m_pSourceNormalAttack);
				m_pSourceNormalAttack->Start();
				m_bTimeSoundStart = true;
			}
			else
			{
				XAUDIO2_BUFFER buffer = CCharacterManager::GetInstance()->GetWeaknessAttackSound()->GetBuffer(false);
				m_pSourceWeaknessAttack->FlushSourceBuffers();
				m_pSourceWeaknessAttack->SubmitSourceBuffer(&buffer);
				if (m_pSourceWeaknessAttack)SetVolumeSE(m_pSourceWeaknessAttack);
				m_pSourceWeaknessAttack->Start();
				m_bTimeSoundStart = true;
			}
		}
	}

	// 基底クラスの戦闘更新処理(共通処理)
	CFighter::BattleUpdate();
}

/*
* @brief 死亡更新処理
*/
void CEnemy::DeathUpdate(void)
{
	// 基底クラスの死亡更新処理(共通処理)
	CFighter::DeathUpdate();
}