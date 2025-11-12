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

constexpr int MAX_DUMMYLINE = 4;


CSceneTutorial::CSceneTutorial(StageType StageNum)
	: m_bEnd(false)
	, m_eSection(TutorialSection::Section1)
	, m_pTextureArray{}, m_pBackGround(nullptr),m_pExplanationScreen(nullptr)
	, m_bExplanationDraw(false)
	, m_nCurrentPage(0), m_fTime(0.0f)
	, m_nBeforeVertex(-1), m_bSpownEffectDraw(false)
	, m_nPassVertexCount(0), m_bFever(false), m_bOnButton(false), m_BattleFlag(false)
	, m_bIsClickMove(true)
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

	m_pExplanationScreen = new Texture();
	if (FAILED(m_pExplanationScreen->Create(TEX_PASS("Tutorial/ExplanationScreen.png"))))
		MessageBox(NULL, "Tutorial/ExplanationScreen.png", "LoadError", MB_OK);

	m_tBackParam = SpriteParam();
	m_tBackParam.size = DirectX::XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_tBackParam.world = Get2DWorld();
	m_tBackParam.view = Get2DView();
	m_tBackParam.proj = Get2DProj();
	
	m_tTextParam = SpriteParam();
	m_tTextParam.pos = DirectX::XMFLOAT2(20.0f, -460.0f);
	m_tTextParam.size = DirectX::XMFLOAT2(SCREEN_WIDTH - 950.0f, 80.0f);
	m_tTextParam.world = Get2DWorld();
	m_tTextParam.view = Get2DView();
	m_tTextParam.proj = Get2DProj();

	m_tExplanationParam = SpriteParam();
	m_tExplanationParam.pos = DirectX::XMFLOAT2(SCREEN_WIDTH, 100.0f);
	m_tExplanationParam.size = DirectX::XMFLOAT2(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.7f);
	m_tExplanationParam.world = Get2DWorld();
	m_tExplanationParam.view = Get2DView();
	m_tExplanationParam.proj = Get2DProj();

	CCharacterManager::GetInstance()->InitCharacterTexture(StageNum);//キャラクターテクスチャ～の初期化
	
	m_pBackGround2 = new CBackGround();
	m_pFieldVertex = new CFieldVertex();
	m_pBattle = new CBattle(StageNum);
	m_pPlayer = new CPlayer();
	m_pField = new Field(StageNum);
	m_pStarLine = new StarLine();


	m_tPenButtonParam.pos = DirectX::XMFLOAT2(800.0f, -460.0f);
	m_tPenButtonParam.size = DirectX::XMFLOAT2(100.0f, 100.0f);
	m_tPenButtonParam.world = Get2DWorld();
	m_tPenButtonParam.view = Get2DView();
	m_tPenButtonParam.proj = Get2DProj();
	m_pPenButtonTexture = new Texture();
	m_pPenButtonTexture->Create(TEX_PASS("Tutorial/Pen.png"));

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

	for (int i = 0; i < MAX_DUMMYLINE; i++)
	{
		m_bDrawDummyLine[i] = false;
		m_pDummyLineTexture[i] = new Texture();
		std::string path = "Assets/Texture/Tutorial/DummyLine/Line"+ std::to_string(i + 1) + ".png";
		if (FAILED(m_pDummyLineTexture[i]->Create(path.c_str())))
			MessageBox(NULL, path.c_str(), "LoadError", MB_OK);
		m_tDummyLineParam[i] = SpriteParam();
		m_tDummyLineParam[i].world = Get2DWorld();
		m_tDummyLineParam[i].view = Get2DView();
		m_tDummyLineParam[i].proj = Get2DProj();
	}
	m_tDummyLineParam[0].pos = DirectX::XMFLOAT2(-120.0f, 170.0f);
	m_tDummyLineParam[0].size = DirectX::XMFLOAT2(250.0f, 500.0f);
	m_tDummyLineParam[1].pos = DirectX::XMFLOAT2(125.0f, 290.0f);
	m_tDummyLineParam[1].size = DirectX::XMFLOAT2(260.0f, 260.0f);
	m_tDummyLineParam[2].pos = DirectX::XMFLOAT2(-120.0f, 35.0f);
	m_tDummyLineParam[2].size = DirectX::XMFLOAT2(250.0f, 250.0f);
	m_tDummyLineParam[3].pos = DirectX::XMFLOAT2(60.0f, 100.0f);
	m_tDummyLineParam[3].size = DirectX::XMFLOAT2(125.0f, 125.0f);

	m_TutorialSoundSE = new CSoundList(SE_TUTORIAL);

	m_TutorialSoundSE->SetMasterVolume();
	if(m_TutorialSoundSE)m_pSourseTutorialSE = m_TutorialSoundSE->GetSound(false);
	
	m_TutorialSoundBGM = new CSoundList(BGM_TUTORIAL);
	
	m_TutorialSoundBGM->SetMasterVolume();
	if (m_TutorialSoundBGM)m_pSourseTutorialBGM = m_TutorialSoundBGM->GetSound(true);

	if (m_pSourseTutorialBGM)SetVolumeBGM(m_pSourseTutorialBGM);
	m_pSourseTutorialBGM->Start();
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
	SAFE_DELETE(m_pPenButtonTexture);
	if (m_pSourseTutorialSE)
	{
		m_pSourseTutorialSE->Stop();
		m_pSourseTutorialSE = nullptr;
	}
	SAFE_DELETE(m_TutorialSoundSE);
	if (m_pSourseTutorialBGM)
	{
		m_pSourseTutorialBGM->Stop();
		m_pSourseTutorialBGM = nullptr;
	}
	SAFE_DELETE(m_TutorialSoundBGM);

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
	if (m_bIsClickMove) m_fButtonMoveTime += 1.0f / 60.0f;
	else m_fButtonMoveTime = 0.0f;

	m_tPenButtonParam.pos.y = -460.0f + 10.0f * sinf(DirectX::XMConvertToRadians(m_fButtonMoveTime * 180.0f));
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
	if (IsKeyTrigger(VK_UP))
	{
		int section = (int)m_eSection;
		section--;
		m_eSection = (TutorialSection)section;
		m_nCurrentPage = 0;
	}
#endif // _DEBUG
}

void CSceneTutorial::Draw()
{
	m_pBackGround2->Draw();
	m_pField->Draw();
	m_pFieldVertex->ShapesDraw();
	if (m_bSpownEffectDraw)m_pFieldVertex->ShapesEffectDraw();
	
	for (int i = 0; i < MAX_DUMMYLINE; i++)
	{
		if (m_bDrawDummyLine[i])
		{
			SetRender2D();
			Sprite::ReSetSprite();
			Sprite::SetParam(m_tDummyLineParam[i]);
			Sprite::SetTexture(m_pDummyLineTexture[i]);
			Sprite::Draw();
		}
	}

	m_pFieldVertex->DrawTutorial();
	m_pPlayer->Draw();
	if (!m_bIsClickMove)m_pPlayer->DrawMoveRenderers();

	std::thread Th_BattleDraw([this]() { this->m_pBattle->Draw(); });
	Th_BattleDraw.join();

	//if (!m_bSpownEffectDraw)
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
	if (m_bExplanationDraw)
	{
		SetRender2D();
		Sprite::ReSetSprite();
		Sprite::SetParam(m_tExplanationParam);
		Sprite::SetTexture(m_pExplanationScreen);
		Sprite::Draw();
	}

	if (m_bIsClickMove)
	{
		SetRender2D();
		Sprite::ReSetSprite();
		Sprite::SetParam(m_tPenButtonParam);
		Sprite::SetTexture(m_pPenButtonTexture);
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
	m_pFieldVertex->ClearSuperStar();
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_bIsClickMove = true;
		m_bDrawDummyLine[0] = true;
		m_pPlayer->SetNowVertex(12);
		m_pPlayer->SetPos(m_pFieldVertex->GetVertexPos(12));
		for (int i = 0; i < MAX_VERTEX; i++)
		{
			m_pFieldVertex->SetVertexStop(true, i);
		}
		m_pFieldVertex->SetVertexStop(false, 11);
		if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)))
		{
			//音の再生
			XAUDIO2_BUFFER buffer = m_TutorialSoundSE->GetBuffer(false);
			m_pSourseTutorialSE->FlushSourceBuffers();
			m_pSourseTutorialSE->SubmitSourceBuffer(&buffer);
			if (m_pSourseTutorialSE)SetVolumeSE(m_pSourseTutorialSE);
			m_pSourseTutorialSE->Start();

			NextPage();
			m_fTime = 0.0f;
		}
		break;
	case 1:
		m_bIsClickMove = false;
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
		m_bIsClickMove = true;
		if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)))
		{
			//音の再生
			XAUDIO2_BUFFER buffer = m_TutorialSoundSE->GetBuffer(false);
			m_pSourseTutorialSE->FlushSourceBuffers();
			m_pSourseTutorialSE->SubmitSourceBuffer(&buffer);
			if (m_pSourseTutorialSE)SetVolumeSE(m_pSourseTutorialSE);
			m_pSourseTutorialSE->Start();

			m_bDrawDummyLine[0] = false;
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
	m_pFieldVertex->ClearSuperStar();
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_bIsClickMove = true;
		m_bDrawDummyLine[1] = true;
		m_pPlayer->SetNowVertex(12);
		m_pPlayer->SetPos(m_pFieldVertex->GetVertexPos(12));
		m_pFieldVertex->InitFieldVertex();

		for (int i = 0; i < MAX_VERTEX; i++)
		{
			m_pFieldVertex->SetVertexStop(true, i);
		}
		m_pFieldVertex->SetVertexStop(false, 13);
		m_pFieldVertex->SetVertexStop(false, 7);
		if (IsKeyTrigger(VK_SPACE))
		{
			//音の再生
			XAUDIO2_BUFFER buffer = m_TutorialSoundSE->GetBuffer(false);
			m_pSourseTutorialSE->FlushSourceBuffers();
			m_pSourseTutorialSE->SubmitSourceBuffer(&buffer);
			if (m_pSourseTutorialSE)SetVolumeSE(m_pSourseTutorialSE);
			m_pSourseTutorialSE->Start();
			NextPage();
			m_fTime = 0.0f;
		}
		break;
	case 1:
		m_bIsClickMove = false;
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
				m_bIsClickMove = true;
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
	m_pFieldVertex->ClearSuperStar();
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_bIsClickMove = true;
		m_bDrawDummyLine[2] = true;
		m_pPlayer->TimerSetMax();
		m_pFieldVertex->SetVertexStop(false, 17);
		m_pFieldVertex->SetVertexStop(false, 11);
		if (IsKeyTrigger(VK_SPACE))
		{
			//音の再生
			XAUDIO2_BUFFER buffer = m_TutorialSoundSE->GetBuffer(false);
			m_pSourseTutorialSE->FlushSourceBuffers();
			m_pSourseTutorialSE->SubmitSourceBuffer(&buffer);
			if (m_pSourseTutorialSE)SetVolumeSE(m_pSourseTutorialSE);
			m_pSourseTutorialSE->Start();
			NextPage();
			m_fTime = 0.0f;
		}
		break;
	case 1:
		m_bIsClickMove = false;
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
				m_pPlayer->SetSoundStop();
				NextPage();
				m_pPlayer->TimerSetValue(3.0f);
				m_bDrawDummyLine[1] = false;
				m_bDrawDummyLine[2] = false;
				m_bIsClickMove = true;
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
	m_pFieldVertex->ClearSuperStar();
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_bIsClickMove = false;
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(true);
		m_pBattle->Update();
		
		//　タイマーを減らす更新処理
		m_pPlayer->TutorialTimerUpdate();
		
		// 決定キーで次のページへ
		if (IsKeyTrigger(VK_SPACE))
		{
			//音の再生
			XAUDIO2_BUFFER buffer = m_TutorialSoundSE->GetBuffer(false);
			m_pSourseTutorialSE->FlushSourceBuffers();
			m_pSourseTutorialSE->SubmitSourceBuffer(&buffer);
			if (m_pSourseTutorialSE)SetVolumeSE(m_pSourseTutorialSE);
			m_pSourseTutorialSE->Start();
			NextPage();
			m_fTime = 0.0f;
		}
		break;
	case 1:
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(false);
		m_pBattle->Update();

		if (m_fTime >= 11.0f)
		{
			// 最後のページなので次のセクションへ
			NextPage();
			// エフェクトを消す
			m_bSpownEffectDraw = false;
			m_pFieldVertex->InitFieldVertex();
			m_fTime = 0.0f;
			m_pBattle->AllFighterClear();
			m_pPlayer->TimerSetMax();
			m_bExplanationDraw = true;
		}
		else if (m_fTime >= 6.0f)
		{
			// 3秒時に味方を生成
			m_pBattle->CreateAlly();
		}
		else if (m_fTime >= 3.0f)
		{
			// 0~3秒からエフェクトを描画
			m_bSpownEffectDraw = true;
		}
		break;
	default:
		break;
	}

	m_fTime += 1.0f / fFPS;
}

void CSceneTutorial::UpdateSection5()
{
	m_tExplanationParam.pos.x -= 30.0f;
	if (m_tExplanationParam.pos.x < 0)m_tExplanationParam.pos.x = 0.0f;	
	// 決定キーで次のページへ
	if (IsKeyTrigger(VK_SPACE))
	{
		//音の再生
		XAUDIO2_BUFFER buffer = m_TutorialSoundSE->GetBuffer(false);
		m_pSourseTutorialSE->FlushSourceBuffers();
		m_pSourseTutorialSE->SubmitSourceBuffer(&buffer);
		if (m_pSourseTutorialSE)SetVolumeSE(m_pSourseTutorialSE);
		m_pSourseTutorialSE->Start();

		m_bExplanationDraw = false;
		NextPage();

		m_pFieldVertex->InitFieldVertex();

		for (int i = 0; i < MAX_VERTEX; i++)
		{
			m_pFieldVertex->SetVertexStop(true, i);
		}
		m_pFieldVertex->SetVertexStop(false, 13);
		m_pFieldVertex->SetVertexStop(false, 17);
		m_pFieldVertex->SetVertexStop(false, 18);
		m_bDrawDummyLine[3] = true;

		m_pPlayer->TimerSetMax();
		m_pPlayer->SetNowVertex(12);
		m_pPlayer->SetPos(m_pFieldVertex->GetVertexPos(12));
		m_fTime = 0.0f;
		m_nBeforeVertex = -1;
		m_nPassVertexCount = 0;
		m_bSpownEffectDraw = false;
	}
}

void CSceneTutorial::UpdateSection6()
{
	m_pFieldVertex->SetFeverInclease(false);
	m_pFieldVertex->ClearSuperStar();
	m_pFieldVertex->Update();

	switch (m_nCurrentPage)
	{
	case 0:
		m_bIsClickMove = false;
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(true);
		m_pBattle->Update();
		m_pPlayer->Update();
		m_pPlayer->TimerReCharge();

		m_pFieldVertex->SetVertexStop(true, m_pPlayer->GetNowVertex());
		switch (m_pPlayer->GetNowVertex())
		{
		case 12:
			if (m_nBeforeVertex == 13)
			{
				m_pFieldVertex->SetVertexStop(false, 17);
				m_pFieldVertex->SetVertexStop(false, 18);
				m_nPassVertexCount++;
			}
			else if (m_nBeforeVertex == 17)
			{
				m_pFieldVertex->SetVertexStop(false, 13);
				m_pFieldVertex->SetVertexStop(false, 18);
				m_nPassVertexCount++;
			}
			else if (m_nBeforeVertex == 18)
			{
				m_pFieldVertex->SetVertexStop(false, 13);
				m_pFieldVertex->SetVertexStop(false, 17);
				m_nPassVertexCount++;
			}
			m_nBeforeVertex = 12;
			break;
		case 13:
			if (m_nBeforeVertex == 12)
			{
				m_pFieldVertex->SetVertexStop(false, 18);
				m_pFieldVertex->SetVertexStop(true, 17);
				m_nPassVertexCount++;
			}
			else if (m_nBeforeVertex == 18)
			{
				m_pFieldVertex->SetVertexStop(false, 12);
				m_pFieldVertex->SetVertexStop(true, 13);
				m_pFieldVertex->SetVertexStop(true, 17);
				m_nPassVertexCount++;
			}
			m_nBeforeVertex = 13;
			break;
		case 17:
			if (m_nBeforeVertex == 12)
			{
				m_pFieldVertex->SetVertexStop(false, 18);
				m_pFieldVertex->SetVertexStop(true, 13);
				m_nPassVertexCount++;
			}
			else if (m_nBeforeVertex == 18)
			{
				m_pFieldVertex->SetVertexStop(false, 12);
				m_pFieldVertex->SetVertexStop(true, 13);
				m_pFieldVertex->SetVertexStop(true, 17);
				m_nPassVertexCount++;
			}
			m_nBeforeVertex = 17;
			break;
		case 18:
			if (m_nPassVertexCount == 5)
			{
				m_bDrawDummyLine[3] = false;
				m_fTime = 0.0f;
				NextPage();
				m_nPassVertexCount = 0;
			}
			else
			{
				if (m_nBeforeVertex == 12)
				{
					m_pFieldVertex->SetVertexStop(false, 13);
					m_pFieldVertex->SetVertexStop(false, 17);
					m_nPassVertexCount++;
				}
				else if (m_nBeforeVertex == 13)
				{
					m_pFieldVertex->SetVertexStop(false, 12);
					m_pFieldVertex->SetVertexStop(false, 17);
					m_nPassVertexCount++;
				}
				else if (m_nBeforeVertex == 17)
				{
					m_pFieldVertex->SetVertexStop(false, 12);
					m_pFieldVertex->SetVertexStop(false, 13);
					m_nPassVertexCount++;
				}
				m_nBeforeVertex = 18;
			}
			break;
		}
		break;
	case 1:
		m_bIsClickMove = true;
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(false);
		m_pBattle->Update();
		//　タイマーを減らす更新処理
		m_pPlayer->TutorialTimerUpdate();

		if (m_fTime >= 11.0f)
		{
			m_bSpownEffectDraw = false;
			if (IsKeyTrigger(VK_SPACE))
			{
				//音の再生
				XAUDIO2_BUFFER buffer = m_TutorialSoundSE->GetBuffer(false);
				m_pSourseTutorialSE->FlushSourceBuffers();
				m_pSourseTutorialSE->SubmitSourceBuffer(&buffer);
				if (m_pSourseTutorialSE)SetVolumeSE(m_pSourseTutorialSE);
				m_pSourseTutorialSE->Start();

				// 最後のページなので次のセクションへ
				NextPage();
				// エフェクトを消す
				m_pFieldVertex->InitFieldVertex();
				m_fTime = 0.0f;
				m_pBattle->AllFighterClear();

				// 次のセクション用の初期化処理
				// あらかじめ味方を生成しておく
				for (int i = 0; i < 15; i++)
				{
					// 3と4の交互に配置
					int nCorner = i % 2 ? 3 : 4;

					// 味方データを保存
					m_pBattle->SaveAllyData(nCorner);
				}
				m_pBattle->CreateAlly();
				m_pBattle->SetTutorialMoveFlag(true);
			}
		}
		else if (m_fTime >= 6.0f)
		{
			// 3秒時に味方を生成
			m_pBattle->CreateAlly();
		}
		else if (m_fTime >= 3.0f)
		{
			// 0~3秒からエフェクトを描画
			m_bSpownEffectDraw = true;
		}
		break;
	default:
		break;
	}

	m_fTime += 1.0f / fFPS;
}

void CSceneTutorial::UpdateSection7()
{
	m_pFieldVertex->Update();
	m_pPlayer->Update();
	m_pBattle->Update();
	if(!m_bSpownEffectDraw)m_pPlayer->TimerReCharge();
	

	if (IsKeyTrigger(VK_SPACE) && !m_bOnButton)
	{
		m_pFieldVertex->AddFeverPoint();
		m_bOnButton = true;
		for (int i = 0; i < MAX_VERTEX; i++)
		{
			m_pFieldVertex->SetVertexStop(false, i);
		}
		m_fTime = 0.0f;
	}
	if (m_pFieldVertex->GetFever() && !m_bFever)
	{
		m_bIsClickMove = false;
		m_bFever = true;
		NextPage();
		m_pStarLine->SetLineMode(1);
		m_pFieldVertex->InitFieldVertex();	// 次の作図に必用な初期化処理	
		m_pFieldVertex->SetFeverPoint();
	}
	if (m_bFever)
	{
		m_pFieldVertex->SubtractFeverPoint();
		if (!m_pPlayer->TutorialBoolMove())m_pFieldVertex->SetZeroFeverPoint();
		if (m_pFieldVertex->GetFeverPoint() == 0)
		{			
			m_pBattle->SetTutorialMoveFlag(false);
			m_pBattle->SetTutorialSpownFlag(false);
			m_bFever = false;
		}
	}
	else
	{
		if (m_pFieldVertex->GetFeverPoint() == 0 && m_bOnButton && !m_BattleFlag)
		{
			m_pPlayer->Reset();
			//m_pFieldVertex->InitFieldVertex();
			m_bSpownEffectDraw = true;
			NextPage();
			m_bIsClickMove = true;
		}
	}
	if (m_bSpownEffectDraw || m_BattleFlag)
	{
		m_fTime += 1.0f / fFPS;
	}
	if (m_fTime >= 7.5f  && !m_BattleFlag)
	{
		m_pBattle->SetTutorialMoveFlag(true);
		m_pBattle->SetTutorialSpownFlag(true);
		m_pPlayer->SetMoveStop();
		m_BattleFlag = true;
		m_bSpownEffectDraw = false;
		m_pBattle->AllTutorialFlagClear();
		SetNext(STAGE_SELECT);
		m_bEnd = true;

	}
	else if (m_fTime >= 3.0f)
	{
		// 3秒時に味方を生成
		m_pBattle->CreateAlly();
		m_pFieldVertex->InitFieldVertex();
	}	

	if (m_fTime >= 12.0f && !m_bEnd)
	{
	}
}
