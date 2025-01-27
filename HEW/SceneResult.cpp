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
bool CSceneResult::bClearState[8];


CSceneResult::CSceneResult()
	:nSelect(0), nAnimationFrame(0), bScore(true), bBestScore(false), bWorldClear(false), nPush{},bAnimation(false),fPiyoA(0.0f),nScore(0)
{
	//ResultGameData.bWin = 1;
	// --- テクスチャの読み込み
	LoadTexture();
	
	// ---　場所の設定
	DefaultSetPos();

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


	int nSum;
	nSum = StageLevel.StageSubNumber % 3;
	if (nSum == 2)
	{
		bWorldClear = true;
	}
	else
	{
		bWorldClear = false;
	}
	if (ResultGameData.bWin)
	{
		bClearState[StageLevel.StageSubNumber] = true;
	}



	m_pResultSelectSound    = new CSoundList(SE_SELECT);
	m_pResultSelectSE = m_pResultSelectSound->GetSound(false);

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
		if (m_pSelect)
		{
			delete m_pSelect;
			m_pSelect = nullptr;
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
	// 背景
	m_pBack->SetTexture();
	m_pBack->Disp();

	if (ResultGameData.bWin)
	{
		WinAnimation();

		WinDisp();

		NumberDisp();
	}
	else
	{
		LoseAnimation();

		LoseDisp();
	}

	if (!bWorldClear)
	{
		m_pNextUI[nPush[1]]->SetTexture();
		m_pNextUI[nPush[1]]->Disp();
	}

	m_pStageSelect[nPush[0]]->SetTexture();
	m_pStageSelect[nPush[0]]->Disp();

	m_pSelect->SetTexture();
	m_pSelect->Disp();

	m_pUnderBar->SetTexture();
	m_pUnderBar->Disp();
	
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

int  CSceneResult::OutBestScore(StageType InStage)
{
	return nBestScore[InStage.StageSubNumber];
}

bool CSceneResult::GetStageClear(int nStage)
{
	return bClearState[nStage];
}

void CSceneResult::SetStageClear(int nStage, bool bWin)
{
	bClearState[nStage] = bWin;
}


// --- テクスチャの読み込み
void CSceneResult::LoadTexture(void)
{
	m_pHitPoint = new SpriteEx("Assets/Texture/Result/Result_Helth.png");
	m_pStageSelect[0] = new SpriteEx("Assets/Texture/Result/Result_Side_Stageselect.png");
	m_pStageSelect[1] = new SpriteEx("Assets/Texture/Result/Result_Side_Stageselect_Push.png");
	m_pShadow = new SpriteEx("Assets/Texture/Result/Result_Shadow.png");
	m_pSelect = new SpriteEx("Assets/Texture/Result/Result_Nextstage_Frame.png");
	m_pSummonData = new SpriteEx("Assets/Texture/Result/Result_Summon.png");
	m_pUnderBar = new SpriteEx("Assets/Texture/Result/Result_Underbar.png");
	m_pBack = new SpriteEx("Assets/Texture/Result/B.png");
	m_pPiyo = new SpriteEx("Assets/Texture/Result/stan.png");
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
		m_pStageSelect[nLoop]->SetSize(600.0f, 80.0f, 1.0f);
		m_pStageSelect[nLoop]->SetPositon(1630.0f, 940.0f, 10.0f);

		m_pNextUI[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pNextUI[nLoop]->SetSize(600.0f, 80.0f, 1.0f);
		m_pNextUI[nLoop]->SetPositon(1630.0f, 840.0f, 10.0f);
	}
	m_pBack->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pBack->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pBack->SetSize(2000.0f, 2000.0f, 1.0f);
	m_pBack->SetPositon(960.0f, 540.0f, 10.0f);

	m_pSelect->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pSelect->SetSize(620.0f, 105.0f, 1.0f);
	m_pSelect->SetPositon(1630.0f, 940.0f, 10.0f);

	m_pUnderBar->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pUnderBar->SetSize(2000.0f, 50.0f, 1.0f);
	m_pUnderBar->SetPositon(960.0f, 1050.0f, 10.0f);

	m_pClearTime->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pClearTime->SetSize(600.0f, 70.0f, 1.0f);
	m_pClearTime->SetPositon(1632.0f, 434.0f, 10.0f);

	m_pHitPoint->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pHitPoint->SetSize(600.0f, 70.0f, 1.0f);
	m_pHitPoint->SetPositon(1632.0f, 504.0f, 10.0f);

	m_pSummonData->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pSummonData->SetSize(600.0f, 140.0f, 1.0f);
	m_pSummonData->SetPositon(1632.0f, 610.0f, 10.0f);
	
	m_pPiyo->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pPiyo->SetSize(300.0f, 100.0f, 1.0f);
	m_pPiyo->SetPositon(600.0f, 600.0f, 10.0f);

	m_pBestScore->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pBestScore->SetSize(300.0f, 100.0f, 1.0f);
	m_pBestScore->SetPositon(1400.0f, 300.0f, 10.0f);

	m_pScore->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pScore->SetSize(300.0f, 100.0f, 1.0f);
	m_pScore->SetPositon(1400.0f, 300.0f, 10.0f);

	if (ResultGameData.bWin)
	{

		m_pTextShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pTextShadow->SetSize(700.0f, 50.0f, 1.0f);
		m_pTextShadow->SetPositon(920.0f, 200.0f, 10.0f);

		m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pText->SetSize(700.0f, 123.0f, 1.0f);
		m_pText->SetPositon(920.0f, 140.0f, 10.0f);

		m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pLighting->SetSize(940.0f, 1000.0f, 1.0f);
		m_pLighting->SetPositon(460.0f, 440.0f, 10.0f);

		m_pCharacter->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacter->SetSize(800.0f, 750.0f, 1.0f);
		m_pCharacter->SetPositon(560.0f, 440.0f, 10.0f);

		m_pShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pShadow->SetSize(400.0f, 150.0f, 1.0f);
		m_pShadow->SetPositon(460.0f, 820.0f, 10.0f);
	}
	else
	{
		m_pTextShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pTextShadow->SetSize(700.0f, 50.0f, 1.0f);
		m_pTextShadow->SetPositon(920.0f, 200.0f, 10.0f);

		m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(190.0f));
		m_pText->SetSize(700.0f, 123.0f, 1.0f);
		m_pText->SetPositon(920.0f, 140.0f, 10.0f);

		m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pLighting->SetSize(940.0f, 600.0f, 1.0f);
		m_pLighting->SetPositon(460.0f, 640.0f, 10.0f);

		m_pCharacter->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacter->SetSize(600.0f, 300.0f, 1.0f);
		m_pCharacter->SetPositon(490.0f, 700.0f, 10.0f);

		m_pShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pShadow->SetSize(600.0f, 150.0f, 1.0f);
		m_pShadow->SetPositon(470.0f, 800.0f, 10.0f);

	}
}

// --- 入力処理
void CSceneResult::KeyProsess(void)
{
	if (!bWorldClear)
	{
		if (WihtGetKeyPress(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyPress('S'))
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
				m_pSelect->SetPositon(1630.0f, 940.0f, 10.0f);
			}
			nSelect = 0;
		}

		if (WihtGetKeyPress(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyPress('W'))
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
				m_pSelect->SetPositon(1630.0f, 840.0f, 10.0f);
			}
			nSelect = 1;
		}
	}
	if (nSelect == 0)
	{
		// StageSelect
		if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN))
		{
			nPush[nSelect] = 1;
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
					StageLevel.StageMainNumber = 2;
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
		bAnimation = true;
		nAnimationTimer = timeGetTime();
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
		m_pNextUI[nLoop]->SetProjection(Get2DProj());
		m_pNextUI[nLoop]->SetView(Get2DView());
	}
	
	m_pSelect->SetProjection(Get2DProj());
	m_pSelect->SetView(Get2DView());

	if (ResultGameData.bWin)
	{
		m_pScore->SetView(Get2DView());
		m_pScore->SetProjection(Get2DProj());
		m_pBestScore->SetView(Get2DView());
		m_pBestScore->SetProjection(Get2DProj());
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
	m_pPiyo->SetProjection(Get2DProj());
	m_pPiyo->SetView(Get2DView());
}

// --- 描画アニメーション
void CSceneResult::WinAnimation(void)
{
	if (bAnimation)
	{
		nAnimationFrame++;

		int nUvPos;
		int nX, nY;
		// --- アニメーション
		if (nAnimationFrame > 63)
		{
			static bool bLoopAnimation = true;
			static int nLoopAnimation = 0;
			nUvPos = 63;



			if (bLoopAnimation)
			{
				nLoopAnimation++;
				if (nLoopAnimation > 15)bLoopAnimation = false;
			}
			else
			{
				nLoopAnimation--;
				if (nLoopAnimation < 1)bLoopAnimation = true;
			}


	
			nX = nUvPos % 8;
			nY = nUvPos / 8;

			nX = nX - nLoopAnimation % 8;
			nY = nY - nLoopAnimation / 8;


		}
		else
		{
			nUvPos = nAnimationFrame;
	
			nX = nUvPos % 8;
			nY = nUvPos / 8;
		}

		float fUvSize;
		fUvSize = 1.0f / 8.0f;
		

		
		fUvPos.X = fUvSize * (float)nX;
		fUvPos.Y = fUvSize * (float)nY;



	}
}

void CSceneResult::LoseAnimation(void)
{
	if (bAnimation)
	{
		
		static float fFream = 0;
		fFream += 0.05f;
		if (fFream > 1)
		{
			float Pos = Easing39(1.0f);
			Pos = Pos * 100.0f;
			m_pCharacter->SetPositon(490.0f, 800.0f - Pos, 10.0f);
			
			fPiyoA = fFream - 1.0f;
			if (fPiyoA < 0.0f)fPiyoA = 0.0f;

			if (fFream > 2.0f)fFream = 2.0f;
		}
		else
		{

			float Pos = Easing39(fFream);
			Pos = Pos * 100.0f;
			m_pCharacter->SetPositon(490.0f, 800.0f - Pos, 10.0f);
		}

		nAnimationFrame++;
		static int PiyoF = 0;
		if (nAnimationFrame > 1)
		{
			nAnimationFrame = 0;
			PiyoF++;
		}

		int nF;
		int nP;
		nF = PiyoF % 4;
		nP = PiyoF / 4;

		if (nF * nP > 21)
		{
			PiyoF = 0;
		}

		fUvPos.X = (1.0f / 4.0f) * (float)nF;
		fUvPos.Y = (1.0f / 21.0f) * (float)nP;

	}
}

// --- 描画
void CSceneResult::WinDisp(void)
{
	m_pClearTime->SetTexture();
	m_pClearTime->Disp();

	m_pHitPoint->SetTexture();
	m_pHitPoint->Disp();

	m_pSummonData->SetTexture();
	m_pSummonData->Disp();

	m_pLighting->SetTexture();
	m_pLighting->Disp();


	m_pTextShadow->SetTexture();
	m_pTextShadow->Disp();

	m_pText->SetTexture();
	m_pText->Disp();

	m_pShadow->SetTexture();
	m_pShadow->Disp();

	m_pCharacter->SetUvPos(fUvPos.X, fUvPos.Y);
	m_pCharacter->SetUvSize(1.0f / 8.0f, 1.0f / 8.0f);
	m_pCharacter->SetTexture();
	m_pCharacter->Disp();

	m_pCharacter->SetUvSize(1.0f, 1.0f);
	m_pCharacter->SetUvPos(0.0f, 0.0f);
}

void CSceneResult::LoseDisp(void)
{
	m_pTextShadow->SetTexture();
	m_pTextShadow->Disp();

	m_pText->SetTexture();
	m_pText->Disp();

	m_pLighting->SetTexture();
	m_pLighting->Disp();

	m_pShadow->SetTexture();
	m_pShadow->Disp();

	m_pCharacter->SetTexture();
	m_pCharacter->Disp();

	m_pPiyo->Setcolor(1.0f, 1.0f, 1.0f, fPiyoA);
	m_pPiyo->SetUvPos(fUvPos.X, fUvPos.Y);
	m_pPiyo->SetUvSize(1.0f / 4.0f, 1.0f / 21.0f);
	m_pPiyo->SetTexture();
	m_pPiyo->Disp();
	m_pPiyo->SetUvSize(1.0f, 1.0f);
	m_pPiyo->SetUvPos(0.0f, 0.0f);
	m_pPiyo->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
}

void CSceneResult::NumberDisp(void)
{
	if (ResultGameData.nAverageSpwn <= 0 || ResultGameData.nDrawCount <= 0 || ResultGameData.nHitPoint <= 0 || ResultGameData.nSpawnCount <= 0 || ResultGameData.nTime <= 0)
	{
		ResultGameData.nAverageSpwn = 114;
		ResultGameData.nTime = 114;
		ResultGameData.nHitPoint = 114;
		ResultGameData.nSpawnCount = 114;
		ResultGameData.nDrawCount = 114;
	}






	m_pNumber->SetArrangment(m_pNumber->Left_AL);
	m_pNumber->SetNumberColor(1);


	// --- 時間計算
	int nM, nS;
	nM = ResultGameData.nTime / 60;
	nS = ResultGameData.nTime % 60;


	// 秒数Animation
	static int nMM = 0;
	static int nSS = 0;
	bool bTime = false;

	nSS++;
	if (nSS > 60)
	{
		nSS = 0;
		nMM++;
	}
	if (nMM >= nM)
	{
		if (nSS >= nS)
		{
			bTime = true;
			nSS = nS;
		}
	}

	// --- 秒数
	m_pNumber->SetNumber(nSS);
	m_pNumber->SetLend(2);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1895.0f,435.0f,0.0f });
	m_pNumber->Draw();

	// --- 分数
	m_pNumber->SetNumber(nMM);
	m_pNumber->SetLend(2);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1810.0f,435.0f,0.0f });
	m_pNumber->Draw();

	// 体力Animation
	bool bHp = false;
	static int nHp = 0;
	if (bTime)
	{
		nHp++;
		if (nHp >= ResultGameData.nHitPoint)
		{
			bHp = true;
			nHp = ResultGameData.nHitPoint;
		}
	}
	// --- 体力
	m_pNumber->SetNumber(nHp);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1840.0f,505.0f,0.0f });
	m_pNumber->Draw();

	// 召喚合計
	bool bSpawn = false;
	static int nSpawn = 0;
	if (bHp)
	{
		nSpawn++;
		if (nSpawn >= ResultGameData.nSpawnCount)
		{
			bSpawn = true;
			nSpawn = ResultGameData.nSpawnCount;
		}
	}
	m_pNumber->SetNumber(nSpawn);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1840.0f,572.0f,0.0f });
	m_pNumber->Draw();

	// 平均賞関数
	bool bAvSpawn = false;
	static int nAvS = 0;
	if (bSpawn)
	{
		nAvS++;
		if (nAvS >= ResultGameData.nAverageSpwn)
		{
			bAvSpawn = true;
			nAvS = ResultGameData.nAverageSpwn;
		}
	}
	m_pNumber->SetNumber(nAvS);
	m_pNumber->SetScale({ 40.0f,40.0f,0.0f });
	m_pNumber->SetPos({ 1840.0f,615.0f,0.0f });
	m_pNumber->Draw();


	// 描画回数
	static int nDisp = 0;
	if (bAvSpawn)
	{
		nDisp++;
		if (nDisp >= ResultGameData.nDrawCount)
		{
			nDisp = ResultGameData.nDrawCount;
		}
	}
	m_pNumber->SetNumber(nAvS);
	m_pNumber->SetScale({ 40.0f,40.0f,0.0f });
	m_pNumber->SetPos({ 1840.0f,645.0f,0.0f });
	m_pNumber->Draw();

	// SCORE
	int nScore;
	nScore = ResultGameData.nHitPoint * ResultGameData.nSpawnCount * ResultGameData.nAverageSpwn / ResultGameData.nTime;
	static  int nnScore = 0;

	if (nBestScore[StageLevel.StageSubNumber] < nScore)
	{
		nBestScore[StageLevel.StageSubNumber] = nScore;
		m_pBestScore->SetTexture();
		m_pBestScore->Disp();
	}
	else
	{
		m_pScore->SetTexture();
		m_pScore->Disp();
	}

	nnScore += 100;
	if (nnScore >= nScore)
	{
		nnScore = nScore;
	}
	m_pNumber->SetNumberColor(2);
	m_pNumber->SetNumber(nnScore);
	m_pNumber->SetScale({ 90.0f,90.0f,0.0f });
	m_pNumber->SetPos({ 1840.0f,350.0f,0.0f });
	m_pNumber->Draw();
}
