#pragma once
#include "Scene.h"
#include "Texture.h"
#include <array>

enum class TutorialSection
{
	Section1,
	Section2,
	Section3,
	Section4,
	Section5,
	Section6,
	Section7,

	Max
};

class CSceneTutorial : public CScene
{
public:
	CSceneTutorial();
	~CSceneTutorial();
	void Update()override;
	void Draw()override;

	void SetInstance();
	void NextPage();
	void BackPage();
private:
	bool m_bEnd;
	TutorialSection m_eSection;
	int m_nCurrentPage;
	std::array<std::vector<Texture*>, (int)TutorialSection::Max> m_pTextureArray;
	SpriteParam m_tTextParam;
	Texture* m_pBackGround;
	SpriteParam m_tBackParam;

};
