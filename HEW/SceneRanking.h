#pragma once

#include "Scene.h"
#include "WriteName.h"
#include "SpriteEx.h"

class CSceneRanking : public CScene
{
public:
	CSceneRanking();
	virtual ~CSceneRanking();
	virtual void Update() override;
	virtual void Draw() override;
private:
	int m_nPhase;
	CWriteName* m_pWrite;
	bool m_bEnd;
	SpriteEx* m_pBackTex;
};