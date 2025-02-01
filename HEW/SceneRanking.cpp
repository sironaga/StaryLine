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
{
	m_pWrite = new CWriteName();
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
			if (WithGetKeyTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_B), IsKeyTrigger(VK_ESCAPE)))
			{
				SetNext(SCENE_TITLE);
			}
			else
			{
				m_pWrite->Update();
			}

			if (WithGetKeyTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A), IsKeyTrigger(VK_SPACE)) || IsKeyTrigger(VK_RETURN))
			{
				if (m_pWrite->GetPlayerName() == "")MessageBox(NULL, "–¼‘O‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢", "Error", MB_OK);
				else
				{
					if (MessageBox(NULL, m_pWrite->GetPlayerName().c_str(), "Šm”F", MB_YESNO) == IDYES)
					{
						m_nPhase = (int)RankModePhase::Check;
					}
				}
			}
			break;
		case (int)RankModePhase::Check:

			break;

		default:
			break;
		}
	}
}

void CSceneRanking::Draw()
{
	m_pWrite->Draw();
}