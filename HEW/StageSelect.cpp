#include "StageSelect.h"
#include "DirectXTex/TextureLoad.h"
#include "Main.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Defines.h"
#include "SpriteDrawer.h"
#include "InputEx.h"
#include "SoundList.h"
#include "SceneResult.h"
#include "Easing.h"
#include "Geometory.h"
#include "Option.h"
#include "SceneTitle.h"

#define LOGO_WIND (1000)
#define LOGO_HID (1000)
#define ACS1_WID (700)
#define ACS1_HID (700)
#define SET_POTION_X (-900)
#define SET_POTION_Y (0)
#define E_WID (540)
#define E_HID (300)
#define SELECT_POSX (-320.0f)
#define SELECT_MOVE (185.0f)

#define ARROW_RIGHTX (560.0f)
#define ARROW_LEFTX (-560.0f)
#define ARROW_RIGHTY (300.0f)
#define ARROW_LEFTY (300.0f)

#define CENTER_POS_X SCREEN_WIDTH / 2.0f
#define CENTER_POS_Y SCREEN_HEIGHT / 2.0f

#define WORLD_SIZE_X 85.0f
#define WORLD_SIZE_Y 38.0f

#define GRASS_ROTATE_Z2 0.0f

#define GRASS_ROTATE_X 21.0f
#define GRASS_ROTATE_Y 0.0f
#define GRASS_ROTATE_Z 360.0f
#define DESERT_ROTATE_X -11.0f
#define DESERT_ROTATE_Y -21.0f
#define DESERT_ROTATE_Z 122.0f
#define SNOW_ROTATE_X -12.0f
#define SNOW_ROTATE_Y 22.0f
#define SNOW_ROTATE_Z 238.0f
#define LINIE_ROTATE_FLONT 270.0f

StageType g_Select_type;
StageType g_OldSelect_type;
Model::AnimeNo g_AnimNo[5];

constexpr float STAGE_BETWEEN = 700.0f;
constexpr float MOVE_TIME = 30.0f;
constexpr float FIRST_POS = 200.0f;
constexpr float UI_POS_Y = 100.0f;

enum class SelectPhase
{
	Story = 0,
	Select,

	Max
};

CStageSelect::CStageSelect()
	: f_Rotation(0)
	, f_Rad(0)
	, f_SelectX(350)
	, f_SelectY(350)
	, MainStage(true)
	, m_bEnd(false)
	, m_pModel{}
	, m_ModelParam{}
	, m_pBackGround(nullptr)
	, m_bMoving(false)
	, posX{ 0.0f,400.0f,800.0f }
	, subposX{ 140.0f,540.0f,940.0f }
	, m_rotate{}
	, m_pDecition(nullptr)
	, m_pEffect{}, m_pStarEfc{}
	, m_tBGRotateZ(0.0f)
	, m_ModelWorldParam{}
	, LinieRotationY{ LINIE_ROTATE_FLONT }
	, m_bClear{}
	, m_bClearCommand{ false }
	, m_bCantMove_Left(false)
	, m_bCantMove_Right(false)
	, m_fTime(0.0f)
	, m_Direction(NULL)
	, m_bTransition(false)
	, m_tTransitionParam{}
	, m_nPage(0)
	, m_pStory{}
	, m_StoryParam{}
{
	InitValue();
	LoadFile();
	LoadSound();
	SetVP();
	InitConposite();
	ClearCheck();
	SetCameraKind(NOMAL_CAMERA);
}

CStageSelect::~CStageSelect()
{
	// 音の終了
	if (m_pSourseStageSelectBGM)
	{
		m_pSourseStageSelectBGM->Stop();
		m_pSourseStageSelectBGM = nullptr;
		SAFE_DELETE(m_BGMSound);
	}
	for (int i = 0; i < 5; i++)
	{
		m_pSourseStageSelectSE[i] = nullptr;
		SAFE_DELETE(m_pSESound[i]);
	}

	// モデルの終了
	// キャラ
	for (int i = 0; i < StageKind::StageKindMax; i++)
	{
		SAFE_DELETE(m_pModel[i]);
	}
	SAFE_DELETE(m_pStageLinie);
	SAFE_DELETE(m_pStageQracker);
	SAFE_DELETE(m_pStageNugar);
	SAFE_DELETE(m_pStageBoldow);
	SAFE_DELETE(m_pStageKannele);
	// ワールド
	SAFE_DELETE(m_pWorldModel);

	// スプライトの終了
	// ランク
	SAFE_DELETE(m_pRank_C);
	SAFE_DELETE(m_pRank_B);
	SAFE_DELETE(m_pRank_A);
	SAFE_DELETE(m_pRank_S);
	// ワールド選択
	for (int i = 0; i < 3; i++)
	{
		// ワールドの番号
		SAFE_DELETE(m_pWorld[i]);
	}
	SAFE_DELETE(m_pGrassLand);	// ワールド草原の画像
	SAFE_DELETE(m_pDesert);		// ワールド砂漠の画像
	SAFE_DELETE(m_pSnowField);	// ワールド雪山の画像
	// ステージ選択
	for (int i = 0; i < 3; i++)
	{
		// ステージの番号
		SAFE_DELETE(m_pGrassLandStage[i]);
		SAFE_DELETE(m_pDesertStage[i]);
		SAFE_DELETE(m_pSnowFieldStage[i]);
	}
	// UI
	SAFE_DELETE(m_pRight_Select);
	SAFE_DELETE(m_pRight_Select_Lock);
	SAFE_DELETE(m_pLeft_Select);
	SAFE_DELETE(m_pLeft_Select_Lock);
	SAFE_DELETE(m_pStageSelected);
	SAFE_DELETE(m_pStageSelect_Underbar);
	SAFE_DELETE(m_pWorldSelect_Backboard);
	// ストーリー
	for (int i = 0; i < (int)StoryTex::Max; i++)
	{
		SAFE_DELETE(m_pStory[i]);
	}

	for (int i = 0; i < STORY_MAX_TXT; i++)
	{
		SAFE_DELETE(m_pStoryTxt[i]);
	}

	// エフェクトの終了
	for (int i = 0; i < (int)Effect::Max; i++)
	{
		SAFE_DELETE(m_pEffect[i]);
	}
	for (int i = 0; i < SELECT_MAX_STAR; i++)
	{
		m_pStarEfc[i] = nullptr;
	}
	SAFE_DELETE(m_pDecition);	// 選択時のエフェクト

	// 多機能の終了
	SAFE_DELETE(m_pBackGround);
}

void CStageSelect::Update()
{

	if (m_pSourseStageSelectBGM)SetVolumeBGM(m_pSourseStageSelectBGM);

	switch (m_nPhase)
	{
	case (int)SelectPhase::Story:
		UpdateStory();
		break;
	case (int)SelectPhase::Select:
		UpdateSelect();
		break;
	default:
		break;
	}
	
	//m_bMoving = false;
}

void CStageSelect::Draw()
{
	switch (m_nPhase)
	{
	case (int)SelectPhase::Story:
		DrawStory();
		break;
	case (int)SelectPhase::Select:
		DrawSelect();
		break;
	default:
		break;
	}
}

void CStageSelect::LinieDraw()
{
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T;
	if (MainStage) T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(0.0f, 25.0f, 0.0f, 0.0f));
	else T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(140.0f, 15.0f, -250.0f, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.5f, 1.5f, 1.5f);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(LinieRotationY), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pStageLinie->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pStageLinie->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pStageLinie->GetMeshNum(); ++i)
	{
		Model::Mesh tMesh = *m_pStageLinie->GetMesh(i);
		Model::Material material = *m_pStageLinie->GetMaterial(tMesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る

		DirectX::XMFLOAT4X4 bones[200];
		for (int j = 0; j < tMesh.bones.size(); ++j)
		{
			DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
				tMesh.bones[j].invOffset * m_pStageLinie->GetBoneMatrix(tMesh.bones[j].nodeIndex)
			));
		}
		ShaderList::SetBones(bones);


		if (m_pStageLinie) 
		{
			m_pStageLinie->Draw(i);
		}
	}
}

void CStageSelect::QrackerDraw()
{
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(subposX[2] + 70.0f, 10.0f, -150.0f, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pStageQracker->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pStageQracker->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pStageQracker->GetMeshNum(); ++i)
	{
		Model::Mesh tMesh = *m_pStageQracker->GetMesh(i);
		Model::Material material = *m_pStageQracker->GetMaterial(tMesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る

		DirectX::XMFLOAT4X4 bones[200];
		for (int j = 0; j < tMesh.bones.size(); ++j)
		{
			DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
				tMesh.bones[j].invOffset * m_pStageQracker->GetBoneMatrix(tMesh.bones[j].nodeIndex)
			));
		}
		ShaderList::SetBones(bones);

		if (m_pStageQracker)
		{
			m_pStageQracker->Draw(i);
		}
	}
}

void CStageSelect::NugarDraw()
{
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(subposX[2] + 70.0f, 10.0f, -150.0f, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.5f, 1.5f, 1.5f);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pStageNugar->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pStageNugar->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pStageNugar->GetMeshNum(); ++i)
	{
		Model::Mesh tMesh = *m_pStageNugar->GetMesh(i);
		Model::Material material = *m_pStageNugar->GetMaterial(tMesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る

		DirectX::XMFLOAT4X4 bones[200];
		for (int j = 0; j < tMesh.bones.size(); ++j)
		{
			DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
				tMesh.bones[j].invOffset * m_pStageNugar->GetBoneMatrix(tMesh.bones[j].nodeIndex)
			));
		}
		ShaderList::SetBones(bones);

		if (m_pStageNugar)
		{
			m_pStageNugar->Draw(i);
		}
	}
}

void CStageSelect::BoldowDraw()
{
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(subposX[2] + 60.0f, 10.0f, -150.0f, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.5f, 1.5f, 1.5f);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pStageBoldow->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pStageBoldow->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pStageBoldow->GetMeshNum(); ++i)
	{
		Model::Mesh tMesh = *m_pStageBoldow->GetMesh(i);
		Model::Material material = *m_pStageBoldow->GetMaterial(tMesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る

		DirectX::XMFLOAT4X4 bones[200];
		for (int j = 0; j < tMesh.bones.size(); ++j)
		{
			DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
				tMesh.bones[j].invOffset * m_pStageBoldow->GetBoneMatrix(tMesh.bones[j].nodeIndex)
			));
		}
		ShaderList::SetBones(bones);

		if (m_pStageBoldow)
		{
			m_pStageBoldow->Draw(i);
		}
	}
}

void CStageSelect::KanneleDraw()
{
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(subposX[2] + 50.0f, 10.0f, -150.0f, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.5f, 1.5f, 1.5f);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pStageKannele->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pStageKannele->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pStageKannele->GetMeshNum(); ++i)
	{
		Model::Mesh tMesh = *m_pStageKannele->GetMesh(i);
		Model::Material material = *m_pStageKannele->GetMaterial(tMesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る

		DirectX::XMFLOAT4X4 bones[200];
		for (int j = 0; j < tMesh.bones.size(); ++j)
		{
			DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
				tMesh.bones[j].invOffset * m_pStageKannele->GetBoneMatrix(tMesh.bones[j].nodeIndex)
			));
		}
		ShaderList::SetBones(bones);

		if (m_pStageKannele)
		{
			m_pStageKannele->Draw(i);
		}
	}
}

void CStageSelect::SelectAnimation()
{
	//if(MainStage)m_pBackGround->MultiScrollRotate({ 0.0f,0.0f,DirectX::XMConvertToRadians(m_tBGRotateZ) });
}

void CStageSelect::ClearCheck()
{
	for (int i = 0; i < MAX_STAGE; i++)
	{
		//m_bClear[i] = true;
		m_bClear[i] = CSceneResult::GetStageClear(i);
	}
}

bool CStageSelect::ClearCommand(void)
{
		if (IsKeyTrigger('C')) {
			m_bClearCommand[0] = true;
			m_fClearCommandResetTimer = 0.0f;
		}
		if (IsKeyTrigger('L') && m_bClearCommand[0]) {
			m_bClearCommand[1] = true;
			m_fClearCommandResetTimer = 0.0f;
		}
		if (IsKeyTrigger('E') && m_bClearCommand[1]) {
			m_bClearCommand[2] = true;
			m_fClearCommandResetTimer = 0.0f;
		}
		if (IsKeyTrigger('A') && m_bClearCommand[2]) {
			m_bClearCommand[3] = true;
			m_fClearCommandResetTimer = 0.0f;
		}
		if (IsKeyTrigger('R') && m_bClearCommand[3]) {
			m_bClearCommand[4] = true;
			m_fClearCommandResetTimer = 0.0f;
		}

		m_fClearCommandResetTimer += 1.0f / 60.0f;

		if (m_fClearCommandResetTimer > 4.0f)
		{
			for (int i = 0; i < 5; i++)m_bClearCommand[i] = false;
			m_fClearCommandResetTimer = 0.0f;
		}

		return m_bClearCommand[4];
	
}

void CStageSelect::InitValue()
{
	g_Select_type.StageMainNumber = GrassField;
	g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
	g_OldSelect_type = g_Select_type;

	for (int i = 0; i < StageKindMax; i++)
	{
		m_ModelParam[i].pos = { FIRST_POS,0.0f,-100.0f };
		m_ModelParam[i].rotate = { DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(180.0f),0.0f };
		m_ModelParam[i].size = { -2.0f,2.0f,2.0f };

	}

	m_ModelWorldParam.pos = { 0.0f,-66.0f,100.0f };
	m_ModelWorldParam.size = { 1.8f,1.8f,1.8f };
	m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(GRASS_ROTATE_Z2) };


	m_tDecitionPos[0] = { -375.0f, -55.0f };
	m_tDecitionPos[1] = { 375.0f, -385.0f };

	for (int i = 0; i < (int)StoryTex::Max; i++)
	{
		m_StoryParam[i].pos = { CENTER_POS_X,CENTER_POS_Y,0.0f };
		m_StoryParam[i].size = { 1920.0f,-1080.0f,1.0f };
		m_StoryParam[i].rotate = { 0.0f,0.0f,0.0f };
		m_StoryParam[i].color = { 1.0f,1.0f,1.0f,1.0f };
		m_StoryParam[i].uvPos = { 0.0f,0.0f };
		m_StoryParam[i].uvSize = { 1.0f,1.0f };
	}

	m_StoryParam[(int)StoryTex::SecondLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };
	m_StoryParam[(int)StoryTex::FirstLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };
	m_StoryParam[(int)StoryTex::SecondLinie].pos.y = CENTER_POS_Y + 180.0f;
	m_StoryParam[(int)StoryTex::FirstLinie].pos.y = CENTER_POS_Y + 180.0f;
	m_StoryParam[(int)StoryTex::Grail].size = { SCREEN_WIDTH * 0.8f,-SCREEN_HEIGHT * 0.8f,0.0f };

	if (CSceneTitle::IsFirstPlay())m_nPhase = (int)SelectPhase::Story;
	else m_nPhase = (int)SelectPhase::Select;

	for (int i = 0; i < STORY_MAX_TXT; i++)
	{
		m_StoryTxtParam[i].pos = { CENTER_POS_X,CENTER_POS_Y,0.0f };
		m_StoryTxtParam[i].size = { 1920.0f,-1080.0f,1.0f };
		m_StoryTxtParam[i].rotate = { 0.0f,0.0f,0.0f };
		m_StoryTxtParam[i].color = { 1.0f,1.0f,1.0f,0.0f };
		m_StoryTxtParam[i].uvPos = { 0.0f,0.0f };
		m_StoryTxtParam[i].uvSize = { 1.0f,1.0f };
	}
}

void CStageSelect::LoadFile()
{
	m_pGrassLand = new SpriteEx("Assets/Texture/StageSelectBackGround/Worldselect_01.png");
	m_pDesert = new SpriteEx("Assets/Texture/StageSelectBackGround/Worldselect_02.png");
	m_pSnowField = new SpriteEx("Assets/Texture/StageSelectBackGround/Worldselect_03.png");

	m_pGrassLandStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_01.png");
	m_pGrassLandStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_02.png");
	m_pGrassLandStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_03.png");
	m_pDesertStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_04.png");
	m_pDesertStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_05.png");
	m_pDesertStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_06.png");
	m_pSnowFieldStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_07.png");
	m_pSnowFieldStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_08.png");
	m_pSnowFieldStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_09.png");

	m_pWorld[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/WorldNumber01.png");
	m_pWorld[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/WorldNumber02.png");
	m_pWorld[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/WorldNumber03.png");

	m_pWorldSelect_Backboard = new SpriteEx("Assets/Texture/StageSelectBackGround/Worldselect_Backboard.png");

	m_pDecition = new SpriteEx("Assets/Effect/Sprite/Decision.png");
	m_pStageSelected = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Selected.png");

	m_pRight_Select = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Right01.png");
	m_pRight_Select_Lock = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Right02.png");
	m_pLeft_Select = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Left01.png");
	m_pLeft_Select_Lock = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Left02.png");

	m_pRank_C = new SpriteEx("Assets/Texture/Result/Rank/Rank_C.png");
	m_pRank_B = new SpriteEx("Assets/Texture/Result/Rank/Rank_B.png");
	m_pRank_A = new SpriteEx("Assets/Texture/Result/Rank/Rank_A.png");
	m_pRank_S = new SpriteEx("Assets/Texture/Result/Rank/Rank_S.png");


	m_pStageSelect_Underbar = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Underbar.png");
	m_pModel[GrassField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage01_GrassField.fbx"), false);
	m_pModel[DesertField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage02_Desert.fbx"), false);
	m_pModel[SnowField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage03_SnowField.fbx"), false);
	m_pWorldModel = new Model();
	m_pWorldModel->Load(MODEL_PASS("StageSelect/WorldSelect_World.fbx"), 1.0f, Model::ZFlip);
	m_pEffect[(int)Effect::Star] = new CEffectManager_sp("Assets/Effect/Sprite/BackGround_ShootginStars.png", 8, 8, 3.0f);

	m_pStageLinie = new Model();
	m_pStageLinie->Load(MODEL_PASS("Leader/Linie/Anim_Linie_SelectStage_Run.fbx"), 1.0f, Model::ZFlip);
	g_AnimNo[0] = m_pStageLinie->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Linie_SelectStage_Run.fbx"));
	m_pStageLinie->PlayAnime(g_AnimNo[0], false);
	m_pStageLinie->Step(0.01f);
	m_pStageLinie->SetAnimeTime(g_AnimNo[0], 0.0f);

	m_pStageQracker = new Model();
	m_pStageQracker->Load(MODEL_PASS("Leader/Qracker/Anim_Boss01_Qracker_Idle.fbx"), 1.0f, Model::ZFlip);


	m_pStageNugar = new Model();
	m_pStageNugar->Load(MODEL_PASS("Leader/Nugar/Anim_Boss02_Nugar_Idle.fbx"), 1.0f, Model::ZFlip);

	m_pStageBoldow = new Model();
	m_pStageBoldow->Load(MODEL_PASS("Leader/Boldow/Anim_Boss03_Boldow_Idle.fbx"), 1.0f, Model::ZFlip);

	m_pStageKannele = new Model();
	m_pStageKannele->Load(MODEL_PASS("Leader/Kannele/Anim_Boss03_Kannele_Idle.fbx"), 1.0f, Model::ZFlip);

	m_pStory[(int)StoryTex::Cinema] = new SpriteEx(TEX_PASS("Story/Beginning/Cinema.png"));
	m_pStory[(int)StoryTex::Back] = new SpriteEx(TEX_PASS("Story/Beginning/01_1A.png"));
	m_pStory[(int)StoryTex::FirstLinie] = new SpriteEx(TEX_PASS("Story/Beginning/01_1C1.png"));
	m_pStory[(int)StoryTex::SecondLinie] = new SpriteEx(TEX_PASS("Story/Beginning/01_1C2.png"));
	m_pStory[(int)StoryTex::LinieHands] = new SpriteEx(TEX_PASS("Story/Beginning/01_1D.png"));
	m_pStory[(int)StoryTex::BackStar] = new SpriteEx(TEX_PASS("Story/Beginning/01_1E.png"));
	m_pStory[(int)StoryTex::Grail] = new SpriteEx(TEX_PASS("Story/Beginning/01_1F.png"));
	m_pStory[(int)StoryTex::Bosses] = new SpriteEx(TEX_PASS("Story/Beginning/01_1B.png"));
	m_pStory[(int)StoryTex::ReruxLinie] = new SpriteEx(TEX_PASS("Story/Beginning/01_2.png"));

	m_pStoryTxt[0] = new SpriteEx(TEX_PASS("Story/Beginning/Text01.png"));
	m_pStoryTxt[1] = new SpriteEx(TEX_PASS("Story/Beginning/Text02.png"));
	m_pStoryTxt[2] = new SpriteEx(TEX_PASS("Story/Beginning/Text03.png"));
	m_pStoryTxt[3] = new SpriteEx(TEX_PASS("Story/Beginning/Text04.png"));
	m_pStoryTxt[4] = new SpriteEx(TEX_PASS("Story/Beginning/Text05.png"));
}

void CStageSelect::SetVP()
{
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pGrassLandStage[nLoop]->SetProjection(GetProj());
		m_pGrassLandStage[nLoop]->SetView(GetView());
		m_pDesertStage[nLoop]->SetProjection(GetProj());
		m_pDesertStage[nLoop]->SetView(GetView());
		m_pSnowFieldStage[nLoop]->SetProjection(GetProj());
		m_pSnowFieldStage[nLoop]->SetView(GetView());
		m_pWorld[nLoop]->SetProjection(GetProj());
		m_pWorld[nLoop]->SetView(GetView());
	}
	m_pGrassLand->SetProjection(GetProj());
	m_pGrassLand->SetView(GetView());
	m_pDesert->SetProjection(GetProj());
	m_pDesert->SetView(GetView());
	m_pSnowField->SetProjection(GetProj());
	m_pSnowField->SetView(GetView());
	m_pRight_Select->SetProjection(GetProj());
	m_pRight_Select->SetView(GetView());
	m_pLeft_Select->SetProjection(GetProj());
	m_pLeft_Select->SetView(GetView());
	m_pRight_Select_Lock->SetProjection(GetProj());
	m_pRight_Select_Lock->SetView(GetView());
	m_pLeft_Select_Lock->SetProjection(GetProj());
	m_pLeft_Select_Lock->SetView(GetView());
	m_pStageSelected->SetProjection(GetProj());
	m_pStageSelected->SetView(GetView());
	m_pStageSelect_Underbar->SetProjection(GetProj());
	m_pStageSelect_Underbar->SetView(GetView());
	m_pRank_C->SetProjection(GetProj());
	m_pRank_C->SetView(GetView());
	m_pRank_B->SetProjection(GetProj());
	m_pRank_B->SetView(GetView());
	m_pRank_A->SetProjection(GetProj());
	m_pRank_A->SetView(GetView());
	m_pRank_S->SetProjection(GetProj());
	m_pRank_S->SetView(GetView());

	for (int i = 0; i < (int)StoryTex::Max; i++)
	{
		m_pStory[i]->SetView(Get2DView());
		m_pStory[i]->SetProjection(Get2DProj());
	}
}

void CStageSelect::InitConposite()
{
	for (int i = 0; i < SELECT_MAX_STAR; i++)
	{
		m_pStarEfc[i] = new CEffectManager_sp(m_pEffect[(int)Effect::Star]);
	}
	m_pBackGround = new CBackGround();
}

void CStageSelect::LoadSound()
{
	m_BGMSound = new CSoundList(BGM_SELECT);
	m_BGMSound->SetMasterVolume();
	m_pSourseStageSelectBGM = m_BGMSound->GetSound(true);
	SetVolumeBGM(m_pSourseStageSelectBGM);
	m_pSourseStageSelectBGM->Start();
	m_pSESound[DECISION_SE] = new CSoundList(SE_DECISION);
	m_pSESound[GRASS_SE] = new CSoundList(SE_WALKGRASS);
	m_pSESound[DESERT_SE] = new CSoundList(SE_WALKDESERT);
	m_pSESound[SNOW_SE] = new CSoundList(SE_WALKSNOW);
	m_pSESound[LOCK_SE] = new CSoundList(SE_LOCK);
	for (int i = 0; i < 5; i++)
	{
		m_pSESound[i]->SetMasterVolume();
		m_pSourseStageSelectSE[i] = m_pSESound[i]->GetSound(false);
		SetVolumeSE(m_pSourseStageSelectSE[i]);
	}
}

void CStageSelect::UpdateStory()
{
	static float time = 0.0f;
	static float time2 = 0.0f;
	static float EasTime = 0.0f;
	static float DegCnt = 0;
	static float GrailPosY = 0.0f;
	float EasVal = 0.0f;
	float cosVal = 0.0f;

	switch (m_nPage)
	{
	case 0:
		m_StoryParam[(int)StoryTex::FirstLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };
		m_StoryParam[(int)StoryTex::FirstLinie].color.w = 1.0f;
		m_StoryParam[(int)StoryTex::SecondLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };

		m_StoryTxtParam[0].color.w += 0.05f;
		if (m_StoryTxtParam[0].color.w >= 1.0f)m_StoryTxtParam[0].color.w = 1.0f;
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)) || IsKeyTrigger(VK_SPACE))
		{
			m_StoryParam[(int)StoryTex::SecondLinie].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::LinieHands].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::Grail].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::BackStar].color.w = 0.0f;
			m_nPage++;
			time = 0.0f;
			time2 = 0.0f;
			m_StoryTxtParam[0].color.w = 1.0f;
			m_StoryTxtParam[1].color.w = 0.0f;
			m_StoryTxtParam[2].color.w = 0.0f;
		}
		else if(IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B)))
		{
			SetNext(SCENE_TITLE);
		}
		break;
	case 1:
		time += 1.0f / 60.0f;
		cosVal = DirectX::XMConvertToRadians((float)DegCnt * 5.0f);
		cosVal = cosf(cosVal) - 0.5f;
		m_StoryParam[(int)StoryTex::Grail].pos.y = CENTER_POS_Y + cosVal * 50.0f;
		m_StoryParam[(int)StoryTex::BackStar].pos.y = CENTER_POS_Y - cosVal * 50.0f;

		m_StoryTxtParam[0].color.w -= 0.05f;
		if (m_StoryTxtParam[0].color.w <= 0.0f)m_StoryTxtParam[0].color.w = 0.0f;


		if (time <= 1.5f)
		{
			m_StoryTxtParam[1].color.w += 0.05f;
			if (m_StoryTxtParam[1].color.w >= 1.0f)
			{
				m_StoryTxtParam[1].color.w = 1.0f;
			}
			m_StoryParam[(int)StoryTex::FirstLinie].size.x -= SCREEN_WIDTH / 400.0f;
			m_StoryParam[(int)StoryTex::FirstLinie].size.y += SCREEN_HEIGHT / 400.0f;		
			m_StoryParam[(int)StoryTex::SecondLinie].size.x -= SCREEN_WIDTH / 400.0f;
			m_StoryParam[(int)StoryTex::SecondLinie].size.y += SCREEN_HEIGHT / 400.0f;
			m_StoryParam[(int)StoryTex::FirstLinie].pos.y -= 2.0f;
			m_StoryParam[(int)StoryTex::SecondLinie].pos.y -= 2.0f;
		}
		else
		{
			if (m_StoryTxtParam[2].color.w >= 1.0f)
			{
				m_StoryTxtParam[2].color.w = 1.0f;
			}
			else
			{
				if (m_StoryTxtParam[1].color.w <= 0.0f)m_StoryTxtParam[1].color.w = 0.0f;
				else m_StoryTxtParam[1].color.w -= 0.05f;
				m_StoryTxtParam[2].color.w += 0.05f;
			}
			time2 += 1.0f / 60.0f;

			m_StoryParam[(int)StoryTex::SecondLinie].color.w = (time2) / 1.0f - 0.5f;
			m_StoryParam[(int)StoryTex::FirstLinie].color.w = (1.0f - time2) / 1.0f + 0.5f;
			m_StoryParam[(int)StoryTex::LinieHands].color.w = (time2) / 1.0f - 0.5f;
			m_StoryParam[(int)StoryTex::Grail].color.w = (time2) / 1.0f - 0.5f;
			m_StoryParam[(int)StoryTex::BackStar].color.w = (time2) / 1.0f - 0.5f;
		}


		DegCnt+= 0.5f;
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)) || IsKeyTrigger(VK_SPACE))
		{
			m_nPage++;
			time = 0.0f;
			m_StoryParam[(int)StoryTex::Grail].color.w = 1.0f;
			m_StoryParam[(int)StoryTex::SecondLinie].color.w = 1.0f;
			m_StoryParam[(int)StoryTex::LinieHands].color.w = 1.0f;
			m_StoryParam[(int)StoryTex::Bosses].pos.y = SCREEN_HEIGHT * 2;
			DegCnt = 0;
			cosVal = 0.0f;
			EasTime = 0.0f;
		}
		else if(IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B)) || IsKeyTrigger(VK_SPACE))
		{
			m_StoryTxtParam[0].color.w = 0.0f;
			m_StoryTxtParam[1].color.w = 0.0f;
			m_StoryTxtParam[2].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::SecondLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };
			m_StoryParam[(int)StoryTex::FirstLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };
			m_StoryParam[(int)StoryTex::SecondLinie].pos.y = CENTER_POS_Y + 180.0f;
			m_StoryParam[(int)StoryTex::FirstLinie].pos.y = CENTER_POS_Y + 180.0f;
			m_StoryParam[(int)StoryTex::Grail].size = { SCREEN_WIDTH * 0.8f,-SCREEN_HEIGHT * 0.8f,0.0f };
			m_nPage--;
			time = 0.0f;
		}
		break;
	case 2:
		time += 1.0f / 60.0f;

		m_StoryParam[(int)StoryTex::SecondLinie].color.w = (1.0f - time) / 1.0f + 0.5f;
		m_StoryParam[(int)StoryTex::LinieHands].color.w = (1.0f - time) / 1.0f + 0.5f;


		DegCnt += 0.5f;

		if (m_StoryParam[(int)StoryTex::SecondLinie].color.w <= 0.0f && EasTime <= 1.0f)
		{
			m_StoryTxtParam[1].color.w -= 0.05f;
			if (m_StoryTxtParam[1].color.w <= 0.0f)m_StoryTxtParam[1].color.w = 0.0f;
			m_StoryTxtParam[2].color.w -= 0.05f;
			if (m_StoryTxtParam[2].color.w <= 0.0f)m_StoryTxtParam[2].color.w = 0.0f;

			if (m_StoryTxtParam[3].color.w >= 1.0f)m_StoryTxtParam[3].color.w = 1.0f;
			else m_StoryTxtParam[3].color.w += 0.05f;
			EasTime += 1.0f / 60.0f;
			EasVal = Easing39(EasTime, 0.5f, 0.9f);
			m_StoryParam[(int)StoryTex::Bosses].pos.y = 0.0f + EasVal * CENTER_POS_Y;
			m_StoryParam[(int)StoryTex::Grail].pos.y = 0.0f + EasVal * CENTER_POS_Y - 200.0f;
			GrailPosY = m_StoryParam[(int)StoryTex::Grail].pos.y;
		}
		else if(EasTime >= 1.0f)
		{
			cosVal = DirectX::XMConvertToRadians((float)DegCnt * 5.0f);
			cosVal = cosf(cosVal) - 0.5f;
			cosVal = cosVal * 30.0f;
			m_StoryParam[(int)StoryTex::Grail].pos.y = GrailPosY + cosVal;
		}

	
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)) || IsKeyTrigger(VK_SPACE))
		{
			m_nPage++;
			time = 0.0f;
			m_StoryParam[(int)StoryTex::SecondLinie].color.w = 1.0f;
			m_StoryParam[(int)StoryTex::LinieHands].color.w = 1.0f;
			m_StoryParam[(int)StoryTex::Bosses].pos.y = SCREEN_HEIGHT;
			m_StoryParam[(int)StoryTex::ReruxLinie].color.w = 0.0f;
		}
		else if(IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B)) || IsKeyTrigger(VK_SPACE))
		{
			m_nPage--;
			time = 0.0f;
			EasTime = 0.0f;
			cosVal = 0.0f;
			time2 = 0.0f;
			m_StoryParam[(int)StoryTex::SecondLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };
			m_StoryParam[(int)StoryTex::FirstLinie].size = { SCREEN_WIDTH * 1.2f,-SCREEN_HEIGHT * 1.2f,0.0f };
			m_StoryParam[(int)StoryTex::SecondLinie].pos.y = CENTER_POS_Y + 180.0f;
			m_StoryParam[(int)StoryTex::FirstLinie].pos.y = CENTER_POS_Y + 180.0f;
			m_StoryParam[(int)StoryTex::Grail].pos.y = CENTER_POS_Y;
			m_StoryParam[(int)StoryTex::Grail].size = { SCREEN_WIDTH * 0.8f,-SCREEN_HEIGHT * 0.8f,0.0f };
			m_StoryParam[(int)StoryTex::FirstLinie].color.w = 1.0f;
			m_StoryParam[(int)StoryTex::SecondLinie].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::LinieHands].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::Grail].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::BackStar].color.w = 0.0f;
			m_StoryParam[(int)StoryTex::Bosses].pos.y = SCREEN_HEIGHT;
			m_StoryTxtParam[1].color.w = 0.0f;
			m_StoryTxtParam[2].color.w = 0.0f;
			m_StoryTxtParam[3].color.w = 0.0f;
		}
		break;
	case 3:
		time += 1.0f / 60.0f;
		m_StoryParam[(int)StoryTex::ReruxLinie].color.w += 0.05f;

		if (m_StoryTxtParam[2].color.w <= 0.0f)m_StoryTxtParam[2].color.w = 0.0f;
		else m_StoryTxtParam[2].color.w -= 0.05f;
		if (m_StoryTxtParam[3].color.w <= 0.0f)m_StoryTxtParam[3].color.w = 0.0f;
		else m_StoryTxtParam[3].color.w -= 0.05f;
		if (m_StoryTxtParam[4].color.w >= 1.0f)m_StoryTxtParam[4].color.w = 1.0f;
		else m_StoryTxtParam[4].color.w += 0.05f;
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)) || IsKeyTrigger(VK_SPACE))
		{
			m_nPage++;
			time = 0.0f;
		}
		else if(IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B)) || IsKeyTrigger(VK_SPACE))
		{
			m_nPage--;
			time = 0.0f;
		}
		break;
	case 4:
		time += 1.0f / 60.0f;

		for (int i = 0; i < (int)StoryTex::Max; i++)
		{
			m_StoryParam[i].color.w = (1.0f - time) / 2.0f;
		}
		if (m_StoryTxtParam[4].color.w >= 0.0f)m_StoryTxtParam[4].color.w = (1.0f - time) / 2.0f;
		if (time >= 2.0f)m_nPhase = (int)SelectPhase::Select;
		break;
	default:
		break;
	}
}

void CStageSelect::UpdateSelect()
{
	static bool rotateSwap = false;
	static bool bRight = false;
	//SpriteDebug(&m_ModelParam[WorldField], true);
	//if (IsKeyTrigger('B'))m_ModelParam[WorldField].rotate.z += DirectX::XMConvertToRadians(120.0f);
	m_Direction = GetControllerLStickTriggerForeDirection();



#if 0
	if (IsKeyPress('Q'))
	{
		m_tTransitionParam[0].size.x += 16.0f;
		m_tTransitionParam[0].size.y += 9.0f;
	}
	if (IsKeyPress('E'))
	{
		m_tTransitionParam[0].size.x -= 16.0f;
		m_tTransitionParam[0].size.y -= 9.0f;
	}
	m_tTransitionParam[0].pos = { CENTER_POS_X,CENTER_POS_Y,0.0f };
	static DirectX::XMFLOAT3 debugpos = { 170.0f, 46.0f, -370.0f };
	if (IsKeyPress('D'))debugpos.x++;
	if (IsKeyPress('A'))debugpos.x--;
	if (IsKeyPress(VK_SPACE))debugpos.y++;
	if (IsKeyPress(VK_SHIFT))debugpos.y--;
	if (IsKeyPress('W'))debugpos.z++;
	if (IsKeyPress('S'))debugpos.z--;


	if (MainStage)
	{
		SetCameraPos(DirectX::XMFLOAT3(0.0f, 45.0f, -15.0f));
		SetCameraRotate(DirectX::XMFLOAT3(0.0f, 0.0f, -90.0f));
		SetFovY(DirectX::XMConvertToRadians(60.0f));
	}
	else
	{
		SetCameraPos(debugpos);
		SetCameraRotate(DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f));
		SetFovY(DirectX::XMConvertToRadians(20.0f));
	}

	OutputDebugStringA("X:");
	OutputDebugStringA(std::to_string(debugpos.x).c_str());
	OutputDebugStringA("Y:");
	OutputDebugStringA(std::to_string(debugpos.y).c_str());
	OutputDebugStringA("Z:");
	OutputDebugStringA(std::to_string(debugpos.z).c_str());
	OutputDebugStringA("\n");
	if (ClearCommand())
	{
		for (int i = 0; i < MAX_STAGE; i++)
		{
			m_bClear[i] = true;

		}
	}
#endif

	if (ClearCommand())
	{
		for (int i = 0; i < MAX_STAGE; i++)
		{
			m_bClear[i] = true;
		}
	}
#if 1
	if (MainStage)
	{
		SetCameraPos(DirectX::XMFLOAT3(0.0f, 45.0f, -15.0f));
		SetCameraRotate(DirectX::XMFLOAT3(0.0f, 0.0f, -90.0f));
		SetFovY(DirectX::XMConvertToRadians(60.0f));
	}
	else
	{
		SetCameraPos(DirectX::XMFLOAT3(140.0f, 46.0f, -370.0f));
		SetCameraRotate(DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f));
		SetFovY(DirectX::XMConvertToRadians(30.0f));
	}



	if (!m_bEnd)
	{
		if (!m_bMoving)
		{
			LinieRotationY = LINIE_ROTATE_FLONT;
			if (MainStage)
			{
				switch (g_Select_type.StageMainNumber)
				{
				case(GRASSLAND):
					posX[0] = 0.0f;
					posX[1] = 400.0f;
					posX[2] = 800.0f;
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[GRASSLAND_STAGE3])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = DESERT;
						m_bMoving = true;
						m_rotate.z = GRASS_ROTATE_Z2;
						m_pStageLinie->PlayAnime(g_AnimNo[0], false);
						bRight = true;
					}
					else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[GRASSLAND_STAGE3])
					{
						m_pSourseStageSelectSE[LOCK_SE]->Stop();
						m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
						XAUDIO2_BUFFER buffer;
						buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
						m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
						SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
						m_pSourseStageSelectSE[LOCK_SE]->Start();
						m_bCantMove_Right = true;
					}
					else if ((IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT) && m_bClear[DESERT_STAGE3])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = SNOWFIELD;
						m_bMoving = true;
						m_rotate.z = GRASS_ROTATE_Z;
						posX[2] = -400.0f;
						m_pStageLinie->PlayAnime(g_AnimNo[0], false);
						bRight = false;
					}
					else if ((IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT) && !m_bClear[DESERT_STAGE3])
					{
						m_pSourseStageSelectSE[LOCK_SE]->Stop();
						m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
						XAUDIO2_BUFFER buffer;
						buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
						m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
						SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
						m_pSourseStageSelectSE[LOCK_SE]->Start();
						m_bCantMove_Left = true;
					}
					g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
					break;
				case(DESERT):
					posX[0] = -400.0f;
					posX[1] = 0.0f;
					posX[2] = 400.0f;
					//LinieRotationY = 180.0f;
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[DESERT_STAGE3])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = SNOWFIELD;
						m_bMoving = true;
						m_pStageLinie->PlayAnime(g_AnimNo[0], false);
						bRight = true;
					}
					else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[DESERT_STAGE3])
					{
						m_pSourseStageSelectSE[LOCK_SE]->Stop();
						m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
						XAUDIO2_BUFFER buffer;
						buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
						m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
						SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
						m_pSourseStageSelectSE[LOCK_SE]->Start();
						m_bCantMove_Right = true;
					}
					else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = GRASSLAND;
						m_bMoving = true;
						m_pStageLinie->PlayAnime(g_AnimNo[0], false);
						bRight = false;
					}
					g_Select_type.StageSubNumber = DESERT_STAGE1;
					break;
				case(SNOWFIELD):
					posX[0] = -800.0f;
					posX[1] = -400.0f;
					posX[2] = 0.0f;
					//LinieRotationY = 180.0f;
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = DESERT;
						m_bMoving = true;
						m_pStageLinie->PlayAnime(g_AnimNo[0], false);
						bRight = false;
					}
					else if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT)
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = GRASSLAND;
						m_bMoving = true;
						posX[0] = 400.0f;
						m_pStageLinie->PlayAnime(g_AnimNo[0], false);
						bRight = true;
					};
					g_Select_type.StageSubNumber = SNOWFIELD_STAGE1;
					break;
				}
				if (IsKeyTrigger(VK_RETURN) || (IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A))))
				{
					StartFade();
					MainStage ^= true;

					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,-100.0f };
					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,-100.0f };
					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,-100.0f };

				}
				else if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B)))
				{
					StartFade();
					SetNext(SCENE_TITLE, g_Select_type);
					m_bEnd = true;
				}
			}
			else
			{
				if (g_Select_type.StageMainNumber == GRASSLAND)
				{
					switch (g_Select_type.StageSubNumber)
					{
					case(GRASSLAND_STAGE1):
						if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[GRASSLAND_STAGE1])
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = GRASSLAND_STAGE2;
							m_bMoving = true;
							bRight = true;
						}
						else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[GRASSLAND_STAGE1])
						{
							m_pSourseStageSelectSE[LOCK_SE]->Stop();
							m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
							XAUDIO2_BUFFER buffer;
							buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
							m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
							SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
							m_pSourseStageSelectSE[LOCK_SE]->Start();
							m_bCantMove_Right = true;
						}
						break;

					case(GRASSLAND_STAGE2):
						if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[GRASSLAND_STAGE2])
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = GRASSLAND_STAGE3;
							m_bMoving = true;
							bRight = true;
						}
						else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[GRASSLAND_STAGE2])
						{
							m_pSourseStageSelectSE[LOCK_SE]->Stop();
							m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
							XAUDIO2_BUFFER buffer;
							buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
							m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
							SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
							m_pSourseStageSelectSE[LOCK_SE]->Start();
							m_bCantMove_Right = true;
						}
						else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
							m_bMoving = true;
							bRight = false;
						}
						break;

					case(GRASSLAND_STAGE3):
						if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = GRASSLAND_STAGE2; m_bMoving = true;
							bRight = false;
						}
						break;
					}
				}
				if (g_Select_type.StageMainNumber == DESERT)
				{
					switch (g_Select_type.StageSubNumber)
					{
					case(DESERT_STAGE1):
						if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[DESERT_STAGE1])
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = DESERT_STAGE2;
							m_bMoving = true;
							bRight = true;
						}
						else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[DESERT_STAGE1])
						{
							m_pSourseStageSelectSE[LOCK_SE]->Stop();
							m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
							XAUDIO2_BUFFER buffer;
							buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
							m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
							SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
							m_pSourseStageSelectSE[LOCK_SE]->Start();
							m_bCantMove_Right = true;
						}
						break;

					case(DESERT_STAGE2):
						if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[DESERT_STAGE2])
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = DESERT_STAGE3;
							m_bMoving = true;
							bRight = true;
						}
						else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[DESERT_STAGE2])
						{
							m_pSourseStageSelectSE[LOCK_SE]->Stop();
							m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
							XAUDIO2_BUFFER buffer;
							buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
							m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
							SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
							m_pSourseStageSelectSE[LOCK_SE]->Start();
							m_bCantMove_Right = true;
						}
						else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = DESERT_STAGE1;
							m_bMoving = true;
							bRight = false;
						}
						break;

					case(DESERT_STAGE3):
						if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = DESERT_STAGE2;
							m_bMoving = true;
							bRight = false;
						}
						break;
					}
				}
				if (g_Select_type.StageMainNumber == SNOWFIELD)
				{
					switch (g_Select_type.StageSubNumber)
					{
					case(SNOWFIELD_STAGE1):
						if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[SNOWFIELD_STAGE1])
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = SNOWFIELD_STAGE2;
							m_bMoving = true;
							bRight = true;
						}
						else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[SNOWFIELD_STAGE1])
						{
							m_pSourseStageSelectSE[LOCK_SE]->Stop();
							m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
							XAUDIO2_BUFFER buffer;
							buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
							m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
							SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
							m_pSourseStageSelectSE[LOCK_SE]->Start();
							m_bCantMove_Right = true;
						}
						break;

					case(SNOWFIELD_STAGE2):
						if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && m_bClear[SNOWFIELD_STAGE2])
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = SNOWFIELD_STAGE3;
							m_bMoving = true;
							bRight = true;
						}
						else if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT)) || m_Direction == XINPUT_GAMEPAD_DPAD_RIGHT) && !m_bClear[SNOWFIELD_STAGE2])
						{
							m_pSourseStageSelectSE[LOCK_SE]->Stop();
							m_pSourseStageSelectSE[LOCK_SE]->FlushSourceBuffers();
							XAUDIO2_BUFFER buffer;
							buffer = m_pSESound[LOCK_SE]->GetBuffer(false);
							m_pSourseStageSelectSE[LOCK_SE]->SubmitSourceBuffer(&buffer);
							SetVolumeSE(m_pSourseStageSelectSE[LOCK_SE]);
							m_pSourseStageSelectSE[LOCK_SE]->Start();
							m_bCantMove_Right = true;
						}
						else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = SNOWFIELD_STAGE1;
							m_bMoving = true;
							bRight = false;
						}
						break;

					case(SNOWFIELD_STAGE3):
						if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT) || WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT)) || m_Direction == XINPUT_GAMEPAD_DPAD_LEFT)
						{
							g_OldSelect_type = g_Select_type;
							g_Select_type.StageSubNumber = SNOWFIELD_STAGE2;
							bRight = false;
							m_bMoving = true;
						}
						break;
					}
				}
				if (IsKeyTrigger(VK_RETURN) || (IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A))))
				{
					CSceneResult::InStageLevel(g_Select_type);
					SetNext(SCENE_GAME, g_Select_type);
					m_bEnd = true;
				}
				else if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B)))
				{
					StartFade();
					g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
					subposX[0] = 140.0f;
					subposX[1] = 540.0f;
					subposX[2] = 940.0f;
					MainStage ^= true;

					m_ModelWorldParam.pos = { 0.0f,-66.0f,100.0f };
					m_ModelWorldParam.size = { 1.8f,1.8f,1.8f };
					switch (g_Select_type.StageMainNumber)
					{
					case(GRASSLAND):
						m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(GRASS_ROTATE_Z2) };
						break;
					case(DESERT):
						m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(DESERT_ROTATE_Z) };
						break;
					case(SNOWFIELD):
						m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(SNOW_ROTATE_Z) };
						break;
					}

				}
			}
			m_rotate.x = m_ModelWorldParam.rotate.x;
			m_rotate.y = m_ModelWorldParam.rotate.y;
			if (g_Select_type.StageMainNumber != GRASSLAND)m_rotate.z = m_ModelWorldParam.rotate.z;
		}
		else if (m_bMoving)
		{
			float rotate = 90.0f;
			static int moveCnt = 0;
			moveCnt++;
			m_pStageLinie->Step(2.0f / MOVE_TIME);
			if (bRight)m_tBGRotateZ += 120.0f / MOVE_TIME;
			else m_tBGRotateZ -= 120.0f / MOVE_TIME;
			if (g_OldSelect_type.StageMainNumber != g_Select_type.StageMainNumber || g_OldSelect_type.StageSubNumber != g_Select_type.StageSubNumber)
			{
				XAUDIO2_BUFFER buffer;
				m_pSourseStageSelectSE[GRASS_SE]->Stop();
				m_pSourseStageSelectSE[DESERT_SE]->Stop();
				m_pSourseStageSelectSE[SNOW_SE]->Stop();
				switch (g_OldSelect_type.StageMainNumber)
				{
				case GRASSLAND:
					m_pSourseStageSelectSE[GRASS_SE]->FlushSourceBuffers();
					buffer = m_pSESound[GRASS_SE]->GetBuffer(false);
					m_pSourseStageSelectSE[GRASS_SE]->SubmitSourceBuffer(&buffer);
					SetVolumeSE(m_pSourseStageSelectSE[GRASS_SE]);
					m_pSourseStageSelectSE[GRASS_SE]->Start();
					break;
				case DESERT:
					m_pSourseStageSelectSE[DESERT_SE]->FlushSourceBuffers();
					buffer = m_pSESound[DESERT_SE]->GetBuffer(false);
					m_pSourseStageSelectSE[DESERT_SE]->SubmitSourceBuffer(&buffer);
					SetVolumeSE(m_pSourseStageSelectSE[DESERT_SE]);
					m_pSourseStageSelectSE[DESERT_SE]->Start();
					break;
				case SNOWFIELD:
					m_pSourseStageSelectSE[SNOW_SE]->FlushSourceBuffers();
					buffer = m_pSESound[SNOW_SE]->GetBuffer(false);
					m_pSourseStageSelectSE[SNOW_SE]->SubmitSourceBuffer(&buffer);
					SetVolumeSE(m_pSourseStageSelectSE[SNOW_SE]);
					m_pSourseStageSelectSE[SNOW_SE]->Start();
					break;
				default:
					break;
				}
				g_OldSelect_type = g_Select_type;
			}
			switch (g_Select_type.StageMainNumber)
			{
			case(GRASSLAND):
				//m_ModelParam[WorldField].rotate.x += (DirectX::XMConvertToRadians(GRASS_ROTATE_X) - m_rotate.x) / MOVE_TIME;
				//m_ModelParam[WorldField].rotate.y += (DirectX::XMConvertToRadians(GRASS_ROTATE_Y) - m_rotate.y) / MOVE_TIME;
				//if (m_bMoving)
				//{
				//	if (moveCnt >= MOVE_TIME / 2.0f)LinieRotationY += 90.0f / MOVE_TIME;
				//	else LinieRotationY -= 90.0f / MOVE_TIME;
				//}
				if (bRight)m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(GRASS_ROTATE_Z) - DirectX::XMConvertToRadians(SNOW_ROTATE_Z)) / MOVE_TIME;
				else m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(GRASS_ROTATE_Z2) - DirectX::XMConvertToRadians(DESERT_ROTATE_Z)) / MOVE_TIME;
				break;
			case(DESERT):
				//m_ModelParam[WorldField].rotate.x += (DirectX::XMConvertToRadians(DESERT_ROTATE_X) - m_rotate.x) / MOVE_TIME;
				//m_ModelParam[WorldField].rotate.y += (DirectX::XMConvertToRadians(DESERT_ROTATE_Y) - m_rotate.y) / MOVE_TIME;
				/*if (m_bMoving == true)
				{
					if (moveCnt >= MOVE_TIME / 2.0f)LinieRotationY += 90.0f / MOVE_TIME;
					else LinieRotationY -= 90.0f / MOVE_TIME;
				}*/
				if (bRight)m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(DESERT_ROTATE_Z) - DirectX::XMConvertToRadians(GRASS_ROTATE_Z2)) / MOVE_TIME;
				else m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(DESERT_ROTATE_Z) - DirectX::XMConvertToRadians(SNOW_ROTATE_Z)) / MOVE_TIME;
				break;
			case(SNOWFIELD):
				//m_ModelParam[WorldField].rotate.x += (DirectX::XMConvertToRadians(SNOW_ROTATE_X) - m_rotate.x) / MOVE_TIME;
				//m_ModelParam[WorldField].rotate.y += (DirectX::XMConvertToRadians(SNOW_ROTATE_Y) - m_rotate.y) / MOVE_TIME;
				//if (m_bMoving) 
				//{
				//	if (moveCnt >= MOVE_TIME / 2.0f)LinieRotationY += 90.0f / MOVE_TIME;
				//	else LinieRotationY -= 90.0f / MOVE_TIME;
				//}
				if (bRight) {
					m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(SNOW_ROTATE_Z) - DirectX::XMConvertToRadians(DESERT_ROTATE_Z)) / MOVE_TIME;
				}
				else m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(SNOW_ROTATE_Z) - DirectX::XMConvertToRadians(GRASS_ROTATE_Z)) / MOVE_TIME;
				break;
			}

			for (int i = 0; i < 3; i++)
			{
				if (!MainStage)
				{
					if (bRight)
					{
						m_ModelParam[i].pos.x -= STAGE_BETWEEN / 3.0f / MOVE_TIME;
					}
					else
					{
						m_ModelParam[i].pos.x += STAGE_BETWEEN / 3.0f / MOVE_TIME;
					}
				}
			}
			for (int i = 0; i < 3; i++)
			{
				if (MainStage)
				{
					if (bRight)
					{
						posX[i] -= 400.0f / MOVE_TIME;
					}
					else
					{
						posX[i] += 400.0f / MOVE_TIME;
					}
				}
				else
				{
					if (bRight)
					{
						subposX[i] -= 400.0f / MOVE_TIME;
					}
					else
					{
						subposX[i] += 400.0f / MOVE_TIME;
					}
				}
			}
			if (moveCnt >= MOVE_TIME)
			{
				m_pStageLinie->SetAnimeTime(g_AnimNo[0], 0.0f);
				m_bMoving = false;
				moveCnt = 0;
			}
		}
	}
#endif
	SelectAnimation();
}

void CStageSelect::DrawStory()
{
	if (m_nPage == 4)
	{
		DrawSelect();
	}
	SetRender2D();
	switch (m_nPage)
	{
	case 0:
		m_pStory[(int)StoryTex::Back]->SetPositon(m_StoryParam[(int)StoryTex::Back].pos);
		m_pStory[(int)StoryTex::Back]->SetSize(m_StoryParam[(int)StoryTex::Back].size);
		m_pStory[(int)StoryTex::Back]->SetRotate(m_StoryParam[(int)StoryTex::Back].rotate);
		m_pStory[(int)StoryTex::Back]->Setcolor(m_StoryParam[(int)StoryTex::Back].color);
		m_pStory[(int)StoryTex::Back]->SetTexture();
		m_pStory[(int)StoryTex::Back]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Back]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Back]->Disp();
		m_pStory[(int)StoryTex::FirstLinie]->SetPositon(m_StoryParam[(int)StoryTex::FirstLinie].pos);
		m_pStory[(int)StoryTex::FirstLinie]->SetSize(m_StoryParam[(int)StoryTex::FirstLinie].size);
		m_pStory[(int)StoryTex::FirstLinie]->SetRotate(m_StoryParam[(int)StoryTex::FirstLinie].rotate);
		m_pStory[(int)StoryTex::FirstLinie]->Setcolor(m_StoryParam[(int)StoryTex::FirstLinie].color);
		m_pStory[(int)StoryTex::FirstLinie]->SetTexture();
		m_pStory[(int)StoryTex::FirstLinie]->SetView(Get2DView());
		m_pStory[(int)StoryTex::FirstLinie]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::FirstLinie]->Disp();
		m_pStory[(int)StoryTex::Cinema]->SetPositon(m_StoryParam[(int)StoryTex::Cinema].pos);
		m_pStory[(int)StoryTex::Cinema]->SetSize(m_StoryParam[(int)StoryTex::Cinema].size);
		m_pStory[(int)StoryTex::Cinema]->SetRotate(m_StoryParam[(int)StoryTex::Cinema].rotate);
		m_pStory[(int)StoryTex::Cinema]->Setcolor(m_StoryParam[(int)StoryTex::Cinema].color);
		m_pStory[(int)StoryTex::Cinema]->SetTexture();
		m_pStory[(int)StoryTex::Cinema]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Cinema]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Cinema]->Disp();
		Sprite::ReSetSprite();
		break;
	case 1:
		m_pStory[(int)StoryTex::Back]->SetPositon(m_StoryParam[(int)StoryTex::Back].pos);
		m_pStory[(int)StoryTex::Back]->SetSize(m_StoryParam[(int)StoryTex::Back].size);
		m_pStory[(int)StoryTex::Back]->SetRotate(m_StoryParam[(int)StoryTex::Back].rotate);
		m_pStory[(int)StoryTex::Back]->Setcolor(m_StoryParam[(int)StoryTex::Back].color);
		m_pStory[(int)StoryTex::Back]->SetTexture();
		m_pStory[(int)StoryTex::Back]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Back]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Back]->Disp();
		m_pStory[(int)StoryTex::FirstLinie]->SetPositon(m_StoryParam[(int)StoryTex::FirstLinie].pos);
		m_pStory[(int)StoryTex::FirstLinie]->SetSize(m_StoryParam[(int)StoryTex::FirstLinie].size);
		m_pStory[(int)StoryTex::FirstLinie]->SetRotate(m_StoryParam[(int)StoryTex::FirstLinie].rotate);
		m_pStory[(int)StoryTex::FirstLinie]->Setcolor(m_StoryParam[(int)StoryTex::FirstLinie].color);
		m_pStory[(int)StoryTex::FirstLinie]->SetTexture();
		m_pStory[(int)StoryTex::FirstLinie]->SetView(Get2DView());
		m_pStory[(int)StoryTex::FirstLinie]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::FirstLinie]->Disp();
		m_pStory[(int)StoryTex::SecondLinie]->SetPositon(m_StoryParam[(int)StoryTex::SecondLinie].pos);
		m_pStory[(int)StoryTex::SecondLinie]->SetSize(m_StoryParam[(int)StoryTex::SecondLinie].size);
		m_pStory[(int)StoryTex::SecondLinie]->SetRotate(m_StoryParam[(int)StoryTex::SecondLinie].rotate);
		m_pStory[(int)StoryTex::SecondLinie]->Setcolor(m_StoryParam[(int)StoryTex::SecondLinie].color);
		m_pStory[(int)StoryTex::SecondLinie]->SetTexture();
		m_pStory[(int)StoryTex::SecondLinie]->SetView(Get2DView());
		m_pStory[(int)StoryTex::SecondLinie]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::SecondLinie]->Disp();
		m_pStory[(int)StoryTex::Cinema]->SetPositon(m_StoryParam[(int)StoryTex::Cinema].pos);
		m_pStory[(int)StoryTex::Cinema]->SetSize(m_StoryParam[(int)StoryTex::Cinema].size);
		m_pStory[(int)StoryTex::Cinema]->SetRotate(m_StoryParam[(int)StoryTex::Cinema].rotate);
		m_pStory[(int)StoryTex::Cinema]->Setcolor(m_StoryParam[(int)StoryTex::Cinema].color);
		m_pStory[(int)StoryTex::Cinema]->SetTexture();
		m_pStory[(int)StoryTex::Cinema]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Cinema]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Cinema]->Disp();
		m_pStory[(int)StoryTex::BackStar]->SetPositon(m_StoryParam[(int)StoryTex::BackStar].pos);
		m_pStory[(int)StoryTex::BackStar]->SetSize(m_StoryParam[(int)StoryTex::BackStar].size);
		m_pStory[(int)StoryTex::BackStar]->SetRotate(m_StoryParam[(int)StoryTex::BackStar].rotate);
		m_pStory[(int)StoryTex::BackStar]->Setcolor(m_StoryParam[(int)StoryTex::BackStar].color);
		m_pStory[(int)StoryTex::BackStar]->SetTexture();
		m_pStory[(int)StoryTex::BackStar]->SetView(Get2DView());
		m_pStory[(int)StoryTex::BackStar]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::BackStar]->Disp();
		m_pStory[(int)StoryTex::LinieHands]->SetPositon(m_StoryParam[(int)StoryTex::LinieHands].pos);
		m_pStory[(int)StoryTex::LinieHands]->SetSize(m_StoryParam[(int)StoryTex::LinieHands].size);
		m_pStory[(int)StoryTex::LinieHands]->SetRotate(m_StoryParam[(int)StoryTex::LinieHands].rotate);
		m_pStory[(int)StoryTex::LinieHands]->Setcolor(m_StoryParam[(int)StoryTex::LinieHands].color);
		m_pStory[(int)StoryTex::LinieHands]->SetTexture();
		m_pStory[(int)StoryTex::LinieHands]->SetView(Get2DView());
		m_pStory[(int)StoryTex::LinieHands]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::LinieHands]->Disp();
		m_pStory[(int)StoryTex::Grail]->SetPositon(m_StoryParam[(int)StoryTex::Grail].pos);
		m_pStory[(int)StoryTex::Grail]->SetSize(m_StoryParam[(int)StoryTex::Grail].size);
		m_pStory[(int)StoryTex::Grail]->SetRotate(m_StoryParam[(int)StoryTex::Grail].rotate);
		m_pStory[(int)StoryTex::Grail]->Setcolor(m_StoryParam[(int)StoryTex::Grail].color);
		m_pStory[(int)StoryTex::Grail]->SetTexture();
		m_pStory[(int)StoryTex::Grail]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Grail]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Grail]->Disp();

		Sprite::ReSetSprite();
		break;
	case 2:
		m_pStory[(int)StoryTex::Back]->SetPositon(m_StoryParam[(int)StoryTex::Back].pos);
		m_pStory[(int)StoryTex::Back]->SetSize(m_StoryParam[(int)StoryTex::Back].size);
		m_pStory[(int)StoryTex::Back]->SetRotate(m_StoryParam[(int)StoryTex::Back].rotate);
		m_pStory[(int)StoryTex::Back]->Setcolor(m_StoryParam[(int)StoryTex::Back].color);
		m_pStory[(int)StoryTex::Back]->SetTexture();
		m_pStory[(int)StoryTex::Back]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Back]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Back]->Disp();
		m_pStory[(int)StoryTex::Bosses]->SetPositon(m_StoryParam[(int)StoryTex::Bosses].pos);
		m_pStory[(int)StoryTex::Bosses]->SetSize(m_StoryParam[(int)StoryTex::Bosses].size);
		m_pStory[(int)StoryTex::Bosses]->SetRotate(m_StoryParam[(int)StoryTex::Bosses].rotate);
		m_pStory[(int)StoryTex::Bosses]->Setcolor(m_StoryParam[(int)StoryTex::Bosses].color);
		m_pStory[(int)StoryTex::Bosses]->SetTexture();
		m_pStory[(int)StoryTex::Bosses]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Bosses]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Bosses]->Disp();
		m_pStory[(int)StoryTex::SecondLinie]->SetPositon(m_StoryParam[(int)StoryTex::SecondLinie].pos);
		m_pStory[(int)StoryTex::SecondLinie]->SetSize(m_StoryParam[(int)StoryTex::SecondLinie].size);
		m_pStory[(int)StoryTex::SecondLinie]->SetRotate(m_StoryParam[(int)StoryTex::SecondLinie].rotate);
		m_pStory[(int)StoryTex::SecondLinie]->Setcolor(m_StoryParam[(int)StoryTex::SecondLinie].color);
		m_pStory[(int)StoryTex::SecondLinie]->SetTexture();
		m_pStory[(int)StoryTex::SecondLinie]->SetView(Get2DView());
		m_pStory[(int)StoryTex::SecondLinie]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::SecondLinie]->Disp();
		m_pStory[(int)StoryTex::Cinema]->SetPositon(m_StoryParam[(int)StoryTex::Cinema].pos);
		m_pStory[(int)StoryTex::Cinema]->SetSize(m_StoryParam[(int)StoryTex::Cinema].size);
		m_pStory[(int)StoryTex::Cinema]->SetRotate(m_StoryParam[(int)StoryTex::Cinema].rotate);
		m_pStory[(int)StoryTex::Cinema]->Setcolor(m_StoryParam[(int)StoryTex::Cinema].color);
		m_pStory[(int)StoryTex::Cinema]->SetTexture();
		m_pStory[(int)StoryTex::Cinema]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Cinema]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Cinema]->Disp();
		m_pStory[(int)StoryTex::BackStar]->SetPositon(m_StoryParam[(int)StoryTex::BackStar].pos);
		m_pStory[(int)StoryTex::BackStar]->SetSize(m_StoryParam[(int)StoryTex::BackStar].size);
		m_pStory[(int)StoryTex::BackStar]->SetRotate(m_StoryParam[(int)StoryTex::BackStar].rotate);
		m_pStory[(int)StoryTex::BackStar]->Setcolor(m_StoryParam[(int)StoryTex::BackStar].color);
		m_pStory[(int)StoryTex::BackStar]->SetTexture();
		m_pStory[(int)StoryTex::BackStar]->SetView(Get2DView());
		m_pStory[(int)StoryTex::BackStar]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::BackStar]->Disp();
		m_pStory[(int)StoryTex::LinieHands]->SetPositon(m_StoryParam[(int)StoryTex::LinieHands].pos);
		m_pStory[(int)StoryTex::LinieHands]->SetSize(m_StoryParam[(int)StoryTex::LinieHands].size);
		m_pStory[(int)StoryTex::LinieHands]->SetRotate(m_StoryParam[(int)StoryTex::LinieHands].rotate);
		m_pStory[(int)StoryTex::LinieHands]->Setcolor(m_StoryParam[(int)StoryTex::LinieHands].color);
		m_pStory[(int)StoryTex::LinieHands]->SetTexture();
		m_pStory[(int)StoryTex::LinieHands]->SetView(Get2DView());
		m_pStory[(int)StoryTex::LinieHands]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::LinieHands]->Disp();
		m_pStory[(int)StoryTex::Grail]->SetPositon(m_StoryParam[(int)StoryTex::Grail].pos);
		m_pStory[(int)StoryTex::Grail]->SetSize(m_StoryParam[(int)StoryTex::Grail].size);
		m_pStory[(int)StoryTex::Grail]->SetRotate(m_StoryParam[(int)StoryTex::Grail].rotate);
		m_pStory[(int)StoryTex::Grail]->Setcolor(m_StoryParam[(int)StoryTex::Grail].color);
		m_pStory[(int)StoryTex::Grail]->SetTexture();
		m_pStory[(int)StoryTex::Grail]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Grail]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Grail]->Disp();
		Sprite::ReSetSprite();
		break;
	case 3:
	case 4:
		if (m_StoryParam[(int)StoryTex::ReruxLinie].color.w != 1.0f && m_nPage  == 3)
		{
			m_pStory[(int)StoryTex::Back]->SetPositon(m_StoryParam[(int)StoryTex::Back].pos);
			m_pStory[(int)StoryTex::Back]->SetSize(m_StoryParam[(int)StoryTex::Back].size);
			m_pStory[(int)StoryTex::Back]->SetRotate(m_StoryParam[(int)StoryTex::Back].rotate);
			m_pStory[(int)StoryTex::Back]->Setcolor(m_StoryParam[(int)StoryTex::Back].color);
			m_pStory[(int)StoryTex::Back]->SetTexture();
			m_pStory[(int)StoryTex::Back]->SetView(Get2DView());
			m_pStory[(int)StoryTex::Back]->SetProjection(Get2DProj());
			m_pStory[(int)StoryTex::Back]->Disp();
			m_pStory[(int)StoryTex::Bosses]->SetPositon(m_StoryParam[(int)StoryTex::Bosses].pos);
			m_pStory[(int)StoryTex::Bosses]->SetSize(m_StoryParam[(int)StoryTex::Bosses].size);
			m_pStory[(int)StoryTex::Bosses]->SetRotate(m_StoryParam[(int)StoryTex::Bosses].rotate);
			m_pStory[(int)StoryTex::Bosses]->Setcolor(m_StoryParam[(int)StoryTex::Bosses].color);
			m_pStory[(int)StoryTex::Bosses]->SetTexture();
			m_pStory[(int)StoryTex::Bosses]->SetView(Get2DView());
			m_pStory[(int)StoryTex::Bosses]->SetProjection(Get2DProj());
			m_pStory[(int)StoryTex::Bosses]->Disp();
			m_pStory[(int)StoryTex::SecondLinie]->SetPositon(m_StoryParam[(int)StoryTex::SecondLinie].pos);
			m_pStory[(int)StoryTex::SecondLinie]->SetSize(m_StoryParam[(int)StoryTex::SecondLinie].size);
			m_pStory[(int)StoryTex::SecondLinie]->SetRotate(m_StoryParam[(int)StoryTex::SecondLinie].rotate);
			m_pStory[(int)StoryTex::SecondLinie]->Setcolor(m_StoryParam[(int)StoryTex::SecondLinie].color);
			m_pStory[(int)StoryTex::SecondLinie]->SetTexture();
			m_pStory[(int)StoryTex::SecondLinie]->SetView(Get2DView());
			m_pStory[(int)StoryTex::SecondLinie]->SetProjection(Get2DProj());
			m_pStory[(int)StoryTex::SecondLinie]->Disp();
			m_pStory[(int)StoryTex::Cinema]->SetPositon(m_StoryParam[(int)StoryTex::Cinema].pos);
			m_pStory[(int)StoryTex::Cinema]->SetSize(m_StoryParam[(int)StoryTex::Cinema].size);
			m_pStory[(int)StoryTex::Cinema]->SetRotate(m_StoryParam[(int)StoryTex::Cinema].rotate);
			m_pStory[(int)StoryTex::Cinema]->Setcolor(m_StoryParam[(int)StoryTex::Cinema].color);
			m_pStory[(int)StoryTex::Cinema]->SetTexture();
			m_pStory[(int)StoryTex::Cinema]->SetView(Get2DView());
			m_pStory[(int)StoryTex::Cinema]->SetProjection(Get2DProj());
			m_pStory[(int)StoryTex::Cinema]->Disp();
			m_pStory[(int)StoryTex::BackStar]->SetPositon(m_StoryParam[(int)StoryTex::BackStar].pos);
			m_pStory[(int)StoryTex::BackStar]->SetSize(m_StoryParam[(int)StoryTex::BackStar].size);
			m_pStory[(int)StoryTex::BackStar]->SetRotate(m_StoryParam[(int)StoryTex::BackStar].rotate);
			m_pStory[(int)StoryTex::BackStar]->Setcolor(m_StoryParam[(int)StoryTex::BackStar].color);
			m_pStory[(int)StoryTex::BackStar]->SetTexture();
			m_pStory[(int)StoryTex::BackStar]->SetView(Get2DView());
			m_pStory[(int)StoryTex::BackStar]->SetProjection(Get2DProj());
			m_pStory[(int)StoryTex::BackStar]->Disp();
			m_pStory[(int)StoryTex::LinieHands]->SetPositon(m_StoryParam[(int)StoryTex::LinieHands].pos);
			m_pStory[(int)StoryTex::LinieHands]->SetSize(m_StoryParam[(int)StoryTex::LinieHands].size);
			m_pStory[(int)StoryTex::LinieHands]->SetRotate(m_StoryParam[(int)StoryTex::LinieHands].rotate);
			m_pStory[(int)StoryTex::LinieHands]->Setcolor(m_StoryParam[(int)StoryTex::LinieHands].color);
			m_pStory[(int)StoryTex::LinieHands]->SetTexture();
			m_pStory[(int)StoryTex::LinieHands]->SetView(Get2DView());
			m_pStory[(int)StoryTex::LinieHands]->SetProjection(Get2DProj());
			m_pStory[(int)StoryTex::LinieHands]->Disp();
			m_pStory[(int)StoryTex::Grail]->SetPositon(m_StoryParam[(int)StoryTex::Grail].pos);
			m_pStory[(int)StoryTex::Grail]->SetSize(m_StoryParam[(int)StoryTex::Grail].size);
			m_pStory[(int)StoryTex::Grail]->SetRotate(m_StoryParam[(int)StoryTex::Grail].rotate);
			m_pStory[(int)StoryTex::Grail]->Setcolor(m_StoryParam[(int)StoryTex::Grail].color);
			m_pStory[(int)StoryTex::Grail]->SetTexture();
			m_pStory[(int)StoryTex::Grail]->SetView(Get2DView());
			m_pStory[(int)StoryTex::Grail]->SetProjection(Get2DProj());
			m_pStory[(int)StoryTex::Grail]->Disp();
		}
		m_pStory[(int)StoryTex::ReruxLinie]->SetPositon(m_StoryParam[(int)StoryTex::ReruxLinie].pos);
		m_pStory[(int)StoryTex::ReruxLinie]->SetSize(m_StoryParam[(int)StoryTex::ReruxLinie].size);
		m_pStory[(int)StoryTex::ReruxLinie]->SetRotate(m_StoryParam[(int)StoryTex::ReruxLinie].rotate);
		m_pStory[(int)StoryTex::ReruxLinie]->Setcolor(m_StoryParam[(int)StoryTex::ReruxLinie].color);
		m_pStory[(int)StoryTex::ReruxLinie]->SetTexture();
		m_pStory[(int)StoryTex::ReruxLinie]->SetView(Get2DView());
		m_pStory[(int)StoryTex::ReruxLinie]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::ReruxLinie]->Disp();
		m_pStory[(int)StoryTex::Cinema]->SetPositon(m_StoryParam[(int)StoryTex::Cinema].pos);
		m_pStory[(int)StoryTex::Cinema]->SetSize(m_StoryParam[(int)StoryTex::Cinema].size);
		m_pStory[(int)StoryTex::Cinema]->SetRotate(m_StoryParam[(int)StoryTex::Cinema].rotate);
		m_pStory[(int)StoryTex::Cinema]->Setcolor(m_StoryParam[(int)StoryTex::Cinema].color);
		m_pStory[(int)StoryTex::Cinema]->SetTexture();
		m_pStory[(int)StoryTex::Cinema]->SetView(Get2DView());
		m_pStory[(int)StoryTex::Cinema]->SetProjection(Get2DProj());
		m_pStory[(int)StoryTex::Cinema]->Disp();
;

		Sprite::ReSetSprite();
		break;
	}

	for (int i = 0; i < STORY_MAX_TXT; i++)
	{
		if (m_StoryTxtParam[i].color.w <= 0.0f)continue;
		m_pStoryTxt[i]->SetPositon(m_StoryTxtParam[i].pos);
		m_pStoryTxt[i]->SetSize(m_StoryTxtParam[i].size);
		m_pStoryTxt[i]->SetRotate(m_StoryTxtParam[i].rotate);
		m_pStoryTxt[i]->Setcolor(m_StoryTxtParam[i].color);
		m_pStoryTxt[i]->SetTexture();
		m_pStoryTxt[i]->SetView(Get2DView());
		m_pStoryTxt[i]->SetProjection(Get2DProj());
		m_pStoryTxt[i]->Disp();
	}
}

void CStageSelect::DrawSelect()
{
	//static float deg = 0.0f;    //度数
//deg += 5.0f;    // 度数を増やし続ける
//	float rad = DirectX::XMConvertToRadians(deg);    //ラジアンに変換する
//// 角度を増やしていく度にsinが変化していく
//// それを使いスケールをかけていく
//float ScaleX = (sinf(rad) * 0.5f + 0.5f) * 10.0f + 40.0f;
//float ScaleY = (sinf(rad + PI) * 0.5f + 0.5f) * 10.0f + 40.0f;

	static float deg = 0.0f;    //度数
	deg += 4.0f;    // 度数を増やし続ける
	float rad = DirectX::XMConvertToRadians(deg);    //ラジアンに変換する
	float cosAngle = cosf(rad);
	float Scale1 = 1.0f + cosAngle / 5.0f;
	float ArrowSize1 = 35.0f * Scale1;

	float Scale2 = 1.05f + cosAngle / 30.0f;
	float ArrowSize2 = 230.0f * Scale2;
	float ArrowSize3 = 102.0f * Scale2;

	float Scale3 = 1.05f + cosAngle / 30.0f;
	float ArrowSize4 = 230.0f * Scale2;
	float ArrowSize5 = 115.0f * Scale2;

	m_pBackGround->Draw();
	// f`æ

	if (MainStage)
	{
		SetRender3D();
		//シェーダーへ変換行列を設定
		DirectX::XMFLOAT4X4 wvp[3];
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_ModelWorldParam.pos.x, m_ModelWorldParam.pos.y, m_ModelWorldParam.pos.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(m_ModelWorldParam.rotate.x, m_ModelWorldParam.rotate.y, m_ModelWorldParam.rotate.z, 0.0f));
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_ModelWorldParam.size.x, m_ModelWorldParam.size.y, m_ModelWorldParam.size.z);
		DirectX::XMMATRIX mat = S * R * T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, mat);
		wvp[0] = world;
		wvp[1] = GetView();
		wvp[2] = GetProj();

		SetRender3D();
		Geometory::SetView(wvp[1]);
		Geometory::SetProjection(wvp[2]);
		ShaderList::SetWVP(wvp);	//引数にはXMFLOAT4x4型の、要素数３の配列のアドレスを渡すこと

		//モデルに使用する頂点シェーダー、ピクセルシェーダーを設
		m_pWorldModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pWorldModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));


		//複数のメッシュで構成されている場合ある部分は金属的な表現、ある部分は非金属的な表現と
		//分ける場合がある前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
		//切り替える
		for (int i = 0; i < m_pWorldModel->GetMeshNum(); ++i)
		{
			//メッシュに割り当てられているマテリアルを取得
			Model::Material material = *m_pWorldModel->GetMaterial(m_pWorldModel->GetMesh(i)->materialID);
			material.ambient.x = 0.85f; // x (r) 
			material.ambient.y = 0.85f; // y (g) 
			material.ambient.z = 0.85f; // z (b) 
			//シェーダーへマテリアルを設定
			ShaderList::SetMaterial(material);

			//モデルの描画
			m_pWorldModel->Draw(i);
		}
	}
	else
	{
		SetRender3D();

		switch (g_Select_type.StageMainNumber)
		{
		case GrassField:
			m_pModel[GrassField]->SetPostion(m_ModelParam[GrassField].pos.x, m_ModelParam[GrassField].pos.y, m_ModelParam[GrassField].pos.z);
			m_pModel[GrassField]->SetRotation(m_ModelParam[GrassField].rotate.x, m_ModelParam[GrassField].rotate.y, m_ModelParam[GrassField].rotate.z);
			m_pModel[GrassField]->SetScale(m_ModelParam[GrassField].size.x, m_ModelParam[GrassField].size.y, m_ModelParam[GrassField].size.z);
			m_pModel[GrassField]->SetViewMatrix(GetView());
			m_pModel[GrassField]->SetProjectionMatrix(GetProj());
			m_pModel[GrassField]->Draw();
			break;
		case DesertField:
			m_pModel[DesertField]->SetPostion(m_ModelParam[DesertField].pos.x, m_ModelParam[DesertField].pos.y, m_ModelParam[DesertField].pos.z);
			m_pModel[DesertField]->SetRotation(m_ModelParam[DesertField].rotate.x, m_ModelParam[DesertField].rotate.y, m_ModelParam[DesertField].rotate.z);
			m_pModel[DesertField]->SetScale(m_ModelParam[DesertField].size.x, m_ModelParam[DesertField].size.y, m_ModelParam[DesertField].size.z);
			m_pModel[DesertField]->SetViewMatrix(GetView());
			m_pModel[DesertField]->SetProjectionMatrix(GetProj());
			m_pModel[DesertField]->Draw();
			break;
		case SnowField:
			m_pModel[SnowField]->SetPostion(m_ModelParam[SnowField].pos.x, m_ModelParam[SnowField].pos.y, m_ModelParam[SnowField].pos.z);
			m_pModel[SnowField]->SetRotation(m_ModelParam[SnowField].rotate.x, m_ModelParam[SnowField].rotate.y, m_ModelParam[SnowField].rotate.z);
			m_pModel[SnowField]->SetScale(m_ModelParam[SnowField].size.x, m_ModelParam[SnowField].size.y, m_ModelParam[SnowField].size.z);
			m_pModel[SnowField]->SetViewMatrix(GetView());
			m_pModel[SnowField]->SetProjectionMatrix(GetProj());
			m_pModel[SnowField]->Draw();
			break;
		default:
			break;
		}
	}

	Sprite::ReSetSprite();
	SetRender2D();
	m_pStageSelect_Underbar->SetProjection(Get2DProj());
	m_pStageSelect_Underbar->SetView(Get2DView());
	m_pStageSelect_Underbar->SetTexture();
	m_pStageSelect_Underbar->SetPositon(CENTER_POS_X, CENTER_POS_Y + 510, 0.0f);
	m_pStageSelect_Underbar->SetSize(1920.0f, -60.0f, 0.0f);
	m_pStageSelect_Underbar->Disp();

	SetRender3D();
	if (MainStage)
	{
		LinieDraw();

		switch (g_Select_type.StageMainNumber)
		{
		case(GRASSLAND):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 70.0f, 0.0f);
			m_pGrassLand->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pGrassLand->Disp();

			SetRender3D();
			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 70.0f, 0.0f);
			m_pDesert->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pDesert->Disp();

			SetRender3D();
			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 70.0f, 0.0f);
			m_pSnowField->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pSnowField->Disp();

			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				SetRender2D();
				m_pWorldSelect_Backboard->SetProjection(Get2DProj());
				m_pWorldSelect_Backboard->SetView(Get2DView());
				m_pWorldSelect_Backboard->SetTexture();
				m_pWorldSelect_Backboard->SetPositon(CENTER_POS_X, CENTER_POS_Y - 491.0f, 0.0f);
				m_pWorldSelect_Backboard->SetSize(820.0f, -100.0f, 0.0f);
				m_pWorldSelect_Backboard->Disp();

				Sprite::ReSetSprite();
				SetRender3D();
				m_pWorld[0]->SetProjection(GetProj());
				m_pWorld[0]->SetView(GetView());
				m_pWorld[0]->SetTexture();
				m_pWorld[0]->SetPositon(0.0f, 170.0f, 145.0f);
				m_pWorld[0]->SetSize(155.0f, 24.0f, 100.0f);
				m_pWorld[0]->Disp();

				if (m_bClear[GRASSLAND_STAGE3])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(180.0f, 105.0f, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{

					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f;
						m_pRight_Select_Lock->SetPositon(posAjust, 105.0f, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(180.0f, 105.0f, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();
				}
				if (m_bClear[DESERT_STAGE3])
				{
					Sprite::ReSetSprite();
					m_pLeft_Select->SetProjection(GetProj());
					m_pLeft_Select->SetView(GetView());
					m_pLeft_Select->SetTexture();
					m_pLeft_Select->SetPositon(-180.0f, 105.0f, 145.0f);
					m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pLeft_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pLeft_Select_Lock->SetProjection(GetProj());
					m_pLeft_Select_Lock->SetView(GetView());
					m_pLeft_Select_Lock->SetTexture();
					if (m_bCantMove_Left)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f;
						m_pLeft_Select_Lock->SetPositon(-posAjust, 105.0f, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Left = false;
						}
						count++;
					}
					else
					{
						m_pLeft_Select_Lock->SetPositon(-180.0f, 105.0f, 145.0f);

					}
					m_pLeft_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pLeft_Select_Lock->Disp();
				}


			}
			break;
		case(DESERT):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 70.0f, 0.0f);
			m_pGrassLand->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pGrassLand->Disp();

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 70.0f, 0.0f);
			m_pDesert->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 70.0f, 0.0f);
			m_pSnowField->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pSnowField->Disp();
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				SetRender2D();
				m_pWorldSelect_Backboard->SetProjection(Get2DProj());
				m_pWorldSelect_Backboard->SetView(Get2DView());
				m_pWorldSelect_Backboard->SetTexture();
				m_pWorldSelect_Backboard->SetPositon(CENTER_POS_X, CENTER_POS_Y - 491.0f, 0.0f);
				m_pWorldSelect_Backboard->SetSize(820.0f, -100.0f, 0.0f);
				m_pWorldSelect_Backboard->Disp();

				Sprite::ReSetSprite();
				SetRender3D();
				m_pWorld[1]->SetProjection(GetProj());
				m_pWorld[1]->SetView(GetView());
				m_pWorld[1]->SetTexture();
				m_pWorld[1]->SetPositon(0.0f, 170.0f, 145.0f);
				m_pWorld[1]->SetSize(155.0f, 24.0f, 100.0f);
				m_pWorld[1]->Disp();

				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(-180.0f, 105.0f, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				if (m_bClear[DESERT_STAGE3])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(180.0f, 105.0f, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f;
						m_pRight_Select_Lock->SetPositon(posAjust, 105.0f, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(180.0f, 105.0f, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();
				}
			}
			break;
		case(SNOWFIELD):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 70.0f, 0.0f);
			m_pGrassLand->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pGrassLand->Disp();

			SetRender3D();
			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 70.0f, 0.0f);
			m_pDesert->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 70.0f, 0.0f);
			m_pSnowField->SetSize(WORLD_SIZE_X + 10.0f, WORLD_SIZE_Y, 100.0f);
			m_pSnowField->Disp();
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				SetRender2D();
				m_pWorldSelect_Backboard->SetProjection(Get2DProj());
				m_pWorldSelect_Backboard->SetView(Get2DView());
				m_pWorldSelect_Backboard->SetTexture();
				m_pWorldSelect_Backboard->SetPositon(CENTER_POS_X, CENTER_POS_Y - 491.0f, 0.0f);
				m_pWorldSelect_Backboard->SetSize(820.0f, -100.0f, 0.0f);
				m_pWorldSelect_Backboard->Disp();

				Sprite::ReSetSprite();
				SetRender3D();
				m_pWorld[2]->SetProjection(GetProj());
				m_pWorld[2]->SetView(GetView());
				m_pWorld[2]->SetTexture();
				m_pWorld[2]->SetPositon(0.0f, 170.0f, 145.0f);
				m_pWorld[2]->SetSize(155.0f, 24.0f, 100.0f);
				m_pWorld[2]->Disp();

				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(-170.0f, 105.0f, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				Sprite::ReSetSprite();
				m_pRight_Select->SetProjection(GetProj());
				m_pRight_Select->SetView(GetView());
				m_pRight_Select->SetTexture();
				m_pRight_Select->SetPositon(170.0f, 105.0f, 145.0f);
				m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pRight_Select->Disp();

			}
			break;
		}

		if (m_bMoving == false)
		{
			SetRender2D();

			//m_pDecition->SetView(Get2DView());
			//m_pDecition->SetProjection(Get2DProj());
			//m_pDecition->SetPositon(CENTER_POS_X + m_tDecitionPos[0].x, CENTER_POS_Y + m_tDecitionPos[0].y, 0.0f);
			//m_pDecition->SetSize(100.0f, 100.0f, 100.0f);
			//m_pDecition->SetRotation(0.0f, 0.0f, 0.0f);
			//m_pDecition->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
			//m_pDecition->SetUvPos(2.0f / 4.0f, 3.0f / 10.0f);
			//m_pDecition->SetUvSize(1.0f / 4.0f, 1.0f / 10.0f);
			//m_pDecition->SetTexture();
			//m_pDecition->Disp();

			//m_pDecition->SetPositon(CENTER_POS_X + m_tDecitionPos[1].x, CENTER_POS_Y + m_tDecitionPos[1].y, 0.0f);
			//m_pDecition->Disp();

			Sprite::ReSetSprite();
			m_pStageSelected->SetProjection(GetProj());
			m_pStageSelected->SetView(GetView());
			m_pStageSelected->SetTexture();
			m_pStageSelected->SetPositon(posX[g_Select_type.StageMainNumber], 103.0f, 140.0f);
			m_pStageSelected->SetSize(ArrowSize2, ArrowSize3, 100.0f);
			m_pStageSelected->Disp();

		}
	}
	else
	{
#if	1
		LinieDraw();
		if (g_Select_type.StageMainNumber == GRASSLAND)QrackerDraw();
		if (g_Select_type.StageMainNumber == DESERT)NugarDraw();
		if (g_Select_type.StageMainNumber == SNOWFIELD)KanneleDraw();
		if (g_Select_type.StageMainNumber == SNOWFIELD)BoldowDraw();
		switch (g_Select_type.StageSubNumber)
		{

		case(GRASSLAND_STAGE1):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLandStage[0]->SetProjection(GetProj());
			m_pGrassLandStage[0]->SetView(GetView());
			m_pGrassLandStage[0]->SetTexture();
			m_pGrassLandStage[0]->SetPositon(subposX[0], UI_POS_Y, 145.0f);
			m_pGrassLandStage[0]->SetSize(220.0f, 120.0f, 100.0f);
			m_pGrassLandStage[0]->Disp();

			Sprite::ReSetSprite();
			m_pGrassLandStage[1]->SetProjection(GetProj());
			m_pGrassLandStage[1]->SetView(GetView());
			m_pGrassLandStage[1]->SetTexture();
			m_pGrassLandStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			//if (!m_bMoving)
			m_pGrassLandStage[1]->SetSize(210.0f, 110.0f, 100.0f);
			m_pGrassLandStage[1]->Disp();

			if (CSceneResult::GetStageClear(0))
			{
				if (CSceneResult::GetRankData(0) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(0) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(0) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(0) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}

			if (m_bMoving == false)
			{
				if (m_bClear[GRASSLAND_STAGE1])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(subposX[0] + 180.0f, UI_POS_Y, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f + subposX[0];
						m_pRight_Select_Lock->SetPositon(posAjust, UI_POS_Y, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(subposX[0] + 180.0f, UI_POS_Y, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();

				}

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[0], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}
			break;

		case(GRASSLAND_STAGE2):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLandStage[0]->SetProjection(GetProj());
			m_pGrassLandStage[0]->SetView(GetView());
			m_pGrassLandStage[0]->SetTexture();
			m_pGrassLandStage[0]->SetPositon(subposX[0], UI_POS_Y, 145.0f);
			m_pGrassLandStage[0]->SetSize(210.0f, 110.0f, 100.0f);
			m_pGrassLandStage[0]->Disp();

			Sprite::ReSetSprite();
			m_pGrassLandStage[1]->SetProjection(GetProj());
			m_pGrassLandStage[1]->SetView(GetView());
			m_pGrassLandStage[1]->SetTexture();
			m_pGrassLandStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pGrassLandStage[1]->SetSize(220.0f, 120.0f, 100.0f);
			m_pGrassLandStage[1]->Disp();

			Sprite::ReSetSprite();
			m_pGrassLandStage[2]->SetProjection(GetProj());
			m_pGrassLandStage[2]->SetView(GetView());
			m_pGrassLandStage[2]->SetTexture();
			m_pGrassLandStage[2]->SetPositon(subposX[2], UI_POS_Y, 145.0f);
			m_pGrassLandStage[2]->SetSize(210.0f, 110.0f, 100.0f);
			m_pGrassLandStage[2]->Disp();

			if (CSceneResult::GetStageClear(1))
			{
				if (CSceneResult::GetRankData(1) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(1) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(1) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(1) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}
			if (m_bMoving == false)
			{
				if (m_bClear[GRASSLAND_STAGE2])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(subposX[1] + 180.0f, UI_POS_Y, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f + subposX[1];
						m_pRight_Select_Lock->SetPositon(posAjust, UI_POS_Y, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(subposX[1] + 180.0f, UI_POS_Y, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();
				}
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(subposX[1] - 180.0f, UI_POS_Y, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[1], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}
			break;

		case(GRASSLAND_STAGE3):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLandStage[1]->SetProjection(GetProj());
			m_pGrassLandStage[1]->SetView(GetView());
			m_pGrassLandStage[1]->SetTexture();
			m_pGrassLandStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pGrassLandStage[1]->SetSize(210.0f, 110.0f, 100.0f);
			m_pGrassLandStage[1]->Disp();

			Sprite::ReSetSprite();
			m_pGrassLandStage[2]->SetProjection(GetProj());
			m_pGrassLandStage[2]->SetView(GetView());
			m_pGrassLandStage[2]->SetTexture();
			m_pGrassLandStage[2]->SetPositon(subposX[2], UI_POS_Y, 145.0f);
			m_pGrassLandStage[2]->SetSize(220.0f, 120.0f, 100.0f);
			m_pGrassLandStage[2]->Disp();

			if (CSceneResult::GetStageClear(2))
			{
				if (CSceneResult::GetRankData(2) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(2) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(2) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(2) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}

			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(subposX[2] + -180.0f, UI_POS_Y, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[2], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}
			break;
		case(DESERT_STAGE1):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pDesertStage[0]->SetProjection(GetProj());
			m_pDesertStage[0]->SetView(GetView());
			m_pDesertStage[0]->SetTexture();
			m_pDesertStage[0]->SetPositon(subposX[0], UI_POS_Y, 145.0f);
			m_pDesertStage[0]->SetSize(220.0f, 120.0f, 100.0f);
			m_pDesertStage[0]->Disp();

			Sprite::ReSetSprite();
			m_pDesertStage[1]->SetProjection(GetProj());
			m_pDesertStage[1]->SetView(GetView());
			m_pDesertStage[1]->SetTexture();
			m_pDesertStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pDesertStage[1]->SetSize(210.0f, 110.0f, 100.0f);
			m_pDesertStage[1]->Disp();

			if (CSceneResult::GetStageClear(3))
			{
				if (CSceneResult::GetRankData(3) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(3) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(3) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(3) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}

			if (m_bMoving == false)
			{
				if (m_bClear[DESERT_STAGE1])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(subposX[0] + 180.0f, UI_POS_Y, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f + subposX[1];
						m_pRight_Select_Lock->SetPositon(posAjust, UI_POS_Y, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(subposX[1] + 180.0f, UI_POS_Y, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();
				}

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[0], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}
			break;

		case(DESERT_STAGE2):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pDesertStage[0]->SetProjection(GetProj());
			m_pDesertStage[0]->SetView(GetView());
			m_pDesertStage[0]->SetTexture();
			m_pDesertStage[0]->SetPositon(subposX[0], UI_POS_Y, 145.0f);
			m_pDesertStage[0]->SetSize(210.0f, 110.0f, 100.0f);
			m_pDesertStage[0]->Disp();

			Sprite::ReSetSprite();
			m_pDesertStage[1]->SetProjection(GetProj());
			m_pDesertStage[1]->SetView(GetView());
			m_pDesertStage[1]->SetTexture();
			m_pDesertStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pDesertStage[1]->SetSize(220.0f, 120.0f, 100.0f);
			m_pDesertStage[1]->Disp();

			Sprite::ReSetSprite();
			m_pDesertStage[2]->SetProjection(GetProj());
			m_pDesertStage[2]->SetView(GetView());
			m_pDesertStage[2]->SetTexture();
			m_pDesertStage[2]->SetPositon(subposX[2], UI_POS_Y, 145.0f);
			m_pDesertStage[2]->SetSize(210.0f, 110.0f, 100.0f);
			m_pDesertStage[2]->Disp();

			if (CSceneResult::GetStageClear(5))
			{
				if (CSceneResult::GetRankData(4) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(4) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(4) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(4) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}

			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(subposX[1] + -180.0f, UI_POS_Y, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				if (m_bClear[DESERT_STAGE2])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(subposX[1] + 180.0f, UI_POS_Y, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f + subposX[1];
						m_pRight_Select_Lock->SetPositon(posAjust, UI_POS_Y, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(subposX[1] + 180.0f, UI_POS_Y, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();
				}

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[1], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}

			break;

		case(DESERT_STAGE3):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pDesertStage[1]->SetProjection(GetProj());
			m_pDesertStage[1]->SetView(GetView());
			m_pDesertStage[1]->SetTexture();
			m_pDesertStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pDesertStage[1]->SetSize(210.0f, 110.0f, 100.0f);
			m_pDesertStage[1]->Disp();

			Sprite::ReSetSprite();
			m_pDesertStage[2]->SetProjection(GetProj());
			m_pDesertStage[2]->SetView(GetView());
			m_pDesertStage[2]->SetTexture();
			m_pDesertStage[2]->SetPositon(subposX[2], UI_POS_Y, 145.0f);
			m_pDesertStage[2]->SetSize(220.0f, 120.0f, 100.0f);
			m_pDesertStage[2]->Disp();

			if (CSceneResult::GetStageClear(5))
			{
				if (CSceneResult::GetRankData(5) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(5) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(5) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(5) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}

			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(subposX[2] + -180.0f, UI_POS_Y, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[2], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}

			break;
		case(SNOWFIELD_STAGE1):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pSnowFieldStage[0]->SetProjection(GetProj());
			m_pSnowFieldStage[0]->SetView(GetView());
			m_pSnowFieldStage[0]->SetTexture();
			m_pSnowFieldStage[0]->SetPositon(subposX[0], UI_POS_Y, 145.0f);
			m_pSnowFieldStage[0]->SetSize(220.0f, 120.0f, 100.0f);
			m_pSnowFieldStage[0]->Disp();

			Sprite::ReSetSprite();
			m_pSnowFieldStage[1]->SetProjection(GetProj());
			m_pSnowFieldStage[1]->SetView(GetView());
			m_pSnowFieldStage[1]->SetTexture();
			m_pSnowFieldStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pSnowFieldStage[1]->SetSize(210.0f, 110.0f, 100.0f);
			m_pSnowFieldStage[1]->Disp();

			if (CSceneResult::GetStageClear(6))
			{
				if (CSceneResult::GetRankData(6) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(6) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(6) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(6) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[0] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}

			if (m_bMoving == false)
			{
				if (m_bClear[SNOWFIELD_STAGE1])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(subposX[0] + 180.0f, UI_POS_Y, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f + subposX[0];
						m_pRight_Select_Lock->SetPositon(posAjust, UI_POS_Y, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(subposX[0] + 180.0f, UI_POS_Y, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();
				}

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[0], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}
			break;

		case(SNOWFIELD_STAGE2):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pSnowFieldStage[0]->SetProjection(GetProj());
			m_pSnowFieldStage[0]->SetView(GetView());
			m_pSnowFieldStage[0]->SetTexture();
			m_pSnowFieldStage[0]->SetPositon(subposX[0], UI_POS_Y, 145.0f);
			m_pSnowFieldStage[0]->SetSize(210.0f, 110.0f, 100.0f);
			m_pSnowFieldStage[0]->Disp();

			Sprite::ReSetSprite();
			m_pSnowFieldStage[1]->SetProjection(GetProj());
			m_pSnowFieldStage[1]->SetView(GetView());
			m_pSnowFieldStage[1]->SetTexture();
			m_pSnowFieldStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pSnowFieldStage[1]->SetSize(220.0f, 120.0f, 100.0f);
			m_pSnowFieldStage[1]->Disp();

			Sprite::ReSetSprite();
			m_pSnowFieldStage[2]->SetProjection(GetProj());
			m_pSnowFieldStage[2]->SetView(GetView());
			m_pSnowFieldStage[2]->SetTexture();
			m_pSnowFieldStage[2]->SetPositon(subposX[2], UI_POS_Y, 145.0f);
			m_pSnowFieldStage[2]->SetSize(210.0f, 110.0f, 100.0f);
			m_pSnowFieldStage[2]->Disp();
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(subposX[1] + -180.0f, UI_POS_Y, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				if (CSceneResult::GetStageClear(6))
				{
					if (CSceneResult::GetRankData(7) == C_RANK)
					{
						SetRender2D();
						Sprite::ReSetSprite();
						m_pRank_C->SetProjection(GetProj());
						m_pRank_C->SetView(GetView());
						m_pRank_C->SetTexture();
						m_pRank_C->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
						m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
						m_pRank_C->Disp();
					}
					else if (CSceneResult::GetRankData(7) == B_RANK)
					{
						SetRender2D();
						Sprite::ReSetSprite();
						m_pRank_B->SetProjection(GetProj());
						m_pRank_B->SetView(GetView());
						m_pRank_B->SetTexture();
						m_pRank_B->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
						m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
						m_pRank_B->Disp();
					}
					else if (CSceneResult::GetRankData(7) == A_RANK)
					{
						SetRender2D();
						Sprite::ReSetSprite();
						m_pRank_A->SetProjection(GetProj());
						m_pRank_A->SetView(GetView());
						m_pRank_A->SetTexture();
						m_pRank_A->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
						m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
						m_pRank_A->Disp();
					}
					else if (CSceneResult::GetRankData(7) == S_RANK)
					{
						SetRender2D();
						Sprite::ReSetSprite();
						m_pRank_S->SetProjection(GetProj());
						m_pRank_S->SetView(GetView());
						m_pRank_S->SetTexture();
						m_pRank_S->SetPositon(subposX[1] + 60, UI_POS_Y + 20.0f, 140.0f);
						m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
						m_pRank_S->Disp();
					}
				}


				if (m_bClear[SNOWFIELD_STAGE2])
				{
					Sprite::ReSetSprite();
					m_pRight_Select->SetProjection(GetProj());
					m_pRight_Select->SetView(GetView());
					m_pRight_Select->SetTexture();
					m_pRight_Select->SetPositon(subposX[1] + 180.0f, UI_POS_Y, 145.0f);
					m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
					m_pRight_Select->Disp();
				}
				else
				{
					Sprite::ReSetSprite();
					m_pRight_Select_Lock->SetProjection(GetProj());
					m_pRight_Select_Lock->SetView(GetView());
					m_pRight_Select_Lock->SetTexture();
					if (m_bCantMove_Right)
					{
						static int count = 0;
						static float rad = 0.0f;
						rad = DirectX::XMConvertToRadians(count * 60);
						float cosScale = cosf(rad) - 0.5f;
						float posAjust = 10.0f * cosScale + 180.0f + subposX[1];
						m_pRight_Select_Lock->SetPositon(posAjust, UI_POS_Y, 145.0f);
						if (count >= 10)
						{
							count = 0;
							m_bCantMove_Right = false;
						}
						count++;
					}
					else
					{
						m_pRight_Select_Lock->SetPositon(subposX[1] + 180.0f, UI_POS_Y, 145.0f);

					}
					m_pRight_Select_Lock->SetSize(35.0f, 35.0f, 100.0f);
					m_pRight_Select_Lock->Disp();
				}

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[1], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();

			}
			break;

		case(SNOWFIELD_STAGE3):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pSnowFieldStage[1]->SetProjection(GetProj());
			m_pSnowFieldStage[1]->SetView(GetView());
			m_pSnowFieldStage[1]->SetTexture();
			m_pSnowFieldStage[1]->SetPositon(subposX[1], UI_POS_Y, 145.0f);
			m_pSnowFieldStage[1]->SetSize(210.0f, 110.0f, 100.0f);
			m_pSnowFieldStage[1]->Disp();

			Sprite::ReSetSprite();
			m_pSnowFieldStage[2]->SetProjection(GetProj());
			m_pSnowFieldStage[2]->SetView(GetView());
			m_pSnowFieldStage[2]->SetTexture();
			m_pSnowFieldStage[2]->SetPositon(subposX[2], UI_POS_Y, 145.0f);
			m_pSnowFieldStage[2]->SetSize(220.0f, 120.0f, 100.0f);
			m_pSnowFieldStage[2]->Disp();

			if (CSceneResult::GetStageClear(8))
			{
				if (CSceneResult::GetRankData(8) == C_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_C->SetProjection(GetProj());
					m_pRank_C->SetView(GetView());
					m_pRank_C->SetTexture();
					m_pRank_C->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_C->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_C->Disp();
				}
				else if (CSceneResult::GetRankData(8) == B_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_B->SetProjection(GetProj());
					m_pRank_B->SetView(GetView());
					m_pRank_B->SetTexture();
					m_pRank_B->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_B->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_B->Disp();
				}
				else if (CSceneResult::GetRankData(8) == A_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_A->SetProjection(GetProj());
					m_pRank_A->SetView(GetView());
					m_pRank_A->SetTexture();
					m_pRank_A->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_A->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_A->Disp();
				}
				else if (CSceneResult::GetRankData(8) == S_RANK)
				{
					SetRender2D();
					Sprite::ReSetSprite();
					m_pRank_S->SetProjection(GetProj());
					m_pRank_S->SetView(GetView());
					m_pRank_S->SetTexture();
					m_pRank_S->SetPositon(subposX[2] + 60, UI_POS_Y + 20.0f, 140.0f);
					m_pRank_S->SetSize(90.0f, 75.0f, 100.0f);
					m_pRank_S->Disp();
				}
			}
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(subposX[2] + -180.0f, UI_POS_Y, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(subposX[2], UI_POS_Y - 12.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
				m_pStageSelected->Disp();
			}

			break;

		}
#endif
	}
}
