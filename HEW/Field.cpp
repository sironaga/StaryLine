#include "Field.h"
#include "Defines.h"
#include "Geometory.h"
#include "ShaderList.h"
#include "CameraDebug.h"
#include "Main.h"

Field::Field(StageType StageNum)
{
		m_pModel = new Model();

		switch (StageNum.StageMainNumber)
		{
		case (int)EStageType::STAGE_GRASSLAND:
			if (!m_pModel->Load(MODEL_PASS("Stage/Stage02_Desert.fbx"), 1.0f/*倍率指定 */, Model::XFlip/*反転設定*/))MessageBox(NULL, "Ground", "Error", MB_OK);
			break;
		case (int)EStageType::STAGE_DESERT:
			if (!m_pModel->Load(MODEL_PASS("Stage/Stage02_Desert.fbx"), 1.0f/*倍率指定 */, Model::XFlip/*反転設定*/))MessageBox(NULL, "Ground", "Error", MB_OK);
			break;
		case (int)EStageType::STAGE_SNOWFIELD:
			if (!m_pModel->Load(MODEL_PASS("Stage/Stage03_.fbx"), 1.0f/*倍率指定 */, Model::XFlip/*反転設定*/))MessageBox(NULL, "Ground", "Error", MB_OK);
			break;
		}

		m_Pos = DirectX::XMFLOAT3(-60.0f, -12.0f, 40.0f);
		m_Angle = DirectX::XMFLOAT3(TORAD(-5), TORAD(180), TORAD(0));
		m_Scale = DirectX::XMFLOAT3(0.5f, 0.6f, 0.4f);
}

Field::~Field()
{
	if (m_pModel) {
		delete m_pModel;
		m_pModel = nullptr;
	}
}

void Field::Update()
{
	//FieldModelUpdate(m_Field_Model, MAX_FIELD_MODEL);
}

void Field::Draw()
{
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	//行列(Scaling)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	//回転行列(Rotation)
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_Angle.x);//X軸回転行列;
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);//Y軸回転行列;
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle.z);//Z軸回転行列;
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
	{
		Model::Material material = *m_pModel->GetMaterial(m_pModel->GetMesh(i)->materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		if (m_pModel) {
			m_pModel->Draw(i);
		}
	}

	//ModelDraw(m_Field_Model, MAX_FIELD_MODEL);
}

//void Field::ModelDraw(FieldModel* InModel, int MaxModel)
//{
//	FieldModel* pModel;
//	pModel = InModel;
//	for (int i = 0; i < MaxModel; i++, pModel++)
//	{
//		if (!m_Use)continue;//使用しているモデルのみ処理
//
//		//頂点シェーダーに渡す変換行列を作成
//		DirectX::XMFLOAT4X4 wvp[3];
//		DirectX::XMMATRIX world, view, proj;
//
//		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z); //移動行列;   // 天面がグリッドよりも下に来るように移動 
//		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_Angle.x);//X軸回転行列;
//		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);//Y軸回転行列;
//		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle.z);//Z軸回転行列;
//		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);//拡大縮小行列; // 地面となるように、前後左右に広く、上下に狭くする
//		DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T; // それぞれの行列を掛け合わせて格納(基本的にこの順番で掛ける)
//
//		world = mat;
//
//		// 計算用のデータから読み取り用のデータに変換
//		DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
//		//DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
//		//DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));
//
//		wvp[1] = GetView();// 修正してください
//		wvp[2] = GetProj();// 修正してください
//
//		Geometory::SetView(wvp[1]);
//		Geometory::SetProjection(wvp[2]);
//
//		// シェーダーへ変換行列を設定
//		ShaderList::SetWVP(wvp); // 引数にはXMFLOAT4X4型の、要素数３の配列のアドレスを渡すこと
//
//		//ライト変更
//		ShaderList::SetLight(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -5.0f, 0.0f));//課題の時
//		
//		// モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
//		m_pModel[m_Model]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
//		m_pModel[m_Model]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));
//
//		// 複数のメッシュで構成されている場合、ある部分は金属的な表現、ある部分は非金属的な表現と
//		//  分ける場合がある。前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
//		//  切り替える。
//		for (int j = 0; j < m_pModel[m_Model]->GetMeshNum(); j++) 
//		{
//			//  メッシュに割り当てられているマテリアルを取得
//			Model::Material material = *m_pModel[m_Model]->GetMaterial(m_pModel[m_Model]->GetMesh(i)->materialID);
//
//			material.ambient.x = 0.85f; // x (r) 
//			material.ambient.y = 0.85f; // y (g) 
//			material.ambient.z = 0.85f; // z (b) 
//
//			//  シェーダーへマテリアルを設定
//			ShaderList::SetMaterial(material);
//			//  モデルの描画
//			m_pModel[m_Model]->Draw(j);
//		}
//	}
//}
//
//void Field::FieldModelUpdate(FieldModel* InModel, int MaxModel)
//{
//	//フィールドの更新
//}
