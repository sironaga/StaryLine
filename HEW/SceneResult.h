#pragma once

#include "Scene.h"
#include"_StructList.h"
#include"SpriteEx.h"
#include"Main.h"

class CSceneResult : public CScene
{
public:
	CSceneResult();
	virtual ~CSceneResult();
	virtual void Update()override;
	virtual void Draw()override;
	static void InResultData(ResultGameInfo InData);
private:
	SpriteEx* m_pClearTime;
	SpriteEx* m_pHitPoint;
	SpriteEx* m_pSummonData;

	SpriteEx* m_pNextStage;
	SpriteEx* m_pSelectStage;
	SpriteEx* m_pUI;

	SpriteEx* m_pText;
	SpriteEx* m_pCharacter;
	SpriteEx* m_pLighting;


	static ResultGameInfo ResultGameData;
};