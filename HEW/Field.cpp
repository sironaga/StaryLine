#include "Field.h"
#include "Defines.h"
#include "Geometory.h"
#include "ShaderList.h"
#include "CameraDebug.h"

Field::Field()
	:m_Field_Model{}
{
	for (int i = 0; i < MAX_MODEL; i++)
	{
		m_pModel[i] = new Model();
	}
	m_pCamera = new CameraDebug();
	for (int i = 0; i < MAX_MODEL; i++)
	{
		switch (i)
		{
		case TILELOW_FOREST:if (!m_pModel[i]->Load("Assets/Model/KayKit Mini-Game Variety Pack 1.2/Models/tileLow_forest.fbx", 0.5f/*倍率指定 */, Model::None/*反転設定*/))MessageBox(NULL, "Ground", "Error", MB_OK);
			break;
		default:
			break;
		}
	}

	RenderTarget* pRTV = GetDefaultRTV(); // デフォルトのRenderTargetViewを取得 
	DepthStencil* pDSV = GetDefaultDSV(); // デフォルトのDepthStencilViewを取得
	SetRenderTargets(1, &pRTV, pDSV); //第３引数がnullの場合、２D表示となる

	FieldModelInit(m_Field_Model, MAX_FIELD_MODEL);
}

Field::~Field()
{
	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (m_pModel[i]) {
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}
	if (m_pCamera) {
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

void Field::FieldModelInit(FieldModel* InModel, int MaxModel)
{
	FieldModel* pModel;
	pModel = InModel;
	//ファイル読み込み
	for (int i = 0, j = 0; i < MaxModel; i++, pModel++)
	{
		pModel->m_Pos = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
		pModel->m_Angle = DirectX::XMFLOAT3(TORAD(0), TORAD(0), TORAD(0));
		pModel->m_Scale = DirectX::XMFLOAT3(200.0f, 2.0f, 200.0f);
		pModel->m_Use = true;
		pModel->m_Model = TILELOW_FOREST;
	}
}

void Field::Update()
{
	FieldModelUpdate(m_Field_Model, MAX_FIELD_MODEL);
	m_pCamera->Update();
}

void Field::Draw()
{
	ModelDraw(m_Field_Model, MAX_FIELD_MODEL);
}

void Field::ModelDraw(FieldModel* InModel, int MaxModel)
{
	FieldModel* pModel;
	pModel = InModel;
	for (int i = 0; i < MaxModel; i++, pModel++)
	{
		if (!pModel->m_Use)continue;//使用しているモデルのみ処理

		//頂点シェーダーに渡す変換行列を作成
		DirectX::XMFLOAT4X4 wvp[3];
		DirectX::XMMATRIX world, view, proj;

		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(pModel->m_Pos.x, pModel->m_Pos.y, pModel->m_Pos.z); //移動行列;   // 天面がグリッドよりも下に来るように移動 
		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(pModel->m_Angle.x);//X軸回転行列;
		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(pModel->m_Angle.y);//Y軸回転行列;
		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(pModel->m_Angle.z);//Z軸回転行列;
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(pModel->m_Scale.x, pModel->m_Scale.y, pModel->m_Scale.z);//拡大縮小行列; // 地面となるように、前後左右に広く、上下に狭くする
		DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T; // それぞれの行列を掛け合わせて格納(基本的にこの順番で掛ける)

		world = mat;

		// 計算用のデータから読み取り用のデータに変換
		DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
		//DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
		//DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));
		wvp[1] = m_pCamera->GetViewMatrix();
		wvp[2] = m_pCamera->GetProjectionMatrix();
		Geometory::SetView(wvp[1]);
		Geometory::SetProjection(wvp[2]);

		// シェーダーへ変換行列を設定
		ShaderList::SetWVP(wvp); // 引数にはXMFLOAT4X4型の、要素数３の配列のアドレスを渡すこと

		//ライト変更
		ShaderList::SetLight(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -5.0f, 0.0f));//課題の時
		
		// モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
		m_pModel[pModel->m_Model]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModel[pModel->m_Model]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		// 複数のメッシュで構成されている場合、ある部分は金属的な表現、ある部分は非金属的な表現と
		//  分ける場合がある。前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
		//  切り替える。
		for (int j = 0; j < m_pModel[pModel->m_Model]->GetMeshNum(); j++) {
			//  モデルのメッシュを取得
			Model::Mesh mesh = *m_pModel[pModel->m_Model]->GetMesh(j);
			//  メッシュに割り当てられているマテリアルを取得
			Model::Material material = *m_pModel[pModel->m_Model]->GetMaterial(mesh.materialID);
			//  シェーダーへマテリアルを設定
			ShaderList::SetMaterial(material);
			//  モデルの描画
			m_pModel[pModel->m_Model]->Draw(j);
		}
	}
}

void Field::FieldModelUpdate(FieldModel* InModel, int MaxModel)
{
	//フィールドの更新
}
