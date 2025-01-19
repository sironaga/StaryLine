#include "DebugRoom.h"
#include "Input.h"
#include "Controller.h"
#include "Main.h"
#include "ShaderList.h"
#include "Geometory.h"

#define SIZECONTROLL_DEADZONE 10000.0f
#define SIZESPEED_AJUST 1,600.0f

constexpr float LIMIT_POS_X = 100.0f;
constexpr float LIMIT_POS_Y = 100.0f;
constexpr float LIMIT_SIZE_X = 100.0f;
constexpr float LIMIT_SIZE_Y = 100.0f;

constexpr float SIZE_SPEED = 10.0f;

const char* ch_pModelList[CDebugRoom::MAX_DEBUGMODEL] =
{
	MODEL_PASS("Leader/Linie/Anim_Linie_WandON.fbx")
};

CDebugRoom::CDebugRoom()
	: m_pDebugModel{}
	, m_nSelect(0), m_bSelect(false)
	, m_Param{}
	, m_pField(nullptr)
{
	m_Param.Size = { 1.0f,1.0f,1.0f };
	for (int i = 0; i < MAX_DEBUGMODEL; i++)
	{
		m_pDebugModel[i] = new Model();
		if (!m_pDebugModel[i]->Load(ch_pModelList[i]))MessageBox(NULL, ch_pModelList[i], "DebugRoom.cpp", MB_OK);
		Model::AnimeNo anim = m_pDebugModel[i]->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Linie_WandON.fbx"));
		m_pDebugModel[i]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
		m_pDebugModel[i]->PlayAnime(anim, true);
	}
}

CDebugRoom::~CDebugRoom()
{
	for (int i = 0; i < MAX_DEBUGMODEL; i++)
	{
		SAFE_DELETE(m_pDebugModel[i]);
	}
}

void CDebugRoom::Update()
{
	//m_pCamera->Update();
	for (int i = 0; i < MAX_DEBUGMODEL; i++)
	{
		m_pDebugModel[i]->Step(0.01f);
	}

	if (IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_THUMB))m_bSelect = true;

	if (IsKeyTrigger('L') || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		m_nSelect++;
		m_bSelect = true;
	}
	else if (IsKeyTrigger('J') || CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		m_nSelect--;
		m_bSelect = true;
	}
	if (m_nSelect < 0) m_nSelect = MAX_DEBUGMODEL - 1;
	else if (m_nSelect >= MAX_DEBUGMODEL) m_nSelect = 0;

	if (IsKeyPress('W'))m_Param.Pos.y++;
	if (IsKeyPress('A'))m_Param.Pos.x--;
	if (IsKeyPress('S'))m_Param.Pos.y--;
	if (IsKeyPress('D'))m_Param.Pos.x++;
	if (IsKeyPress(VK_RIGHT))	m_Param.Rotate.x+= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress(VK_LEFT))	m_Param.Rotate.x-= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress(VK_UP))		m_Param.Rotate.y+= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress(VK_DOWN))	m_Param.Rotate.y-= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress(VK_SPACE))	m_Param.Rotate.z+= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress(VK_SHIFT))	m_Param.Rotate.z-= DirectX::XMConvertToRadians(1.0f);
	if (IsKeyPress('E'))
	{
		m_Param.Size.x+=0.1f;
		m_Param.Size.y+=0.1f;
		m_Param.Size.z+=0.1f;
	}
	if (IsKeyPress('Q'))
	{
		m_Param.Size.x-=0.1f;
		m_Param.Size.y-=0.1f;
		m_Param.Size.z-=0.1f;
	}

	if (m_bSelect)
	{
		m_Param.Pos = { 0.0f,0.0f,0.0f };
		m_Param.Rotate = { 0.0f,0.0f,0.0f };
		m_Param.Size = { 1.0f,1.0f,1.0f };
		m_bSelect = false;
	}
}

void CDebugRoom::Draw()
{
	for (int i = 0; i < CDebugRoom::MAX_DEBUGMODEL; i++)
	{
		DrawModel(m_pDebugModel[i], m_Param,true);
	}
}

void CDebugRoom::DrawModel(Model* model, param InParam , bool isAnim)
{
	SetRender3D();
	//シェーダーへ変換行列を設定
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(InParam.Pos.x, InParam.Pos.y, InParam.Pos.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(InParam.Rotate.x, InParam.Rotate.y, InParam.Rotate.z, 0.0f));
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InParam.Size.x, InParam.Size.y, InParam.Size.z);
	DirectX::XMMATRIX mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mat);
	wvp[0] = world;
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);
	ShaderList::SetWVP(wvp);	//引数にはXMFLOAT4x4型の、要素数３の配列のアドレスを渡すこと

	//モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	if (isAnim)model->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	else model->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	model->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));


	//複数のメッシュで構成されている場合ある部分は金属的な表現、ある部分は非金属的な表現と
	//分ける場合がある前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
	//切り替える
	for (int i = 0; i < model->GetMeshNum(); ++i)
	{
		//メッシュに割り当てられているマテリアルを取得
		Model::Material material = *model->GetMaterial(model->GetMesh(i)->materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		//シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る
		if (isAnim)
		{
			DirectX::XMFLOAT4X4 bones[200];
			for (int j = 0; j < model->GetMesh(i)->bones.size(); ++j)
			{
				DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
					model->GetMesh(i)->bones[j].invOffset * model->GetBoneMatrix(model->GetMesh(i)->bones[j].nodeIndex)
				));
				ShaderList::SetBones(bones);
			}
		}

		//モデルの描画
		model->Draw(i);
	}
#ifdef _DEBUG
	//model->DrawBone();
#endif
}
