#pragma once
#include "Scene.h"
#include "Texture.h"

class CSceneTutorial : public CScene
{
public:
	CSceneTutorial();
	~CSceneTutorial();
	void Update()override;
	void Draw()override;
private:
	bool m_bEnd;
};
