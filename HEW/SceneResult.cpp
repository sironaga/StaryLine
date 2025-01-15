#include "SceneResult.h"
#include"Input.h"
#include"Controller.h"
#include"InputEx.h"


ResultGameInfo CSceneResult::ResultGameData;
StageType CSceneResult::StageLevel;


CSceneResult::CSceneResult()
	:nAnimationFrame(0)
{
	// デバッグ
	ResultGameData.bWin = 1;

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
	m_pBack				= new SpriteEx("Assets/Texture/Result/B.png");


	if (ResultGameData.bWin)
	{
		// --- WinData
		m_pText			= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_top.png");
		m_pCharacter	= new SpriteEx("Assets/Texture/Result/Win/Result_Linie_Win_Sprite.png");
		m_pLighting		= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Light.png");
		m_pTextShadow	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_under.png");
		m_pClearTime	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Cleartime.png");
		m_pNextUI[0]	= new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
		m_pNextUI[1]	= new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
		m_pStar			= new SpriteEx("Assets/Texture/Star/star.png");
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
		m_pStar			= new SpriteEx("Assets/Texture/Star/star2.png");
	}

	// =====================================================================
	
	// --- デフォルト
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pStageSelect[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pStageSelect[nLoop]->SetSize(0.3f, 0.07f, 1.0f);
		m_pStageSelect[nLoop]->SetPositon(285.0f, 930.0f, 10.0f);

		m_pNextUI[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pNextUI[nLoop]->SetSize(0.3f, 0.07f, 1.0f);
		m_pNextUI[nLoop]->SetPositon(1650.0f, 930.0f, 10.0f);
	}

	m_pSelect[0]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pSelect[0]->SetSize(0.315f, 0.09f, 1.0f);
	m_pSelect[0]->SetPositon(285.0f, 930.0f, 10.0f);
	m_pSelect[1]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pSelect[1]->SetSize(0.315f, 0.09f, 1.0f);
	m_pSelect[1]->SetPositon(1650.0f, 930.0f, 10.0f);

	m_pBack->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pBack->SetSize(1.0f, 1.0f, 1.0f);
	m_pBack->SetPositon(960.0f, 540.0f, 10.0f);

	m_pUnderBar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pUnderBar->SetSize(1.0f, 0.05f, 1.0f);
	m_pUnderBar->SetPositon(960.0f, 1020.0f, 10.0f);

	m_pClearTime->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pClearTime->SetSize(0.3f, 0.07f, 1.0f);
	m_pClearTime->SetPositon(1632.0f, 304.0f, 10.0f);

	m_pHitPoint->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pHitPoint->SetSize(0.3f, 0.07f, 1.0f);
	m_pHitPoint->SetPositon(1632.0f, 380.0f, 10.0f);

	m_pSummonData->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pSummonData->SetSize(0.3f, 0.14f, 1.0f);
	m_pSummonData->SetPositon(1632.0f, 493.5f, 10.0f);

	// ----
	if (ResultGameData.bWin)
	{
		m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pText->SetSize(0.4f, 0.12f, 1.0f);
		m_pText->SetPositon(960.0f, 150.0f, 10.0f);

		m_pTextShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pTextShadow->SetSize(0.5f, 0.05f, 1.0f);
		m_pTextShadow->SetPositon(960.0f, 220.0f, 10.0f);

		m_pCharacter->SetUvSize((1.0f / 8.0f), (1.0f / 8.0f));
		m_pCharacter->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacter->SetSize(0.50f, 0.8f, 1.0f);
		m_pCharacter->SetPositon(1100.0f, 420.0f, 10.0f);

		m_pShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pShadow->SetSize(0.25f, 0.05f, 1.0f);
		m_pShadow->SetPositon(960.0f, 840.0f, 10.0f);

		m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pLighting->SetSize(0.50f, 0.9f, 1.0f);
		m_pLighting->SetPositon(940.0f, 470.0f, 10.0f);
	}
	else
	{
		m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(190.0f));
		m_pText->SetSize(0.4f, 0.12f, 1.0f);
		m_pText->SetPositon(960.0f, 150.0f, 10.0f);

		m_pTextShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pTextShadow->SetSize(0.4f, 0.05f, 1.0f);
		m_pTextShadow->SetPositon(960.0f, 220.0f, 10.0f);

		m_pCharacter->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacter->SetSize(0.4f, 0.4f, 1.0f);
		m_pCharacter->SetPositon(940.0f, 620.0f, 10.0f);

		m_pShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pShadow->SetSize(0.35f, 0.1f, 1.0f);
		m_pShadow->SetPositon(900.0f, 780.0f, 10.0f);

		m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pLighting->SetSize(0.55f, 0.6, 1.0f);
		m_pLighting->SetPositon(940.0f, 570.0f, 10.0f);
	}

	// 選択初期化
	nSlect = 0;
	// Animationタイマー
	nAnimationTimer = timeGetTime();

	// 数字の描画
	m_pNumber = new CNumberUI();
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
	// -- 入力処理
	if (WihtGetKeyPress(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT))
	{
		nSlect = 0;
		// SE
	}
	if (WihtGetKeyPress(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT))
	{
		nSlect = 1;
		// SE
	}
	// ----

	// -- 分岐処理
	if (nSlect == 0)
	{
		// StageSelect
		if(WithGetKeyTriger(XINPUT_GAMEPAD_A,VK_RETURN))
		{
			SetNext(STAGE_SELECT);
		}
	}
	else
	{
		// retry or Next
		if (ResultGameData.bWin)
		{
			



			// Next
			if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN))
			{
				SetNext(SCENE_GAME, StageLevel);
			}

		}
		else
		{
			// Retry
			if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN))
			{
				SetNext(SCENE_GAME, StageLevel);
			}
		}
	}
	// ----
	if (timeGetTime() - nAnimationTimer >= 20.0f)
	{
		nAnimationFrame++;
		nAnimationTimer = timeGetTime();
	}
	m_pNumber->SetNumber(1);

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
	m_pHitPoint->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
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
	m_pBack->SetProjection(Get2DProj());
	m_pBack->SetView(Get2DView());
	m_pStar->SetProjection(Get2DProj());
	m_pStar->SetView(Get2DView());
	// -- 描画

	m_pBack->SetTexture();
	m_pBack->Disp();
	m_pLighting->SetTexture();
	m_pLighting->Disp();
	m_pShadow->SetTexture();
	m_pShadow->Disp();


	if (ResultGameData.bWin)
	{
		// Animation計算
		float UvSize;
		UvSize = (1.0f / 8.0f);
		int nUvMovePosX, nUvMovePosY;
		if (nAnimationFrame > 63)
		{
			nAnimationFrame = 63;
		}
		nUvMovePosX = nAnimationFrame % 8;
		nUvMovePosY = nAnimationFrame / 8;

		float fUvPosX, fUvPosY;

		fUvPosX = UvSize * (float)nUvMovePosX;
		fUvPosY = UvSize * (float)nUvMovePosY;


		m_pCharacter->SetUvSize((1.0f / 8.0f), (1.0f / 8.0f));
		m_pCharacter->SetUvPos(fUvPosX, fUvPosY);
		m_pCharacter->SetTexture();
		m_pCharacter->Disp();
		m_pCharacter->SetUvSize(1.0f, 1.0f);
		m_pCharacter->SetUvPos(0.0f, 0.0f);
	}
	else
	{
		m_pCharacter->SetTexture();
		m_pCharacter->Disp();
	}
		m_pHitPoint->SetTexture();
		m_pHitPoint->Disp();
		m_pClearTime->SetTexture();
		m_pClearTime->Disp();
		m_pSummonData->SetTexture();
		m_pSummonData->Disp();
		m_pUnderBar->SetTexture();
		m_pUnderBar->Disp();
		m_pTextShadow->SetTexture();
		m_pTextShadow->Disp();
		m_pText->SetTexture();
		m_pText->Disp();
		m_pStageSelect[0]->SetTexture();
		m_pStageSelect[0]->Disp();
		m_pNextUI[0]->SetTexture();
		m_pNextUI[0]->Disp();
		m_pSelect[nSlect]->SetTexture();
		m_pSelect[nSlect]->Disp();

		if (ResultGameData.bWin)
		{
			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(200.0f, 140.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();



			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(400.0f, 340.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();

			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(1500.0f, 100.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();


			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(1700.0f, 140.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();

		}
		else
		{
			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(225.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(400.0f, 740.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();

			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(475.0f, 730.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();

			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(1350.0f, 730.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();

			m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(225.0f));
			m_pStar->SetSize(0.05f, 0.1f, 1.0f);
			m_pStar->SetPositon(1500.0f, 740.0f, 10.0f);
			m_pStar->SetTexture();
			m_pStar->Disp();
		}
		
		m_pNumber->SetPos({920.0f,540.0f,0.0f});
		m_pNumber->SetScale({ 0.1f,0.1f,1.0f });
		m_pNumber->Draw();
}


void CSceneResult::InResultData(ResultGameInfo InData)
{
	ResultGameData = InData;
}

void CSceneResult::InStageLevel(StageType  nInLevel)
{
	StageLevel = nInLevel;
}
