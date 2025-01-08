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
	// --- èüîsä÷åWñ≥ÇµÇ…ïœÇÌÇÁÇ»Ç¢Ç‚Ç¬
	SpriteEx* m_pHitPoint;
	SpriteEx* m_pSelect[2];
	SpriteEx* m_pShadow;
	SpriteEx* m_pStageSelect[2];
	SpriteEx* m_pSummonData;
	SpriteEx* m_pUnderBar;
	SpriteEx* m_pBack;
	// --- ïœÇÌÇÈÇ‚Ç¬
	SpriteEx* m_pText;
	SpriteEx* m_pCharacter;
	SpriteEx* m_pLighting;
	SpriteEx* m_pTextShadow;
	SpriteEx* m_pClearTime;
	SpriteEx* m_pNextUI[2];
	SpriteEx* m_pStar;


	int nSlect; 


	static ResultGameInfo ResultGameData;
};