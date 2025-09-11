/*==============================================================
* 
*  File：Ally.cpp
* 
*  編集者：宇留野陸斗｜[管理]
* 
*  編集者：小川蓮　　｜[作成]
*  
*  編集者：中嶋飛賀　｜[ 音 ]
*
================================================================*/


#include "Ally.h"

//味方クラスのコンストラクタ
CAlly::CAlly(int InCornerCount, bool IsStellaBuff)
	:CFighter(InCornerCount, IsStellaBuff)
{
	// 兵士タイプの設定
	m_eFighterType = FighterType::Ally;

	//サイズの初期化
	m_tSize.x = (float)MODEL_DEFAULTSIZE::Ally;
	m_tSize.y = (float)MODEL_DEFAULTSIZE::Ally;
	m_tSize.z = (float)MODEL_DEFAULTSIZE::Ally;

	//初期ステータス決め
	SettingStatus();
	//Hpのポインタの作成
	m_pHpGage = new CHpUI(m_fHp, CHpUI::Ally);
	//角数ごとに同期するポインタを変更

	//角数ごとにモデルのポインタを同期
	switch (m_nCornerCount)
	{
	case 3:
		//モデル
		for (int i = 0; i < (int)CharacterAnimation::MAX; i++)
		{
			m_pModel[i] = CCharacterManager::GetInstance()->GetAllyModel((int)Ally::Ally3, i);
		}
		//攻撃エフェクトのポインタ同期
		m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::SwordAtk));
		break;
	case 4:
		//モデル
		for (int i = 0; i < (int)CharacterAnimation::MAX; i++)
		{
			m_pModel[i] = CCharacterManager::GetInstance()->GetAllyModel((int)Ally::Ally4, i);
		}
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

}

//味方クラスのデストラクタ
CAlly::~CAlly()
{

}

//味方クラスの更新処理
void CAlly::Update(void)
{
	//モデルの再読み込みフラグが立っていたら読み込む
	if (m_bReLoadFlag)
	{
		//モデルの破棄
		if (m_pModel[(int)m_eModelNo])m_pModel[(int)m_eModelNo] = nullptr;
		//エフェクトの破棄
		for (int i = 0; i < (int)FighterEffect::MAX; i++)
		{
			if (m_pEffect[i])
			{
				//エフェクトの破棄
				m_pEffect[i] = nullptr;
			}
		}

		//角数別にモデルのポインタを同期
		switch (m_nCornerCount)
		{
		case 3:
			//モデル
			for (int i = 0; i < (int)CharacterAnimation::MAX; i++)
			{
				m_pModel[i] = CCharacterManager::GetInstance()->GetAllyModel((int)Ally::Ally3, i);
			}

			//攻撃エフェクトのポインタ同期
			m_pEffect[(int)FighterEffect::Attack] = new CEffectManager_sp(CCharacterManager::GetInstance()->GetCharacterEffect((int)CharactersEffect::SwordAtk));
			break;
		case 4:
			//モデル
			for (int i = 0; i < (int)CharacterAnimation::MAX; i++)
			{
				m_pModel[i] = CCharacterManager::GetInstance()->GetAllyModel((int)Ally::Ally4, i);
			}

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

	//オーラエフェクト
	if (m_pEffect[(int)FighterEffect::Aura])
	{
		//オーラエフェクトの位置を設定
		m_pEffect[(int)FighterEffect::Aura]->SetPos({ m_tPos.x,m_tPos.y,m_tPos.z });
		//オーラエフェクトが再生されていなかったら再生する
		if (!m_pEffect[(int)FighterEffect::Aura]->IsPlay())
		{
			//オーラエフェクトの回転を設定
			m_pEffect[(int)FighterEffect::Aura]->SetRotate({ 0.0f,0.0f,0.0f });
			//オーラエフェクトのサイズを設定
			m_pEffect[(int)FighterEffect::Aura]->SetSize({ 15.0f,10.0f,10.0f });
			//オーラエフェクトの再生
			m_pEffect[(int)FighterEffect::Aura]->Play(true);
		}
	}

	// 基底クラスの更新処理(共通処理)
	CFighter::Update();
}

void CAlly::Draw(void)
{
	// 基底クラスの描画処理(共通処理)
	CFighter::Draw();

	//死亡フラグが立っていたら
	if (m_bIsDeath)
	{
		//モデルを死亡アニメーションモデルに変更
		m_eModelNo = CharacterAnimation::Death;
	}
	//攻撃フラグが立っていたら
	else if (m_bIsAttack)
	{
		//攻撃アニメーションモデルに変更
		//攻撃タイプ別に処理
		if (m_bTypeAttack)
			//攻撃アニメーションモデルを優位に変更
			m_eModelNo = CharacterAnimation::Attack_Advantage;
		else
			//攻撃アニメーションモデルを劣位に変更
			m_eModelNo = CharacterAnimation::Attack_Disadvantage;
	}
	else
	{
		//移動アニメーションモデルに変更
		m_eModelNo = CharacterAnimation::Walk;
	}
	//アニメーションモデルがnullじゃなかったら
	if (m_pModel[(int)m_eModelNo])
	{
		//キャラクターの描画
		SetRender3D();
		DirectX::XMFLOAT4X4 wvp[3];
		DirectX::XMMATRIX world;
		//行列(Scaling)
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x, m_tPos.y, m_tPos.z, 0.0f));
		//拡大縮小行列(Scaling)
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.x, m_tSize.y, m_tSize.z);
		//回転行列(Rotation)
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(90.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
		//それぞれの行列を掛け合わせて格納
		DirectX::XMMATRIX mat = S * R * T;

		world = mat;
		//ワールド行列を転置して格納
		DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
		wvp[1] = GetView();
		wvp[2] = GetProj();

		//ビュー行列を設定
		Geometory::SetView(wvp[1]);
		//プロジェクション行列を設定
		Geometory::SetProjection(wvp[2]);

		//ワールド行列をシェーダーに設定
		ShaderList::SetWVP(wvp);

		if (m_pModel[(int)m_eModelNo]->IsAnimePlay(0))
			//頂点シェーダをアニメーションモデル用に設定
			m_pModel[(int)m_eModelNo]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
		else
			//頂点シェーダを通常モデル用に設定
			m_pModel[(int)m_eModelNo]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		//ピクセルシェーダをトゥーンシェーダに設定
		m_pModel[(int)m_eModelNo]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

		const Model::Mesh* tMesh;

		for (int i = 0; i < m_pModel[(int)m_eModelNo]->GetMeshNum(); ++i)
		{
			//メッシュのポインタを取得
			tMesh = m_pModel[(int)m_eModelNo]->GetMesh(i);
			//マテリアルのポインタを取得
			Model::Material material = *m_pModel[(int)m_eModelNo]->GetMaterial(tMesh->materialID);
			//マテリアルの色を設定
			material.ambient.x = 0.85f; // x (r) 
			material.ambient.y = 0.85f; // y (g) 
			material.ambient.z = 0.85f; // z (b) 
			//シェーダーにマテリアルを設定
			ShaderList::SetMaterial(material);

			// ボーンの情報をシェーダーに送る
			DirectX::XMFLOAT4X4 bones[200];
			//ボーンの数だけループ
			for (int j = 0; j < tMesh->bones.size(); ++j)
			{
				//ボーンの行列をシェーダーに送る
				DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
					tMesh->bones[j].invOffset * m_pModel[(int)m_eModelNo]->GetBoneMatrix(tMesh->bones[j].nodeIndex)
				));
			}
			//シェーダーにボーンの情報を設定
			ShaderList::SetBones(bones);

			//モデルの描画
			if (m_pModel[(int)m_eModelNo]) {
				m_pModel[(int)m_eModelNo]->Draw(i);
			}
		}

	}
}

/*
* @brief 生成更新処理
*/
void CAlly::CreateUpdate(void)
{
	//生成エフェクト
	if (!m_pEffect[(int)FighterEffect::Create]->IsPlay() && !IsCreateEffectPlay && m_bFirstBattlePosSetting)
	{
		m_pEffect[(int)FighterEffect::Create]->SetPos({ m_tPos.x,m_tPos.y,m_tPos.z + m_tSize.z / 2 });
		m_pEffect[(int)FighterEffect::Create]->SetSize({ 15.0f,15.0f,15.0f });
		m_pEffect[(int)FighterEffect::Create]->SetRotate({ 0.0f,0.0f,0.0f });
		m_pEffect[(int)FighterEffect::Create]->Play();
		CCharacterManager::GetInstance()->GetSourceSummon(0)->FlushSourceBuffers();
		XAUDIO2_BUFFER buffer;
		buffer = CCharacterManager::GetInstance()->GetSummonSound(0)->GetBuffer(false);
		CCharacterManager::GetInstance()->GetSourceSummon(0)->SubmitSourceBuffer(&buffer);
		SetVolumeSE(CCharacterManager::GetInstance()->GetSourceSummon(0));
		CCharacterManager::GetInstance()->GetSourceSummon(0)->Start();
		IsCreateEffectPlay = true;
	}

	// 基底クラスの生成更新処理(共通処理)
	CFighter::CreateUpdate();
}

/*
* @brief 戦闘更新処理
*/
void CAlly::BattleUpdate(void)
{
	//攻撃していたら
	if (m_bIsAttack)
	{
		//攻撃チャージが攻撃アニメーションの時間を引いたMAX数値分たまっているかどうか
		if (m_fAtkCharge >= m_fAtkChargeMax - m_fAtkAnimationMaxTime)
		{
			//アニメーションの時間の加算
			m_fAtkAnimationTime++;
			//アニメーションの時間が最大以上になったら初期化する
			if (m_fAtkAnimationMaxTime <= m_fAtkCharge)m_fAtkAnimationTime = 0;
		}
		//攻撃エフェクト
		if (!m_pEffect[(int)FighterEffect::Attack]->IsPlay() && !IsAttackEffectPlay)
		{
			//攻撃エフェクトの位置を設定
			m_pEffect[(int)FighterEffect::Attack]->SetPos({ m_tTargetPos.x ,m_tTargetPos.y,m_tTargetPos.z });
			//角数別に回転を設定
			if (m_nCornerCount == 3)
				m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,TORAD(180.0f),0.0f });
			else
				m_pEffect[(int)FighterEffect::Attack]->SetRotate({ 0.0f,0.0f,0.0f });
			//攻撃エフェクトのサイズを設定
			m_pEffect[(int)FighterEffect::Attack]->SetSize({ 10.0f,10.0f,10.0f });
			//攻撃エフェクトの再生
			m_pEffect[(int)FighterEffect::Attack]->Play();
			//攻撃エフェクトの再生フラグを立てる
			IsAttackEffectPlay = true;
		}
		//攻撃音
		if (!m_bTimeSoundStart)
		{
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
//死亡更新処理
void CAlly::DeathUpdate(void)
{
	//死亡アニメーション
	m_bIsDeath = true;

	// 基底クラスの死亡更新処理(共通処理)
	CFighter::DeathUpdate();
}