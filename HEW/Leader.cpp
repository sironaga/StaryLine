/*==============================================================
* 
*  File：Leader.cpp
* 
*  編集者：宇留野陸斗｜[作成][管理]
* 
================================================================*/

#include "Leader.h"

/*
* @brief コンストラクタ
 * @param InSize サイズ
 * @param FirstPos 初期位置
 * @param InTextureNumber テクスチャ番号
 * @param SubModelCreate サブモデルの作成フラグ
 */
CLeader::CLeader(float InSize, DirectX::XMFLOAT3 FirstPos, int InTextureNumber, bool SubModelCreate)
	:m_tStatus(Status::Create)			//ステータスの初期化
	, m_tPos()							//位置座標
	, m_tSize()							//サイズ						
	, m_tSubPos()						//サブモデルの位置
	, m_tSubSize()						//サブモデルのサイズ
	, m_fHp(15000.0f)					//体力
	, m_fMaxHp(m_fHp)					//最大体力
	, m_nTextureNumber(InTextureNumber) //テクスチャ番号
	, m_pHpGage(nullptr)				//Hpゲージのポインタ
	, m_bSubModelCreate(SubModelCreate) //サブモデルの作成フラグ
	, m_bAnimationPlay(false)			//アニメーションの再生フラグ
	, m_bReLoadFlag(false)				//モデルの再読み込みフラグ
	, m_bWin(false)						//勝利フラグ
	, m_bDamage(false)					//ダメージフラグ	
	, m_bSummon(false)					//召喚フラグ
	, m_nModelNo(0)						//モデル番号
	, m_fDamageTimer(5.0f)				//ダメージタイマー
	, m_fWinTimer(0.0f)					//勝利タイマー
{
	//テクスチャ番号別にモデルのポインタを同期
	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		for (int i = 0; i < (int)LinieAnimation::MAX; i++)
		{
			m_pModel.push_back(CCharacterManager::GetInstance()->GetLinieModel(i));
		}
		m_pHpGage = new CHpUI(m_fHp, CHpUI::Player);
		break;
	case 1://ボス
		for (int i = 0; i < (int)BossAnimation::MAX; i++)
		{
			m_pModel.push_back(CCharacterManager::GetInstance()->GetBossModel(i, 0));

			//サブモデルの生成フラグが立っていたら
			if (m_bSubModelCreate)
			{
				m_pSubModel.push_back(CCharacterManager::GetInstance()->GetBossModel(i, 1));
			}
			else
			{
				m_pSubModel.push_back(nullptr);
			}
		}
		//Hpゲージのポインタの作成
		m_pHpGage = new CHpUI(m_fHp, CHpUI::Bos);
		break;

	}
	//テクスチャ番号別にメインモデルのサイズと位置設定
	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		//位置設定
		m_tPos.x = FirstPos.x;
		m_tPos.y = FirstPos.y;
		m_tPos.z = FirstPos.z - 5.0f;
		//サイズ設定
		m_tSize.x = InSize;
		m_tSize.y = InSize;
		m_tSize.z = InSize;
		//回転設定
		m_tRotate = { 0.0f,85.0f,0.0f };
		break;
	case 1://ボス
		//位置設定
		m_tPos.x = FirstPos.x - 12.0f;
		m_tPos.y = FirstPos.y;
		m_tPos.z = FirstPos.z - 5.0f;
		//サイズ設定
		m_tSize.x = InSize;
		m_tSize.y = InSize;
		m_tSize.z = InSize;
		break;
	}
	//サブモデルのサイズと位置設定
	//位置設定
	m_tSubPos.x = FirstPos.x - 15.0f;
	m_tSubPos.y = FirstPos.y + 0.5f;
	m_tSubPos.z = FirstPos.z + 5.0f;
	//サイズ設定
	m_tSubSize.x = InSize;
	m_tSubSize.y = InSize;
	m_tSubSize.z = InSize;
}

/*
* @brief デストラクタ
*/
CLeader::~CLeader()
{
	//Hpポインタの破棄
	SAFE_DELETE(m_pHpGage);
	//モデルのポインタの破棄
	m_pModel.clear();
	//サブモデルのポインタの破棄
	m_pSubModel.clear();
}

/*
* @brief 更新処理
* @param IsStart 戦闘開始フラグ
* @param IsEnd 戦闘終了フラグ
*/
void CLeader::Update(bool IsStart, bool IsEnd)
{
	//モデルの再読み込みフラグが立っていたら
	if (m_bReLoadFlag)
	{
		//テクスチャ番号別にモデルのポインタを同期
		switch (m_nTextureNumber)
		{
		case 0://プレイヤー
			for (int i = 0; i < (int)LinieAnimation::MAX; i++)
			{
				m_pModel.push_back(CCharacterManager::GetInstance()->GetLinieModel(i));
			}
			m_pHpGage = new CHpUI(m_fHp, CHpUI::Player);
			break;
		case 1://ボス
			for (int i = 0; i < (int)BossAnimation::MAX; i++)
			{
				m_pModel.push_back(CCharacterManager::GetInstance()->GetBossModel(i, 0));

				//サブモデルの生成フラグが立っていたら
				if (m_bSubModelCreate)
				{
					m_pSubModel.push_back(CCharacterManager::GetInstance()->GetBossModel(i, 1));
				}
				else
				{
					m_pSubModel.push_back(nullptr);
				}
			}
			//Hpゲージのポインタの作成
			m_pHpGage = new CHpUI(m_fHp, CHpUI::Bos);
			break;

		}
		//再読み込みフラグをfalseにする
		m_bReLoadFlag = false;
	}
	//Hpゲージの更新
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);

	//ステータス別に処理を分ける
	switch (m_tStatus)
	{
	case Status::Create:CreateUpdate(); break;
	case Status::Battle:BattleUpdate(IsStart, IsEnd); break;
	case Status::Death:DeathUpdate(); break;
	}
}

/*
* @brief 描画処理
* @param StageNum ステージ番号
*/
void CLeader::Draw(int StageNum)
{
	//テクスチャ番号別に描画処理
	switch (m_nTextureNumber)
	{
	case 0://プレイヤー
		//勝利フラグが立っていたら
		if (m_bWin)
		{
			//モデル番号を勝利アニメーションモデルにする
			m_nModelNo = (int)LinieAnimation::Win;
		}
		//召喚フラグが立っていたら
		else if (m_bSummon)
		{
			//モデル番号を召喚アニメーションモデルにする
			m_nModelNo = (int)LinieAnimation::Summon;
		}
		//ダメージフラグが立っているかダメージタイマーが1.5未満だったら
		else if (m_bDamage || m_fDamageTimer < 1.5f)
		{
			//モデル番号をダメージアニメーションモデルにする
			m_nModelNo = (int)LinieAnimation::Damage;
		}
		else
		{
			//モデル番号を待機アニメーションモデルにする
			m_nModelNo = (int)LinieAnimation::Idle;
		}

		//モデルのポインタがnullptrじゃなかったら
		if (m_pModel[m_nModelNo])
		{
			//アニメーションが再生中だったら
			if (m_pModel[m_nModelNo]->IsAnimePlay(0))
			{
				SetRender3D();
				DirectX::XMFLOAT4X4 wvp[3];
				DirectX::XMMATRIX world;
				//移動行列(Translation)
				DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x, m_tPos.y, m_tPos.z, 0.0f));
				//拡大縮小行列(Scaling)
				DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSize.x, m_tSize.y, m_tSize.z);
				//回転行列(Rotation)
				DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(m_tRotate.x), DirectX::XMConvertToRadians(m_tRotate.y), DirectX::XMConvertToRadians(m_tRotate.z), 0.0f));
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

				//頂点シェーダーを設定
				m_pModel[m_nModelNo]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
				//ピクセルシェーダーを設定
				m_pModel[m_nModelNo]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

				const Model::Mesh* tMesh;
				//メッシュの数だけループ
				for (int i = 0; i < m_pModel[m_nModelNo]->GetMeshNum(); ++i)
				{
					//メッシュのポインタを取得
					tMesh = m_pModel[m_nModelNo]->GetMesh(i);
					//マテリアルのポインタを取得
					Model::Material material = *m_pModel[m_nModelNo]->GetMaterial(tMesh->materialID);

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
						DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
							tMesh->bones[j].invOffset * m_pModel[m_nModelNo]->GetBoneMatrix(tMesh->bones[j].nodeIndex)
						));
					}
					//シェーダーにボーンの情報を設定
					ShaderList::SetBones(bones);


					if (m_pModel[m_nModelNo]) {
						//モデルの描画
						m_pModel[m_nModelNo]->Draw(i);
					}
				}
			}
		}

		break;
	case 1://ボス

		Model* pBosCarModel = CCharacterManager::GetInstance()->GetBosCar();

		if (pBosCarModel)
		{
			SetRender3D();
			DirectX::XMFLOAT4X4 wvp[3];
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX T, S, R;

			switch (StageNum)
			{
			case 0:
				T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x - 8.0f, m_tPos.y - 10.0f, m_tPos.z, 0.0f));
				//拡大縮小行列(Scaling)
				S = DirectX::XMMatrixScaling(0.4f, 0.4f, 0.4f);
				//回転行列(Rotation)
				R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
				break;
			case 1:
				T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x - 2.0f, m_tPos.y - 17.0f, m_tPos.z + 6.0f, 0.0f));
				//拡大縮小行列(Scaling)
				S = DirectX::XMMatrixScaling(0.4f, 0.4f, 0.4f);
				//回転行列(Rotation)
				R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
				break;
			case 2:
				T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tPos.x + 2.0f, m_tPos.y - 5.0f, m_tPos.z, 0.0f));
				//拡大縮小行列(Scaling)
				S = DirectX::XMMatrixScaling(0.35f, 0.35f, 0.35f);
				//回転行列(Rotation)
				R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(1.0f), DirectX::XMConvertToRadians(170.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
				break;
			}
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

			//頂点シェーダーを設定
			pBosCarModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
			//ピクセルシェーダーを設定
			pBosCarModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

			//メッシュのポインタを取得
			for (int i = 0; i < pBosCarModel->GetMeshNum(); ++i)
			{
				//マテリアルのポインタを取得
				Model::Material material = *pBosCarModel->GetMaterial(pBosCarModel->GetMesh(i)->materialID);

				material.ambient.x = 0.85f; // x (r) 
				material.ambient.y = 0.85f; // y (g) 
				material.ambient.z = 0.85f; // z (b) 

				//シェーダーにマテリアルを設定
				ShaderList::SetMaterial(material);

				if (pBosCarModel) {
					//モデルの描画
					pBosCarModel->Draw(i);
				}
			}
		}
		//ダメージフラグが立っているかダメージタイマーが1.5未満だったら
		if (m_bDamage || m_fDamageTimer < 1.5f)
		{
			//モデル番号をダメージアニメーションモデルにする
			m_nModelNo = (int)BossAnimation::Damage;
		}
		else
		{
			//モデル番号を待機アニメーションモデルにする
			m_nModelNo = (int)BossAnimation::Idle;
		}
		//モデルのポインタがnullptrじゃなかったら
		if (m_pModel[m_nModelNo])
		{
			//アニメーションが再生中だったら
			if (m_pModel[m_nModelNo]->IsAnimePlay(0))
			{
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

				//ビュー行列を設定
				Geometory::SetView(wvp[1]);
				//プロジェクション行列を設定
				Geometory::SetProjection(wvp[2]);
				//ワールド行列をシェーダーに設定
				ShaderList::SetWVP(wvp);

				//頂点シェーダーを設定
				m_pModel[m_nModelNo]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
				//ピクセルシェーダーを設定
				m_pModel[m_nModelNo]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

				const Model::Mesh* tMesh;

				//メッシュの数だけループ
				for (int i = 0; i < m_pModel[m_nModelNo]->GetMeshNum(); ++i)
				{
					//メッシュのポインタを取得
					tMesh = m_pModel[m_nModelNo]->GetMesh(i);
					//マテリアルのポインタを取得
					Model::Material material = *m_pModel[m_nModelNo]->GetMaterial(tMesh->materialID);
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
						DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
							tMesh->bones[j].invOffset * m_pModel[m_nModelNo]->GetBoneMatrix(tMesh->bones[j].nodeIndex)
						));
					}
					//シェーダーにボーンの情報を設定
					ShaderList::SetBones(bones);

					if (m_pModel[m_nModelNo]) {
						//モデルの描画
						m_pModel[m_nModelNo]->Draw(i);
					}
				}
			}
		}
		//サブモデルのポインタがnullptrじゃなかったら
		if (m_pSubModel[m_nModelNo])
		{
			//アニメーションが再生中だったら
			if (m_pSubModel[m_nModelNo]->IsAnimePlay(0))
			{
				SetRender3D();
				DirectX::XMFLOAT4X4 wvp[3];
				DirectX::XMMATRIX world;
				//移動行列(Translation)
				DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_tSubPos.x, m_tSubPos.y, m_tSubPos.z, 0.0f));
				//拡大縮小行列(Scaling)
				DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_tSubSize.x, m_tSubSize.y, m_tSubSize.z);
				//回転行列(Rotation)
				DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(265.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
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

				//頂点シェーダーを設定
				m_pSubModel[m_nModelNo]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
				//ピクセルシェーダーを設定
				m_pSubModel[m_nModelNo]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

				const Model::Mesh* tSubMesh;
				//メッシュの数だけループ
				for (int i = 0; i < m_pSubModel[m_nModelNo]->GetMeshNum(); ++i)
				{
					//メッシュのポインタを取得
					tSubMesh = m_pSubModel[m_nModelNo]->GetMesh(i);

					//マテリアルのポインタを取得
					Model::Material material = *m_pSubModel[m_nModelNo]->GetMaterial(tSubMesh->materialID);
					material.ambient.x = 0.85f; // x (r)
					material.ambient.y = 0.85f; // y (g)
					material.ambient.z = 0.85f; // z (b)

					//シェーダーにマテリアルを設定
					ShaderList::SetMaterial(material);

					// ボーンの情報をシェーダーに送る
					DirectX::XMFLOAT4X4 bones[200];
					//ボーンの数だけループ
					for (int j = 0; j < tSubMesh->bones.size(); ++j)
					{
						DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
							tSubMesh->bones[j].invOffset * m_pSubModel[m_nModelNo]->GetBoneMatrix(tSubMesh->bones[j].nodeIndex)
						));
					}
					//シェーダーにボーンの情報を設定
					ShaderList::SetBones(bones);

					if (m_pSubModel[m_nModelNo]) {
						//モデルの描画
						m_pSubModel[m_nModelNo]->Draw(i);
					}
				}
			}
		}
		break;
	}

	//ダメージフラグをfalseにする
	m_bDamage = false;
	//召喚フラグをfalseにする
	m_bSummon = false;
}

/*
* @brief 体力ゲージの描画処理
*/
void CLeader::HpDraw(void)
{
	//体力ゲージの描画
	m_pHpGage->Draw();
}

/*
* @brief ダメージを受ける処理
* @param pFighter 攻撃者のポインタ
*/
void CLeader::Damage(CFighter* pFighter)
{
	//ダメージフラグをtrueにする
	m_bDamage = true;
	//ダメージタイマーを0にする
	m_fDamageTimer = 0.0f;

	//体力を攻撃者の攻撃力分減らす
	m_fHp -= pFighter->GetAtk();
	//体力が0未満だったら
	if (m_fHp < 0)
	{
		//体力を0にする
		m_fHp = 0;
	}
	//ダメージログの描画フラグをオンにする
	pFighter->m_bDamageLogDraw[0] = true;
	pFighter->m_bDamageLogDraw[1] = true;
	pFighter->m_bDamageLogDraw[2] = false;
	//ダメージログの位置を設定
	pFighter->m_tDamageLogPos.x = m_tPos.x;
	pFighter->m_tDamageLogPos.y = m_tPos.y - 1.0f;
	pFighter->m_tDamageLogPos.z = m_tPos.z;
}

/*
* @brief 生成モードの更新処理
*/
void CLeader::CreateUpdate(void)
{
	//生成アニメーションが終わったら
	SetStatus(Status::Battle);
}

/*
* @brief 戦闘モードの更新処理
* @param IsStart 戦闘開始フラグ
* @param IsEnd 戦闘終了フラグ
*/
void CLeader::BattleUpdate(bool IsStart, bool IsEnd)
{
	//ダメージタイマーの更新
	m_fDamageTimer += 1.0f / fFPS;

	//勝利フラグが立っていたら
	if (m_bWin)
	{
		//勝利タイマーの更新
		m_fWinTimer += 1.0f / fFPS;
	}

	//HPUIの更新処理
	m_pHpGage->Update(m_fHp, { m_tPos.x,m_tPos.y,m_tPos.z }, m_tSize.y);
}

/*
* @brief 死亡モードの更新処理
*/
void CLeader::DeathUpdate(void)
{
	//死亡アニメーションが終わったら
	SetStatus(Status::Delete);
}

