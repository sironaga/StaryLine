#include "SceneRanking.h"
#include "Option.h"
#include "InputEx.h"

enum class RankModePhase
{
	Naming = 0,
	Check,

	PhaseMax
};

CSceneRanking::CSceneRanking()
	: m_nPhase(0), m_pWrite(nullptr), m_bEnd(false)
	, m_pBackTex(nullptr)
{
	m_pWrite = new CWriteName();
	m_pBackTex = new SpriteEx(TEX_PASS("BackGround/RankingBack.png"));
}

CSceneRanking::~CSceneRanking()
{
	SAFE_DELETE(m_pWrite);	
}

void CSceneRanking::Update()
{
	if (!m_bEnd)
	{
		switch (m_nPhase)
		{
		case (int)RankModePhase::Naming:
			if (WithGetKeyTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B), VK_ESCAPE))
			{
				SetNext(SCENE_TITLE);
			}
			else
			{
				m_pWrite->Update();
			}

			if (WithGetKeyTriger(XINPUT_GAMEPAD_START, VK_TAB))
			{
				m_pWrite->SetPlayerName("");
				SetNext(SCENE_TITLE);
			}

			if (WithGetKeyTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A),VK_SPACE) || IsKeyTrigger(VK_RETURN))
			{
				if (m_pWrite->GetPlayerName() == "")MessageBox(NULL, "–¼‘O‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢", "Error", MB_OK);
				else
				{
					if (MessageBox(NULL, m_pWrite->GetPlayerName().c_str(), "Šm”F", MB_YESNO) == IDYES)
					{
						SetNext(SCENE_TITLE);
					}
				}
			}
			break;
		case (int)RankModePhase::Check:

			if (WithGetKeyTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B), VK_ESCAPE))
			{
				SetNext(SCENE_TITLE);
			}

			break;

		default:
			break;
		}
	}
}

void CSceneRanking::Draw()
{
	Sprite::ReSetSprite();
	m_pBackTex->SetPositon(DirectX::XMFLOAT3( SCREEN_CENTER.x,SCREEN_CENTER.y, 0.0f ));
	m_pBackTex->SetSize(DirectX::XMFLOAT3(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f));
	m_pBackTex->SetView(Get2DView());
	m_pBackTex->SetProjection(Get2DProj());
	m_pBackTex->SetTexture();
	m_pBackTex->Disp();
	Sprite::ReSetSprite();
	m_pWrite->Draw();
}