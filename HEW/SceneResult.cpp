// --- Include
#include "SceneResult.h"
#include"Input.h"
#include"InputEx.h"
#include"Controller.h"
#include"InputEx.h"
#include"Easing.h"

// 変数きれいにする！！！！！！！！！！！！！！！！！


// --- Global
ResultGameInfo CSceneResult::ResultGameData;
StageType CSceneResult::StageLevel;
int CSceneResult::nBestScore[8];

CSceneResult::CSceneResult()
	:nSelect(0),nAnimationFrame(0),nCAnimation(0),bCAnimation(true),bAnimation(false),bScore(true),bBestScore(false)
{
	// デバッグ
	ResultGameData.bWin = 1;

	// --- テクスチャの読み込み
	LoadTexture();
	
	// ---　場所の設定
	DefaultSetPos();

	// 変数初期化
	CPosY = 200.0f;
	CScle = 0;
	nCount = 0;
	StarPosY = 1000.0f;
	LogoAngle = 0.0f;
	fTime = 0.0f;
	fStarAngle = 120.0f;
	fCTime = 0.0f;
	// Animationタイマー
	nAnimationTimer = timeGetTime();
	if (ResultGameData.bWin)
	{
		m_pResultSound = new CSoundList(BGM_GAMECLEAR);
		m_pResultBGM = m_pResultSound->GetSound(true);
		m_pResultBGM->Start();
	}
	else
	{
		m_pResultSound = new CSoundList(BGM_GAMEOVER);
		m_pResultBGM = m_pResultSound->GetSound(true);
		m_pResultBGM->Start();
	}
	m_pResultSelectSound    = new CSoundList(SE_SELECT);
	m_pResultSelectSE = m_pResultSelectSound->GetSound(false);

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
	if (m_pNumber)
	{
		delete m_pNumber;
		m_pNumber = nullptr;
	}
	if (m_pResultBGM)
	{
		m_pResultBGM->Stop();
		m_pResultBGM = nullptr;
	}
	if (m_pResultSound)
	{
		SAFE_DELETE(m_pResultSound);
	}
	if (m_pResultSelectSE)
	{
		m_pResultSelectSE->Stop();
		m_pResultSelectSE = nullptr;
	}
	if (m_pResultSelectSound)
	{
		SAFE_DELETE(m_pResultSelectSound);
	}
	
}

void CSceneResult::Update()
{
	//--- 入力処理
	KeyProsess();

	// --- AnimationProcess
	AnimationManagemer();

	// --- カメラ情報の更新
	SetCamData();


}

void CSceneResult::Draw()
{

	if (ResultGameData.bWin)
	{
		WinAnimation();
	}
	else
	{
		LoseAnimation();
	}

	NumberDisp();
	
}

// --- セッター　ゲッター
void CSceneResult::InResultData(ResultGameInfo InData)
{
	ResultGameData = InData;
}

void CSceneResult::InStageLevel(StageType  nInLevel)
{
	StageLevel = nInLevel;
}

void CSceneResult::InBestScore(int InScore, StageType InStage)
{
	nBestScore[InStage.StageSubNumber] = InScore;
}

int CSceneResult::OutBestScore(StageType InStage)
{
	return nBestScore[InStage.StageSubNumber];
}





// --- テクスチャの読み込み
void CSceneResult::LoadTexture(void)
{
	m_pHitPoint = new SpriteEx("Assets/Texture/Result/Result_Helth.png");
	m_pStageSelect[0] = new SpriteEx("Assets/Texture/Result/Result_Stageselect.png");
	m_pStageSelect[1] = new SpriteEx("Assets/Texture/Result/Result_Stageselect_Push.png");
	m_pShadow = new SpriteEx("Assets/Texture/Result/Result_Shadow.png");
	m_pSelect[0] = new SpriteEx("Assets/Texture/Result/Result_Stageselect_Frame.png");
	m_pSelect[1] = new SpriteEx("Assets/Texture/Result/Result_Nextstage_Frame.png");
	m_pSummonData = new SpriteEx("Assets/Texture/Result/Result_Summon.png");
	m_pUnderBar = new SpriteEx("Assets/Texture/Result/Result_Underbar.png");
	m_pBack = new SpriteEx("Assets/Texture/Result/B.png");

	// -- 分岐読み込み
	if (ResultGameData.bWin)
	{
		// --- WinData
		m_pText = new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_top.png");
		m_pCharacter = new SpriteEx("Assets/Texture/Result/Win/Result_Linie_Win_Sprite.png");
		m_pLighting = new SpriteEx("Assets/Texture/Result/Win/Result_Win_Light.png");
		m_pTextShadow = new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_under.png");
		m_pClearTime = new SpriteEx("Assets/Texture/Result/Win/Result_Win_Cleartime.png");
		m_pNextUI[0] = new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
		m_pNextUI[1] = new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
		m_pStar = new SpriteEx("Assets/Texture/Star/star.png");
		m_pScore = new SpriteEx("Assets/Texture/Result/Score.png");
		m_pBestScore = new SpriteEx("Assets/Texture/Result/BestScore.png");
	}
	else
	{
		// ---- Lose
		m_pText = new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Text_top.png");
		m_pCharacter = new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Linie.png");
		m_pLighting = new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Light.png");
		m_pTextShadow = new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Text_under.png");
		m_pClearTime = new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Cleartime.png");
		m_pNextUI[0] = new SpriteEx("Assets/Texture/Result/Lose/Result_Retry.png");
		m_pNextUI[1] = new SpriteEx("Assets/Texture/Result/Lose/Result_Retry_Push.png");
		m_pStar = new SpriteEx("Assets/Texture/Star/star2.png");
	}
}
// --- 初期位置の設定
void CSceneResult::DefaultSetPos(void)
{
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
	m_pClearTime->SetPositon(1632.0f, 504.0f, 10.0f);

	m_pHitPoint->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pHitPoint->SetSize(0.3f, 0.07f, 1.0f);
	m_pHitPoint->SetPositon(1632.0f, 580.0f, 10.0f);

	m_pSummonData->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pSummonData->SetSize(0.3f, 0.14f, 1.0f);
	m_pSummonData->SetPositon(1632.0f, 693.5f, 10.0f);
	m_pNumber = new CNumberUI();
	m_pNumber->SetPos({ 920.0f, 540.0f ,0.0f });
	m_pNumber->SetScale({ 0.1f,0.2f,1.0f });
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

		m_pScore->SetRotation(0.0f, TORAD(180.0f), TORAD(190.0f));
		m_pScore->SetPositon(1350.0f, 300.0f, 0.0f);
		m_pScore->SetSize(0.15, 0.09, 1.0f);

		m_pBestScore->SetRotation(0.0f, TORAD(180.0f), TORAD(190.0f));
		m_pBestScore->SetPositon(1350.0f, 300.0f, 0.0f);
		m_pBestScore->SetSize(0.15, 0.09, 1.0f);


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
}
// --- 入力処理
void CSceneResult::KeyProsess(void)
{
	if (WihtGetKeyPress(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyPress('A'))
	{
		// SE
		if (nSelect != 0)
		{
			m_pResultSelectSE->Stop();
			m_pResultSelectSE->FlushSourceBuffers();
			XAUDIO2_BUFFER buffer;
			buffer = m_pResultSelectSound->GetBuffer(false);
			m_pResultSelectSE->SubmitSourceBuffer(&buffer);
			if (m_pResultSelectSE)SetVolumeBGM(m_pResultSelectSE);
			m_pResultSelectSE->Start();
		}
		nSelect = 0;
	}

	if (WihtGetKeyPress(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyPress('D'))
	{
		// SE
		if (nSelect != 1)
		{
			m_pResultSelectSE->Stop();
			m_pResultSelectSE->FlushSourceBuffers();
			XAUDIO2_BUFFER buffer;
			buffer = m_pResultSelectSound->GetBuffer(false);
			m_pResultSelectSE->SubmitSourceBuffer(&buffer);
			if (m_pResultSelectSE)SetVolumeBGM(m_pResultSelectSE);
			m_pResultSelectSE->Start();
		}
		nSelect = 1;
	}

	if (nSelect == 0)
	{
		// StageSelect
		if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN))
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
				StageLevel.StageSubNumber++;
				if (StageLevel.StageSubNumber < 3)
				{
					StageLevel.StageMainNumber = 0;
				}
				else
				if (StageLevel.StageSubNumber < 6)
				{
					StageLevel.StageMainNumber = 1;
				}
				else
				if (StageLevel.StageSubNumber < 9)
				{
					StageLevel.StageSubNumber = 2;
				}
				else
				{
					// 全クリ
					SetNext(STAGE_SELECT);
				}
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


}
// --- 全体のAnimation管理
void CSceneResult::AnimationManagemer(void)
{
	if (timeGetTime() - nAnimationTimer >= 20.0f)
	{
		nAnimationFrame++;
		nAnimationTimer = timeGetTime();
		bAnimation = true;
	}
	else
	{
		bAnimation = false;
	}
}
// --- カメラ情報の更新
void CSceneResult::SetCamData(void)
{
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pStageSelect[nLoop]->SetProjection(Get2DProj());
		m_pStageSelect[nLoop]->SetView(Get2DView());
		m_pSelect[nLoop]->SetProjection(Get2DProj());
		m_pSelect[nLoop]->SetView(Get2DView());
		m_pNextUI[nLoop]->SetProjection(Get2DProj());
		m_pNextUI[nLoop]->SetView(Get2DView());
	}
	m_pScore->SetView(Get2DView());
	m_pScore->SetProjection(Get2DProj());
	m_pBestScore->SetView(Get2DView());
	m_pBestScore->SetProjection(Get2DProj());
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
}


void CSceneResult::WinAnimation(void)
{
	// -- 描画
	m_pBack->SetTexture();
	m_pBack->Disp();
	m_pLighting->SetTexture();
	m_pLighting->Disp();
	m_pShadow->SetTexture();
	m_pShadow->Disp();

	// --　分岐によるアニメーション処理
	
	// Animation計算
	float UvSize;
	UvSize = (1.0f / 8.0f);
	int nUvMovePosX, nUvMovePosY;
	

	if (nAnimationFrame > 63)
	{
		nAnimationFrame = 63;

		if (bAnimation)
		{
			if (bCAnimation)
			{
				nCAnimation++;
				if (nCAnimation > 15)bCAnimation = false;
			}
			else
			{
				nCAnimation--;
				if (nCAnimation < 1)bCAnimation = true;
			}
		}

	}

	int nX, nY;
	nX = nY = 0;
	nX = nCAnimation % 8;
	nY = nCAnimation / 8;
	nUvMovePosX = (nAnimationFrame) % 8;
	nUvMovePosY = (nAnimationFrame) / 8;

	float fUvPosX, fUvPosY;

	fUvPosX = UvSize * ((float)nUvMovePosX - (float)nX);
	fUvPosY = UvSize * ((float)nUvMovePosY - (float)nY);




	m_pCharacter->SetUvSize((1.0f / 8.0f), (1.0f / 8.0f));
	m_pCharacter->SetUvPos(fUvPosX, fUvPosY);
	m_pCharacter->SetTexture();
	m_pCharacter->Disp();
	m_pCharacter->SetUvSize(1.0f, 1.0f);
	m_pCharacter->SetUvPos(0.0f, 0.0f);

	m_pText->SetTexture();
	m_pText->Disp();
	

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

	m_pStageSelect[0]->SetTexture();
	m_pStageSelect[0]->Disp();

	m_pNextUI[0]->SetTexture();
	m_pNextUI[0]->Disp();

	m_pSelect[nSelect]->SetTexture();
	m_pSelect[nSelect]->Disp();





	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f + fStarAngle));
	m_pStar->SetSize(0.05f, 0.1f, 1.0f);
	m_pStar->SetPositon(200.0f, 140.0f, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();

	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f - fStarAngle));
	m_pStar->SetSize(0.05f, 0.1f, 1.0f);
	m_pStar->SetPositon(400.0f, 340.0f, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();

	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f + fStarAngle));
	m_pStar->SetSize(0.05f, 0.1f, 1.0f);
	m_pStar->SetPositon(1500.0f, 100.0f, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();

	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f - fStarAngle));
	m_pStar->SetSize(0.05f, 0.1f, 1.0f);
	m_pStar->SetPositon(1700.0f, 140.0f, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();



}

void CSceneResult::LoseAnimation(void)
{
	// -- 描画
	m_pBack->SetTexture();
	m_pBack->Disp();
	m_pLighting->SetTexture();
	m_pLighting->Disp();
	m_pShadow->SetTexture();
	m_pShadow->Disp();

	// --　分岐によるアニメーション処理
	

	fCTime += 0.1f;
	CPosY -= 20;
	StarPosY = InEasing(0.0f, 730.0f, 5, 1.0f) - 10.0f;

	StarPosY -= InEasing(fCTime, 0.0f, 50.0f, 1.0f);
	if (CPosY <= 0)
	{
		CPosY = 0;

		if (nCount < 3)
		{
			nCount += 0.1f;
		}
		else
		{
			fTime += 0.1f;
			StarPosY = InEasing(fTime, 730.0f, 5, 1.0f);
			StarPosY -= 100.0f;
			fStarAngle -= 1.0f;
			if (StarPosY >= 720.0f)
			{
				StarPosY = 740.0f;
			}



			if (fStarAngle < 0.0f)
			{
				fStarAngle = 0.0f;
			}

			if (LogoAngle < 10.0f)
			{
				LogoAngle++;
			}
		}
	}
	else
	{
		nAnimationFrame = 0;
	}
	CScle = sinf(nCount);
	CScle = CScle / 20.0f;

	m_pCharacter->SetSize(0.4f, 0.4f - CScle, 1.0f);
	m_pCharacter->SetPositon(940.0f, 620.0f - CPosY, 10.0f);
	m_pCharacter->SetTexture();
	m_pCharacter->Disp();

	float Angle;
	Angle = 180.0f + (float)LogoAngle;

	m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(Angle));
	m_pText->SetPositon(960.0f, 150.0f, 10.0f);
	m_pText->SetTexture();
		m_pText->Disp();

	

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
	m_pStageSelect[0]->SetTexture();
	m_pStageSelect[0]->Disp();
	m_pNextUI[0]->SetTexture();
	m_pNextUI[0]->Disp();
	m_pSelect[nSelect]->SetTexture();
	m_pSelect[nSelect]->Disp();

	// 分岐による星描画処理

	float dAngle;
	dAngle = 225.0f + fStarAngle;
	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(dAngle));
	m_pStar->SetSize(0.075f, 0.15f, 1.0f);
	m_pStar->SetPositon(300.0f, StarPosY, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();
	dAngle = 180.0f - fStarAngle;
	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(dAngle));
	m_pStar->SetSize(0.075f, 0.15f, 1.0f);
	m_pStar->SetPositon(475.0f, StarPosY, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();
	dAngle = 180.0f + fStarAngle;
	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(dAngle));
	m_pStar->SetSize(0.075f, 0.15f, 1.0f);
	m_pStar->SetPositon(1350.0f, StarPosY, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();
	dAngle = 225.0f - fStarAngle;
	m_pStar->SetRotation(0.0f, TORAD(180.0f), TORAD(dAngle));
	m_pStar->SetSize(0.075f, 0.15f, 1.0f);
	m_pStar->SetPositon(1600.0f, StarPosY, 10.0f);
	m_pStar->SetTexture();
	m_pStar->Disp();

}

void CSceneResult::NumberDisp(void)
{
	m_pNumber->SetArrangment(m_pNumber->Left_AL);
	m_pNumber->SetNumberColor(1);
	// 数字の描画
	if (ResultGameData.bWin)
	{

		int nMinutes = 0;
		int nSeconds = 0;
		nMinutes = ResultGameData.nTime / 60;
		nSeconds = ResultGameData.nTime % 60;

		if (nSeconds < 0)
		{
			nSeconds = 0;
		}

		// 秒数　(分)
		m_pNumber->SetNumber(nMinutes);
		m_pNumber->SetLend(2);
		m_pNumber->SetPos({ 1800.0f, 505.0f ,0.0f });
		m_pNumber->SetScale({ 0.03f,0.06f,1.0f });
		m_pNumber->Draw();
		// 秒数　(秒)
		m_pNumber->SetNumber(nSeconds);
		m_pNumber->SetLend(2);
		m_pNumber->SetPos({ 1890.0f, 505.0f ,0.0f });
		m_pNumber->SetScale({ 0.03f,0.06f,1.0f });
		m_pNumber->Draw();
	}
	//  Hp割合
	m_pNumber->SetNumber(ResultGameData.nHitPoint);
	if (ResultGameData.nHitPoint < 0)
	{
		m_pNumber->SetNumber(123);
	}
	m_pNumber->SetPos({ 1830.0f, 580.0f ,0.0f });
	m_pNumber->SetScale({ 0.03f,0.06f,1.0f });
	m_pNumber->Draw();
	//	合計召喚数
	m_pNumber->SetNumber(ResultGameData.nSpawnCount);
	if (ResultGameData.nSpawnCount < 0)
	{
		m_pNumber->SetNumber(0);
	}
	m_pNumber->SetPos({ 1830.0f, 650.0f ,0.0f });
	m_pNumber->SetScale({ 0.03f,0.06f,1.0f });
	m_pNumber->Draw();
	// 平均召喚数

	int nAv;
	if (ResultGameData.nSpawnCount > 0 && ResultGameData.nDrawCount > 0)
	{
		nAv = ResultGameData.nSpawnCount / ResultGameData.nDrawCount;
		m_pNumber->SetNumber(nAv);
	}
	else
	{
		m_pNumber->SetNumber(0);
	}
	m_pNumber->SetPos({ 1835.0f, 700.0f ,0.0f });
	m_pNumber->SetScale({ 0.015f,0.03f,1.0f });
	m_pNumber->Draw();
	// 描画回数
	m_pNumber->SetNumber(ResultGameData.nDrawCount);
	if (ResultGameData.nDrawCount < 0)
	{
		m_pNumber->SetNumber(0);
	}
	m_pNumber->SetPos({ 1835.0f, 730.0f ,0.0f });
	m_pNumber->SetScale({ 0.015f,0.03f,1.0f });
	m_pNumber->Draw();


	if (ResultGameData.bWin)
	{
		int nSCORE;
		nSCORE = 0;

		if (ResultGameData.nHitPoint >= 0 && ResultGameData.nSpawnCount >= 0 && ResultGameData.nTime >= 0)
		{
			if (ResultGameData.nHitPoint * ResultGameData.nSpawnCount > ResultGameData.nTime)
			{
				nSCORE = ResultGameData.nHitPoint * ResultGameData.nSpawnCount  * nAv/ ResultGameData.nTime;
			}
			else
			{
				nSCORE = 11114514;
			}
		}
		else
		{
			nSCORE = 11111514;
		}

		if (bScore)
		{
			if (nBestScore[StageLevel.StageSubNumber] < nSCORE)
			{
				nBestScore[StageLevel.StageSubNumber] = nSCORE;
				bBestScore = true;
			}
			else
			{
				bBestScore = false;
			}
			bScore = false;
		}

		if (bBestScore)
		{
			m_pBestScore->SetTexture();
			m_pBestScore->Disp();
		}
		else
		{
			m_pScore->SetTexture();
			m_pScore->Disp();
		}

		m_pNumber->SetNumberColor(2);
		m_pNumber->SetNumber(nSCORE);
		m_pNumber->SetPos({ 1850.0f , 390.0f,0.0f });
		m_pNumber->SetScale({ 0.09f,0.18f,1.0f });
		m_pNumber->Draw();

	}


}
