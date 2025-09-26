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

CSceneTutorial::CSceneTutorial()
	: m_bEnd(false)
	, m_eSection(TutorialSection::Section1)
	, m_pTextureArray{}, m_pBackGround(nullptr)
	, m_nCurrentPage(0)
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

	SAFE_DELETE(m_pBackGround);
}

void CSceneTutorial::Update()
{
	if (IsKeyTrigger(VK_ESCAPE) && !m_bEnd)
	{
		m_bEnd = true;
		SetNext(STAGE_SELECT);
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
