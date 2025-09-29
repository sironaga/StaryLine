#include "SceneTutorial.h"
#include "InputEx.h"
#include "Main.h"
#include "Defines.h"
#include "Sprite.h"

constexpr int ce_nMaxPage[(int)TutorialSection::Max] =
{
	3,
	2,
	2,
	2,
	1,
	2,
	3,

};



CSceneTutorial::CSceneTutorial(StageType StageNum)
	: m_bEnd(false)
	, m_eSection(TutorialSection::Section1)
	, m_pTextureArray{}, m_pBackGround(nullptr)
	, m_nCurrentPage(0), m_fTime(0.0f)
	, m_nBeforeVertex(-1), m_bSpownEffectDraw(false)
{
	std::string section = "Assets/Texture/Tutorial/Section";
	for (int i = 0; i < (int)TutorialSection::Max; i++)
	{
		for (int j = 0; j < ce_nMaxPage[i]; j++)
		{
			std::string page = "/Text" + std::to_string(i + 1)  + "_" + std::to_string(j + 1);
			std::string path = section + page + ".png";

			m_pTextureArray[i].push_back(new Texture());
			if (FAILED(m_pTextureArray[i][j]->Create(path.c_str())))
				MessageBox(NULL, path.c_str(), "LoadError", MB_OK);
		}
	}

	m_pBackGround = new Texture();
	if (FAILED(m_pBackGround->Create(TEX_PASS("Tutorial/TutorialBack.png"))))
		MessageBox(NULL, "Tutorial/TutorialBack.png", "LoadError", MB_OK);
	
	m_tBackParam = SpriteParam();
	m_tBackParam.size = DirectX::XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_tBackParam.world = Get2DWorld();
	m_tBackParam.view = Get2DView();
	m_tBackParam.proj = Get2DProj();
	
	m_tTextParam = SpriteParam();
	m_tTextParam.pos = DirectX::XMFLOAT2(0.0f, -460.0f);
	m_tTextParam.size = DirectX::XMFLOAT2(SCREEN_WIDTH - 950.0f, 80.0f);
	m_tTextParam.world = Get2DWorld();
	m_tTextParam.view = Get2DView();
	m_tTextParam.proj = Get2DProj();

	CCharacterManager::GetInstance()->InitCharacterTexture(StageNum);//キャラクターテクスチャ～の初期化
	
	m_pBackGround2 = new CBackGround();
	m_pFieldVertex = new CFieldVertex();
	m_pBattle = new CBattle(StageNum);
	m_pPlayer = new CPlayer();
	m_pField = new Field(StageNum);
	m_pStarLine = new StarLine();


	m_pFieldVertex->SetBattleAddress(m_pBattle);
	m_pFieldVertex->SetPlayerAddress(m_pPlayer);
	m_pPlayer->SetFieldVertexAddress(m_pFieldVertex);
	m_pFieldVertex->SetStarLineAddress(m_pStarLine);
	

	ID3D11SamplerState* pGameSPSampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	HRESULT hr = GetDevice()->CreateSamplerState(&samplerDesc, &pGameSPSampler);
	if (FAILED(hr)) { return; }
	GetContext()->PSSetSamplers(0, 1, &pGameSPSampler);
}

CSceneTutorial::~CSceneTutorial()
{
	for (int i = 0; i < (int)TutorialSection::Max; i++)
	{
		for (int j = 0; j < ce_nMaxPage[i]; j++)
		{
			SAFE_DELETE(m_pTextureArray[i][j]);
		}
	}

	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pFieldVertex);
	SAFE_DELETE(m_pBattle);
	SAFE_DELETE(m_pField);
	SAFE_DELETE(m_pBackGround2);
	SAFE_DELETE(m_pBackGround);

	CCharacterManager::GetInstance()->UnInitCharacterTexture();
}

void CSceneTutorial::Update()
{
	if (IsKeyTrigger(VK_ESCAPE) && !m_bEnd)
	{
		m_pBattle->AllTutorialFlagClear();
		m_bEnd = true;
		SetNext(STAGE_SELECT);
	}

	CCharacterManager::GetInstance()->ModelUpDate();

	switch (m_eSection)
	{
	case TutorialSection::Section1:
		UpdateSection1();
		break;
	case TutorialSection::Section2:
		UpdateSection2();
		break;
	case TutorialSection::Section3:
		UpdateSection3();
		break;
	case TutorialSection::Section4:
		UpdateSection4();
		break;
	case TutorialSection::Section5:
		UpdateSection5();
		break;
	case TutorialSection::Section6:
		UpdateSection6();
		break;
	case TutorialSection::Section7:
		UpdateSection7();
		break;
	case TutorialSection::Max:
		break;
	default:
		break;
	}

#ifdef _DEBUG
	// デバッグ用進行キー
	if (IsKeyTrigger(VK_RIGHT))
	{
		if (m_eSection == TutorialSection::Section7 && m_nCurrentPage == ce_nMaxPage[(int)TutorialSection::Section7] - 1)
		{
			return;
		}
		m_nCurrentPage++;
		if (m_nCurrentPage >= ce_nMaxPage[(int)m_eSection])
		{
			m_nCurrentPage = 0;
			int section = (int)m_eSection;
			section++;
			m_eSection = (TutorialSection)section;
		}
	}
	if (IsKeyTrigger(VK_LEFT))
	{
		if (m_eSection == TutorialSection::Section1 && m_nCurrentPage == 0)
		{
			return;
		}
		m_nCurrentPage--;
		if (m_nCurrentPage < 0)
		{
			int section = (int)m_eSection;
			section--;
			m_eSection = (TutorialSection)section;
			m_nCurrentPage = ce_nMaxPage[(int)m_eSection] - 1;
		}
	}
#endif // _DEBUG
}

void CSceneTutorial::Draw()
{
	m_pBackGround2->Draw();
	m_pField->Draw();
	m_pFieldVertex->ShapesDraw();
	if (m_bSpownEffectDraw)m_pFieldVertex->ShapesEffectDraw();
	m_pFieldVertex->Draw();
	m_pPlayer->Draw();

	std::thread Th_BattleDraw([this]() { this->m_pBattle->Draw(); });
	Th_BattleDraw.join();

	if (!m_bSpownEffectDraw)
	{
		SetRender2D();
		Sprite::ReSetSprite();
		Sprite::SetParam(m_tBackParam);
		Sprite::SetTexture(m_pBackGround);
		Sprite::Draw();
		Sprite::ReSetSprite();
		Sprite::SetParam(m_tTextParam);
		Sprite::SetTexture(m_pTextureArray[(int)m_eSection][m_nCurrentPage]);
		Sprite::Draw();
	}
}

void CSceneTutorial::SetInstance()
{

}

void CSceneTutorial::NextPage()
{
	if (m_eSection == TutorialSection::Section7 && m_nCurrentPage == ce_nMaxPage[(int)TutorialSection::Section7] - 1)
	{
		return;
	}
	m_nCurrentPage++;
	if (m_nCurrentPage >= ce_nMaxPage[(int)m_eSection])
	{
		m_nCurrentPage = 0;
		int section = (int)m_eSection;
		section++;
		m_eSection = (TutorialSection)section;
	}
}

void CSceneTutorial::BackPage()
{
	if (m_eSection == TutorialSection::Section1 && m_nCurrentPage == 0)
	{
		return;
	}
	m_nCurrentPage--;
	if (m_nCurrentPage < 0)
	{
		int section = (int)m_eSection;
		section--;
		m_eSection = (TutorialSection)section;
		m_nCurrentPage = ce_nMaxPage[(int)m_eSection] - 1;
	}
}

void CSceneTutorial::UpdateSection1()
{

	m_pFieldVertex->SetFeverInclease(false);
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_pPlayer->SetNowVertex(12);
		m_pPlayer->SetPos(m_pFieldVertex->GetVertexPos(12));
		for (int i = 0; i < MAX_VERTEX; i++)
		{
			m_pFieldVertex->SetVertexStop(true, i);
		}
		m_pFieldVertex->SetVertexStop(false, 11);
		if (m_fTime >= 2.0f)
		{
			NextPage();
			m_fTime = 0.0f;
		}
		break;
	case 1:
		m_pFieldVertex->SetVertexStop(true, m_pPlayer->GetNowVertex());
		switch (m_pPlayer->GetNowVertex())
		{
		case 11:
			m_pFieldVertex->SetVertexStop(false, 6);
			break;
		case 6:
			m_pFieldVertex->SetVertexStop(false, 1);
			break;
		case 1:
			m_pFieldVertex->SetVertexStop(false, 5);
			break;
		case 5:
			m_pFieldVertex->SetVertexStop(false, 10);
			break;
		case 10:
			m_pFieldVertex->SetVertexStop(false, 16);
			break;
		case 16:
			m_pFieldVertex->SetVertexStop(false, 20);

			break;
		case 20:
			m_fTime = 0.0f;
			NextPage();
		default:
			break;
		}

		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(true);
		m_pBattle->Update();
		m_pPlayer->Update();
		m_pPlayer->TimerReCharge();
		break;
	case 2:
		if (m_fTime >= 2.0f)
		{
			NextPage();
			m_fTime = 0.0f;
			m_pPlayer->TimerSetMax();
		}
		break;

	default:
		break;
	}

	m_fTime += 1.0f / fFPS;
}

void CSceneTutorial::UpdateSection2()
{
	m_pFieldVertex->SetFeverInclease(false);
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		
		m_pPlayer->SetNowVertex(12);
		m_pPlayer->SetPos(m_pFieldVertex->GetVertexPos(12));
		m_pFieldVertex->InitFieldVertex();

		for (int i = 0; i < MAX_VERTEX; i++)
		{
			m_pFieldVertex->SetVertexStop(true, i);
		}
		m_pFieldVertex->SetVertexStop(false, 13);
		m_pFieldVertex->SetVertexStop(false, 7);
		if (m_fTime >= 2.0f)
		{
			NextPage();
			m_fTime = 0.0f;
		}
		break;
	case 1:
		m_pFieldVertex->SetVertexStop(true, m_pPlayer->GetNowVertex());
		switch (m_pPlayer->GetNowVertex())
		{
		case 13:
			if (m_nBeforeVertex == 12) { m_pFieldVertex->SetVertexStop(false, 14); m_pFieldVertex->SetVertexStop(true, 7); }
			else if (m_nBeforeVertex == 14)m_pFieldVertex->SetVertexStop(false, 12);
			m_nBeforeVertex = 13;
			break;
		case 14:
			if(m_nBeforeVertex == 13)m_pFieldVertex->SetVertexStop(false, 8);
			else if (m_nBeforeVertex == 8)m_pFieldVertex->SetVertexStop(false, 13);
			m_nBeforeVertex = 14;
			break;
		case 8:
			if(m_nBeforeVertex == 14)m_pFieldVertex->SetVertexStop(false, 2);
			else if (m_nBeforeVertex == 2)m_pFieldVertex->SetVertexStop(false, 14);
			m_nBeforeVertex = 8;
			break;
		case 2:
			if(m_nBeforeVertex == 8)m_pFieldVertex->SetVertexStop(false, 7);
			else if (m_nBeforeVertex == 7)m_pFieldVertex->SetVertexStop(false, 8);
			m_nBeforeVertex = 2;
			break;
		case 7:
			if(m_nBeforeVertex == 2)m_pFieldVertex->SetVertexStop(false, 12);
			else if (m_nBeforeVertex == 12) { m_pFieldVertex->SetVertexStop(false, 2); m_pFieldVertex->SetVertexStop(true, 13); }
			m_nBeforeVertex = 7;
			break;
		case 12:
			if (m_nBeforeVertex == 7 || m_nBeforeVertex == 13)
			{
				m_fTime = 0.0f;
				NextPage();
			}
			else
			{
				m_nBeforeVertex = 12;
			}
		default:
			break;
		}

		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(true);
		m_pBattle->Update();
		m_pPlayer->Update();
		m_pPlayer->TimerReCharge();
		break;

	default:
		break;
	}

	m_fTime += 1.0f / fFPS;
}

void CSceneTutorial::UpdateSection3()
{
	m_pFieldVertex->SetFeverInclease(false);
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_pPlayer->TimerSetMax();
		m_pFieldVertex->SetVertexStop(false, 17);
		m_pFieldVertex->SetVertexStop(false, 11);
		if (m_fTime >= 2.0f)
		{
			NextPage();
			m_fTime = 0.0f;
		}
		break;
	case 1:
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(true);
		m_pBattle->Update();
		m_pPlayer->Update();
		m_pPlayer->TimerReCharge();

		m_pFieldVertex->SetVertexStop(true, m_pPlayer->GetNowVertex());
		switch (m_pPlayer->GetNowVertex())
		{
		case 17:
			if (m_nBeforeVertex == 12) { m_pFieldVertex->SetVertexStop(false, 22); m_pFieldVertex->SetVertexStop(true, 11); }
			else if (m_nBeforeVertex == 22)m_pFieldVertex->SetVertexStop(false, 12);
			m_nBeforeVertex = 17;
			break;
		case 22:
			if (m_nBeforeVertex == 17)m_pFieldVertex->SetVertexStop(false, 21);
			else if (m_nBeforeVertex == 21)m_pFieldVertex->SetVertexStop(false, 17);
			m_nBeforeVertex = 22;
			break;
		case 21:
			if(m_nBeforeVertex == 22)m_pFieldVertex->SetVertexStop(false, 20);
			else if (m_nBeforeVertex == 20)m_pFieldVertex->SetVertexStop(false, 22);
			m_nBeforeVertex = 21;
			break;
		case 20:
			if(m_nBeforeVertex == 21)m_pFieldVertex->SetVertexStop(false, 15);
			else if (m_nBeforeVertex == 15)m_pFieldVertex->SetVertexStop(false, 21);
			m_nBeforeVertex = 20;
			break;
		case 15:
			if(m_nBeforeVertex == 20)m_pFieldVertex->SetVertexStop(false, 10);
			else if (m_nBeforeVertex == 10)m_pFieldVertex->SetVertexStop(false, 20);
			m_nBeforeVertex = 15;
			break;
		case 10:
			if(m_nBeforeVertex == 15)m_pFieldVertex->SetVertexStop(false, 11);
			else if (m_nBeforeVertex == 11)m_pFieldVertex->SetVertexStop(false, 15);
			m_nBeforeVertex = 10;
			break;
		case 11:
			if(m_nBeforeVertex == 10)m_pFieldVertex->SetVertexStop(false, 12);
			else if (m_nBeforeVertex == 12) { m_pFieldVertex->SetVertexStop(false, 10); m_pFieldVertex->SetVertexStop(true, 17); }
			m_nBeforeVertex = 11;
			break;
		case 12:
			if (m_nBeforeVertex == 11 || m_nBeforeVertex == 17)
			{
				m_fTime = 0.0f;
				NextPage();
				m_pPlayer->TimerSetValue(3.0f);
			}
			else
			{
				m_nBeforeVertex = 12;
			}
		default:
			break;
		}
		break;

	default:
		break;
	}

	m_fTime += 1.0f / fFPS;
}

void CSceneTutorial::UpdateSection4()
{
	m_pFieldVertex->SetFeverInclease(false);
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(false);
		m_pBattle->Update();
		
		m_pPlayer->TutorialTimerUpdate();
		
		if (m_fTime >= 3.0f)
		{
			m_bSpownEffectDraw = true;
			if (m_fTime > 6.0f)
			{
				m_pBattle->CreateAlly();
				NextPage();
				m_fTime = 0.0f;
			}
		}
		break;
	case 1:
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(true);
		m_pBattle->Update();
		//m_pPlayer->Update();
		if (m_fTime >= 8.0f)
		{
			NextPage();
			m_fTime = 0.0f;
			m_pBattle->AllFighterClear();
			m_pPlayer->TimerSetMax();
		}
		else if (m_fTime >= 4.0f)
		{
			m_bSpownEffectDraw = false;
			m_pFieldVertex->InitFieldVertex();
		}
		break;
	default:
		break;
	}

	m_fTime += 1.0f / fFPS;
}

void CSceneTutorial::UpdateSection5()
{

}

void CSceneTutorial::UpdateSection6()
{

}

void CSceneTutorial::UpdateSection7()
{

}
