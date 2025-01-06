#include "SceneResult.h"
#include"Input.h"

ResultGameInfo CSceneResult::ResultGameData;



CSceneResult::CSceneResult()
{
	// --- テクスチャの読み込み
	
	// --- Default
	m_pHitPoint			= new SpriteEx("Assets/Texture/Result/Result_Helth.png");
	m_pStageSelect[0]	= new SpriteEx("Assets/Texture/Result/Result_Stageselect.png");
	m_pStageSelect[1]	= new SpriteEx("Assets/Texture/Result/Result_Stageselect_Push.png");
	m_pShadow			= new SpriteEx("Assets/Texture/Result/Result_Shadow.png");
	m_pSelect[0]		= new SpriteEx("Assets/Texture/Result/Result_Stageselect_Frame.png");
	m_pSelect[1]		= new SpriteEx("Assets/Texture/Result/Result_Nextstage_Frame.png");
	m_pSummonData		= new SpriteEx("Assets/Texture/Result/Result_Summon.png");
	m_pUnderBar			= new SpriteEx("Assets/Texture/Result/Result_Underbar.png");

	if (ResultGameData.bWin)
	{
		// --- WinData
		m_pText			= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_top.png");
		m_pCharacter	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Linie.png");
		m_pLighting		= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Light.png");
		m_pTextShadow	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_under.png");
		m_pClearTime	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Cleartime.png");
		m_pNextUI[0]	= new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
		m_pNextUI[1]	= new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
	}
	else
	{
		// ---- Lose
		m_pText			= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Text_top.png");
		m_pCharacter	= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Linie.png");
		m_pLighting		= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Light.png");
		m_pTextShadow	= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Text_under.png");
		m_pClearTime	= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Cleartime.png");
		m_pNextUI[0]	= new SpriteEx("Assets/Texture/Result/Lose/Result_Retry.png");
		m_pNextUI[1]	= new SpriteEx("Assets/Texture/Result/Lose/Result_Retry_Push.png");

	}

	// =====================================================================

	m_pClearTime->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pClearTime->SetSize(0.3f, 0.07f, 1.0f);
	m_pClearTime->SetPositon(1632.0f, 424.0f, 10.0f);

	m_pHitPoint->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pHitPoint->SetSize(0.3f, 0.07f, 1.0f);
	m_pHitPoint->SetPositon(1632.0f, 500.0f, 10.0f);


	m_pSummonData->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pSummonData->SetSize(0.3f, 0.14f, 1.0f);
	m_pSummonData->SetPositon(1632.0f, 613.5f, 10.0f);

	m_pCharacter->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pCharacter->SetSize(0.4f, 0.4f, 1.0f);
	m_pCharacter->SetPositon(940.0f, 600.0f, 10.0f);

	m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pLighting->SetSize(0.6f, 0.7, 1.0f);
	m_pLighting->SetPositon(940.0f, 550.0f, 10.0f);


	nSlect = 0;

}

CSceneResult::~CSceneResult()
{
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		if (m_pStageSelect[nLoop])
		{
			delete m_pStageSelect[nLoop];
			m_pStageSelect[nLoop] = nullptr;
		}
		if (m_pSelect[nLoop])
		{
			delete m_pSelect[nLoop];
			m_pSelect[nLoop] = nullptr;
		}
		if (m_pNextUI[nLoop])
		{
			delete m_pNextUI[nLoop];
			m_pNextUI[nLoop] = nullptr;
		}
	}

	if (m_pHitPoint)
	{
		delete m_pHitPoint;
		m_pHitPoint = nullptr;
	}
	if (m_pShadow)
	{
		delete m_pShadow;
		m_pShadow = nullptr;
	}
	if (m_pSummonData)
	{
		delete m_pSummonData;
		m_pSummonData = nullptr;
	}
	if (m_pUnderBar)
	{
		delete m_pUnderBar;
		m_pUnderBar = nullptr;
	}
	if (m_pText)
	{
		delete m_pText;
		m_pText = nullptr;
	}
	if (m_pCharacter)
	{
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}
	if (m_pLighting)
	{
		delete m_pLighting;
		m_pLighting = nullptr;
	}
	if (m_pTextShadow)
	{
		delete m_pTextShadow;
		m_pTextShadow = nullptr;
	}
	if (m_pClearTime)
	{
		delete m_pClearTime;
		m_pClearTime = nullptr;
	}
}

void CSceneResult::Update()
{



}

void CSceneResult::Draw()
{
	// --- 更新処理
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pStageSelect[nLoop]->SetProjection(Get2DProj());
		m_pStageSelect[nLoop]->SetView(Get2DView());
		m_pSelect[nLoop]->SetProjection(Get2DProj());
		m_pSelect[nLoop]->SetView(Get2DView());
		m_pNextUI[nLoop]->SetProjection(Get2DProj());
		m_pNextUI[nLoop]->SetView(Get2DView());
	}
	m_pHitPoint->SetProjection(Get2DProj());
	m_pHitPoint->SetView(Get2DView());
	m_pShadow->SetProjection(Get2DProj());
	m_pShadow->SetView(Get2DView());
	m_pSummonData->SetProjection(Get2DProj());
	m_pSummonData->SetView(Get2DView());
	m_pUnderBar->SetProjection(Get2DProj());
	m_pUnderBar->SetView(Get2DView());
	m_pText->SetProjection(Get2DProj());
	m_pText->SetView(Get2DView());
	m_pCharacter->SetProjection(Get2DProj());
	m_pCharacter->SetView(Get2DView());
	m_pLighting->SetProjection(Get2DProj());
	m_pLighting->SetView(Get2DView());
	m_pLighting->SetProjection(Get2DProj());
	m_pLighting->SetView(Get2DView());
	m_pTextShadow->SetProjection(Get2DProj());
	m_pTextShadow->SetView(Get2DView());
	m_pClearTime->SetProjection(Get2DProj());
	m_pClearTime->SetView(Get2DView());


	m_pLighting->SetTexture();
	m_pLighting->Disp();
	m_pCharacter->SetTexture();
	m_pCharacter->Disp();
	m_pHitPoint->SetTexture();
	m_pHitPoint->Disp();
	m_pClearTime->SetTexture();
	m_pClearTime->Disp();
	m_pSummonData->SetTexture();
	m_pSummonData->Disp();


}

void CSceneResult::InResultData(ResultGameInfo InData)
{
	ResultGameData = InData;
}
