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
		if (!m_pModel->Load(MODEL_PASS("Stage/Stage1_Meadow.fbx"), 1.0f, Model::None))MessageBox(NULL, "Ground", "Error", MB_OK);
		m_Pos = DirectX::XMFLOAT3(30.0f, -3.0f, 40.0f);
		m_Angle = DirectX::XMFLOAT3(TORAD(0.0f), TORAD(180), TORAD(0));
		m_Scale = DirectX::XMFLOAT3(0.5f, 0.6f, 0.4f);
		break;
	case (int)EStageType::STAGE_DESERT:
		if (!m_pModel->Load(MODEL_PASS("Stage/Stage02_Desert.fbx"), 1.0f, Model::None))MessageBox(NULL, "Ground", "Error", MB_OK);
		m_Pos = DirectX::XMFLOAT3(30.0f, -8.0f, 40.0f);
		m_Angle = DirectX::XMFLOAT3(TORAD(-5), TORAD(180), TORAD(0));
		m_Scale = DirectX::XMFLOAT3(0.5f, 0.6f, 0.4f);
		break;
	case (int)EStageType::STAGE_SNOWFIELD:
		if (!m_pModel->Load(MODEL_PASS("Stage/Stage3_SnowField.fbx"), 1.0f, Model::None))MessageBox(NULL, "Ground", "Error", MB_OK);
		m_Pos = DirectX::XMFLOAT3(30.0f, 5.0f, 130.0f);
		m_Angle = DirectX::XMFLOAT3(TORAD(5), TORAD(180), TORAD(0));
		m_Scale = DirectX::XMFLOAT3(0.5f, 0.6f, 0.4f);
		break;
	}

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
	//陦悟・(Scaling)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f));
	//諡｡螟ｧ邵ｮ蟆剰｡悟・(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	//蝗櫁ｻ｢陦悟・(Rotation)
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_Angle.x);//X霆ｸ蝗櫁ｻ｢陦悟・;
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);//Y霆ｸ蝗櫁ｻ｢陦悟・;
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle.z);//Z霆ｸ蝗櫁ｻ｢陦悟・;
	//縺昴ｌ縺槭ｌ縺ｮ陦悟・繧呈寺縺大粋繧上○縺ｦ譬ｼ邏・
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

void Field::Reload(StageType StageNum)
{
	if (m_pModel) {
		delete m_pModel;
		m_pModel = nullptr;
	}
	m_pModel = new Model();

	switch (StageNum.StageMainNumber)
	{
	case (int)EStageType::STAGE_GRASSLAND:
		if (!m_pModel->Load(MODEL_PASS("Stage/Stage1_Meadow.fbx"), 1.0f, Model::None))MessageBox(NULL, "Ground", "Error", MB_OK);
		m_Pos = DirectX::XMFLOAT3(30.0f, -12.0f, 40.0f);
		m_Angle = DirectX::XMFLOAT3(TORAD(-5), TORAD(180), TORAD(0));
		m_Scale = DirectX::XMFLOAT3(0.5f, 0.6f, 0.4f);
		break;
	case (int)EStageType::STAGE_DESERT:
		if (!m_pModel->Load(MODEL_PASS("Stage/Stage02_Desert.fbx"), 1.0f, Model::None))MessageBox(NULL, "Ground", "Error", MB_OK);
		m_Pos = DirectX::XMFLOAT3(30.0f, -12.0f, 40.0f);
		m_Angle = DirectX::XMFLOAT3(TORAD(-5), TORAD(180), TORAD(0));
		m_Scale = DirectX::XMFLOAT3(0.5f, 0.6f, 0.4f);
		break;
	case (int)EStageType::STAGE_SNOWFIELD:
		if (!m_pModel->Load(MODEL_PASS("Stage/Stage3_SnowField.fbx"), 1.0f, Model::None))MessageBox(NULL, "Ground", "Error", MB_OK);
		m_Pos = DirectX::XMFLOAT3(30.0f, 5.0f, 130.0f);
		m_Angle = DirectX::XMFLOAT3(TORAD(5), TORAD(180), TORAD(0));
		m_Scale = DirectX::XMFLOAT3(0.5f, 0.6f, 0.4f);
		break;
	}
}
