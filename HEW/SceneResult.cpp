// --- Include
#include "SceneResult.h"
#include"Input.h"
#include"InputEx.h"
#include"Controller.h"
#include"InputEx.h"
#include"Easing.h"
#include "File.h"
// ---

// --- SaverIPSetting
#define SAVER_IP ("14.133.0.16")
// ---

// --- static
ResultGameInfo CSceneResult::ResultGameData;
StageType CSceneResult::StageLevel;
int CSceneResult::nBestScore[MAX_STAGE];
bool CSceneResult::bClearState[MAX_STAGE];
Rank CSceneResult::StageRank[MAX_STAGE];
// ---

// --- 初期化
CSceneResult::CSceneResult()
	:nSelect(0), nAnimationFrame(0), nPush{}, bAnimation(false), bEnter(false), fUvPos{}, Group{ 1920.0f,660.0f }, bLoopAnime(false), nLoopAnimeCount(0),
	bTimeProsess(false),nSeconds(0),nMinutes(0),bHPProsess(false),nHitPoint(0),bSpwanProsess(false),nSpawn(0),nAverage(0),nDispCount(0),nScore(0),bBestScore(false),nStage(StageLevel.StageSubNumber)
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
		m_pResultSound->SetMasterVolume();
		SetVolumeBGM(m_pResultBGM);
		m_pResultBGM->Start();
	}
	else
	{
		m_pResultSound = new CSoundList(BGM_GAMEOVER);
		m_pResultBGM = m_pResultSound->GetSound(true);
		m_pResultSound->SetMasterVolume();
		SetVolumeBGM(m_pResultBGM);
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
	m_pResultSelectSound->SetMasterVolume();
	m_pResultSelectSE = m_pResultSelectSound->GetSound(false);

	m_pNumber = new CNumberUI();


	if (ResultGameData.nAverageSpwn <= 0 && ResultGameData.nDrawCount <= 0 && ResultGameData.nHitPoint <= 0 && ResultGameData.nSpawnCount <= 0)
	{
		ResultGameData.nTime = 444;
		ResultGameData.nHitPoint = 444;
		ResultGameData.nSpawnCount = 444;
		ResultGameData.nDrawCount = 444;
		ResultGameData.nAverageSpwn = 444;
	}

	nScore = (sqrt(ResultGameData.nSpawnCount) * 100) + (5000 / ResultGameData.nTime + 1) + (ResultGameData.nAverageSpwn * 150) + (ResultGameData.nHitPoint * 5);


	if (nBestScore[StageLevel.StageSubNumber] < nScore)
	{
		nBestScore[StageLevel.StageSubNumber] = nScore;
		bBestScore = true;
	}

	char buf[255];
	sprintf(buf, "%d", nScore);


	Acquired_Data SetData;
	SetData.bNew = true;
	SetData.nScore = nScore;
	SetData.Stage_Number = StageLevel.StageSubNumber;

	SetIP(SAVER_IP);
	SendData(SetData);

	if (nBestScore[StageLevel.StageSubNumber] <= 1999)
	{
		StageRank[StageLevel.StageSubNumber] = C_RANK;
	}else
	if (nBestScore[StageLevel.StageSubNumber] <= 3999)
	{
		StageRank[StageLevel.StageSubNumber] = B_RANK;
	}else
	if (nBestScore[StageLevel.StageSubNumber] <= 5999)
	{
			StageRank[StageLevel.StageSubNumber] = A_RANK;
	}
	else
	{
		StageRank[StageLevel.StageSubNumber] = S_RANK;
	}
	// 現在のランクの設定
	NowRank = NONE_RANK;
	if (nScore <= 1999)
	{
		NowRank = C_RANK;
	}
	else
	if (nScore <= 3999)
	{
		NowRank = B_RANK;
	}
	else
	if (nScore <= 5999)
	{
		NowRank = A_RANK;
	}
	else
	{
		NowRank = S_RANK;
	}



}
CSceneResult::~CSceneResult()
{
	InStageData();
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

	if (m_pBackGround)
	{
		delete m_pBackGround;
		m_pBackGround = nullptr;
	}

	if (m_pUnberUI)
	{
		delete m_pUnberUI;
		m_pUnberUI = nullptr;
	}

	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		if ((m_pStageSelectUI[nLoop]))
		{
			delete m_pStageSelectUI[nLoop];
			m_pStageSelectUI[nLoop] = nullptr;
		}
		if (m_pNextUI[nLoop])
		{
			delete m_pNextUI[nLoop];
			m_pNextUI[nLoop] = nullptr;
		}
	}
	if (m_pSelectUI)
	{
		delete m_pSelectUI;
		m_pSelectUI = nullptr;
	}
	if (m_pResultData)
	{
		delete m_pResultData;
		m_pResultData = nullptr;
	}
	if (m_pLighting)
	{
		delete  m_pLighting;
		m_pLighting = nullptr;
	}
	if (m_pText)
	{
		delete m_pText;
		m_pText = nullptr;
	}
	for (int nLoop = 0; nLoop < NONE_RANK; nLoop++)
	{
		if (m_pRank[nLoop])
		{
			delete m_pRank[nLoop];
			m_pRank[nLoop] = nullptr;
		}
	}
	if (m_pCharacter)
	{
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}
	if (m_pNumber)
	{
		delete m_pNumber;
		m_pNumber = nullptr;
	}
	if (m_pBestScore)
	{
		delete m_pBestScore;
		m_pBestScore = nullptr;
	}

}

// --- 全体の更新処理
void CSceneResult::Update()
{
	//--- 入力処理
	KeyProsess();

	// --- AnimationProcess
	AnimationManagemer();

	// --- カメラ情報の更新
	SetCamData();


}
// --- 全体の描画処理
void CSceneResult::Draw()
{
	// 背景描画
	m_pBackGround->SetTexture();
	m_pBackGround->Disp();

	// 分岐処理
	if (ResultGameData.bWin)
	{
		// 勝利時の描画
		WinAnimation();

		WinDisp();

		m_pResultData->SetTexture();
		m_pResultData->Disp();

		NumberDisp();
	}
	else
	{
		// 敗北時の描画
		LoseAnimation();

		LoseDisp();
	}

	// UI系
	m_pStageSelectUI[nPush[0]]->SetTexture();
	m_pStageSelectUI[nPush[0]]->Disp();

	if (!bWorldClear)
	{
		m_pNextUI[nPush[1]]->SetTexture();
		m_pNextUI[nPush[1]]->Disp();
	}

	m_pSelectUI->SetTexture();
	m_pSelectUI->Disp();

	m_pUnberUI->SetTexture();
	m_pUnberUI->Disp();
}

// --- static 変数の初期化
void CSceneResult::Init_Result()
{
	StageLevel.StageMainNumber	= 0;
	StageLevel.StageSubNumber	= 0;
	ResultGameData.bWin			= true;
	ResultGameData.nAverageSpwn = 1;
	ResultGameData.nDrawCount	= 1;
	ResultGameData.nHitPoint	= 1;
	ResultGameData.nSpawnCount	= 1;
	ResultGameData.nTime		= 1;
	for (int nLoop = 0; nLoop < MAX_STAGE; nLoop++)
	{
		nBestScore[nLoop]	 = 0;
		bClearState[nLoop]	 = false;
		StageRank[nLoop]	 = NONE_RANK;
	}
}

// --- リザルト情報の設定
void CSceneResult::InResultData(ResultGameInfo InData)
{
	if (InData.nSpawnCount > 0 && InData.nDrawCount > 0)
	{
		InData.nAverageSpwn = InData.nSpawnCount / InData.nDrawCount;
	}
	ResultGameData = InData;
}
// --- ステージ情報を設定
void CSceneResult::InStageLevel(StageType  nInLevel)
{
	StageLevel = nInLevel;
}
// --- ベストSCORE情報の設定
void CSceneResult::InBestScore(int InScore, StageType InStage)
{
	nBestScore[InStage.StageSubNumber] = InScore;
}
// --- ベストSCORE情報の取得
int  CSceneResult::OutBestScore(StageType InStage)
{
	return nBestScore[InStage.StageSubNumber];
}
// --- ステージクリア情報の取得
bool CSceneResult::GetStageClear(int nStage)
{
	return bClearState[nStage];
}
// ---　ステージクリア情報の設定
void CSceneResult::SetStageClear(int nStage, bool bWin)
{
	bClearState[nStage] = bWin;
}
// --- ステージクリア情報の取得
Rank CSceneResult::GetRankData(int nStage)
{
	return StageRank[nStage];
}
// --- ステージクリア情報の設定
void CSceneResult::SetRankData(int nStage, Rank SetRank)
{
	StageRank[nStage] = SetRank;
}


// --- テクスチャの読み込み
void CSceneResult::LoadTexture(void)
{
	m_pBackGround		= new SpriteEx("Assets/Texture/Result/B.png");
	m_pUnberUI			= new SpriteEx("Assets/Texture/Result/UI/Result_Underbar.png");
	m_pStageSelectUI[0] = new SpriteEx("Assets/Texture/Result/UI/Result_01.png");
	m_pStageSelectUI[1] = new SpriteEx("Assets/Texture/Result/UI/Result_02.png");
	m_pSelectUI			= new SpriteEx("Assets/Texture/Result/UI/Result_Selected.png");
	m_pResultData		= new SpriteEx("Assets/Texture/Result/UI/Result_List.png");
	m_pRank[S_RANK]		= new SpriteEx("Assets/Texture/Result/Rank/Rank_S.png");
	m_pRank[A_RANK]		= new SpriteEx("Assets/Texture/Result/Rank/Rank_A.png");
	m_pRank[B_RANK]		= new SpriteEx("Assets/Texture/Result/Rank/Rank_B.png");
	m_pRank[C_RANK]		= new SpriteEx("Assets/Texture/Result/Rank/Rank_C.png");
	m_pBestScore		= new SpriteEx("Assets/Texture/Result/Text/BestScore.png");
	m_pCharacterShadow = new SpriteEx("Assets/Texture/Result/shadow/Result_Shadow.png");

	if (ResultGameData.bWin)
	{
		m_pNextUI[0] = new SpriteEx("Assets/Texture/Result/UI/Result_03.png");
		m_pNextUI[1] = new SpriteEx("Assets/Texture/Result/UI/Result_04.png");

		m_pText		= new SpriteEx("Assets/Texture/Result/Text/Result_Win_Text_top.png");
		m_pLighting = new SpriteEx("Assets/Texture/Result/Lighting/Result_Win_Light.png");
		m_pTextShadow = new SpriteEx("Assets/Texture/Result/shadow/Result_Win_Text_under.png");
		m_pCharacter = new SpriteEx("Assets/Texture/Result/Character/Result_Linie_Win_Sprite.png");

	}
	else
	{
		m_pNextUI[0] = new SpriteEx("Assets/Texture/Result/UI/Result_05.png");
		m_pNextUI[1] = new SpriteEx("Assets/Texture/Result/UI/Result_06.png");
		m_pText = new SpriteEx("Assets/Texture/Result/Text/Result_Lose_Text_top.png");
		m_pLighting = new SpriteEx("Assets/Texture/Result/Lighting/Result_Lose_Light.png");
		m_pTextShadow = new SpriteEx("Assets/Texture/Result/shadow/Result_Lose_Text_under.png");
		m_pCharacter = new SpriteEx("Assets/Texture/Result/Character/Line_Lose.png");
	}


}

// --- 初期位置の設定
void CSceneResult::DefaultSetPos(void)
{
	m_pBackGround->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pBackGround->SetSize(1920.0f, 1080.0f, 0.0f);
	m_pBackGround->SetPositon(960.0f, 540.0f, 0.0f);

	m_pUnberUI->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pUnberUI->SetSize(1920.0f, 60.0f, 0.0f);
	m_pUnberUI->SetPositon(960.0f, 1050.0f, 0.0f);




	if (ResultGameData.bWin)
	{
		for (int nLoop = 0; nLoop < 2; nLoop++)
		{
			m_pStageSelectUI[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStageSelectUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
			m_pStageSelectUI[nLoop]->SetPositon(960.0f + Group.X, 900.0f, 0.0f);

			m_pNextUI[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pNextUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
			m_pNextUI[nLoop]->SetPositon(960.0f + Group.X, 790.0f, 0.0f);
		}

		m_pSelectUI->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pSelectUI->SetSize(670.0f * 1.1f, 107.0f * 1.1f, 0.0f);
		m_pSelectUI->SetPositon(960.0f + Group.X, 900.0f, 0.0f);

		m_pResultData->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pResultData->SetSize(710.0f * 0.9, 390.0f * 0.9, 0.0f);
		m_pResultData->SetPositon(1600.0f + Group.X, 520.0f, 0.0f);

		m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pLighting->SetSize(969, 982, 0.0f);
		m_pLighting->SetPositon((300.0f + Group.Y), 490.0f, 0.0f);

		m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pText->SetSize(822.0f, 143.0f, 0.0f);
		m_pText->SetPositon(960.0f + Group.X, 100.0f, 0.0f);

		for (int nLoop = 0; nLoop < NONE_RANK; nLoop++)
		{
			m_pRank[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pRank[nLoop]->SetSize(445.0f, 446.0f, 0.0f);
			m_pRank[nLoop]->SetPositon(960.0f + Group.X, 450.0f, 0.0f);
		}

		m_pCharacter->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacter->SetSize(8192.0f / 8.0f, 8192 / 8.0f,0.0f);
		m_pCharacter->SetPositon((460.0f + Group.Y), 350.0f, 0.0f);

		m_pBestScore->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pBestScore->SetSize(1302.0f * 0.3f, 368.0f * 0.3f, 0.0f);
		m_pBestScore->SetPositon(1500.0f + Group.X, 230.0f, 0.0f);

		m_pCharacterShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacterShadow->SetSize(440.0f,81.0f, 0.0f);
		m_pCharacterShadow->SetPositon((330.0f + Group.Y), 850.0f, 0.0f);

		m_pTextShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pTextShadow->SetSize(1129.0f, 73.0f, 0.0f);
		m_pTextShadow->SetPositon(960.0f + Group.X, 190.0f, 0.0f);
	}
	else
	{
		for (int nLoop = 0; nLoop < 2; nLoop++)
		{
			m_pStageSelectUI[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pStageSelectUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
			m_pStageSelectUI[nLoop]->SetPositon(960.0f, 900.0f, 0.0f);

			m_pNextUI[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pNextUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
			m_pNextUI[nLoop]->SetPositon(960.0f, 790.0f, 0.0f);
		}

		m_pSelectUI->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pSelectUI->SetSize(670.0f * 1.1f, 107.0f * 1.1f, 0.0f);
		m_pSelectUI->SetPositon(960.0f, 900.0f, 0.0f);


		m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pLighting->SetSize(969 * 1.1f, 642 * 1.1f, 0.0f);
		m_pLighting->SetPositon(960, 520.0f, 0.0f);

		m_pCharacter->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacter->SetSize(705, 430, 0.0f);
		m_pCharacter->SetPositon(960, 620.0f, 0.0f);

		m_pCharacterShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pCharacterShadow->SetSize(440.0f * 1.5f, 81.0f * 1.5f, 0.0f);
		m_pCharacterShadow->SetPositon(920.0f, 800.0f, 0.0f);

		m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pText->SetSize(913.0f, 145.0f, 0.0f);
		m_pText->SetPositon(960.0f , 100.0f, 0.0f);

		m_pTextShadow->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pTextShadow->SetSize(1129.0f, 73.0f, 0.0f);
		m_pTextShadow->SetPositon((960.0f), 190.0f, 0.0f);

	}

}

// --- 入力処理
void CSceneResult::KeyProsess(void)
{
	// --- 勝利時のボタン処理
	if (ResultGameData.bWin)
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
					if (m_pResultSelectSE)SetVolumeSE(m_pResultSelectSE);
					m_pResultSelectSE->Start();
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
					if (m_pResultSelectSE)SetVolumeSE(m_pResultSelectSE);
					m_pResultSelectSE->Start();


			
				}
				nSelect = 1;
			}
		}
	}
	// --- 敗北時のボタン処理
	else
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
				if (m_pResultSelectSE)SetVolumeSE(m_pResultSelectSE);
				m_pResultSelectSE->Start();



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
				if (m_pResultSelectSE)SetVolumeSE(m_pResultSelectSE);
				m_pResultSelectSE->Start();

	

			}
			nSelect = 1;
		}
	}

	// --- nSelect == 0 のEnter処理
	if (nSelect == 0)
	{
		// StageSelect
		if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN) && !bEnter)
		{
			bEnter = true;
			nPush[nSelect] = 1;
			SetNext(STAGE_SELECT);
		}
	}
	// --- nSelect == 1 のEnter処理
	else
	{
		// retry or Next
		if (ResultGameData.bWin)
		{
			// Next
			if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN) && !bEnter)
			{
				bEnter = true;
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
			if (WithGetKeyTriger(XINPUT_GAMEPAD_A, VK_RETURN ) && !bEnter)
			{
				bEnter = true;
				SetNext(SCENE_GAME, StageLevel);
			}
		}
	}
}

// --- 全体のAnimation管理
void CSceneResult::AnimationManagemer(void)
{
	// --- Animation管理　20FPS
	if (timeGetTime() - nAnimationTimer >= 20.0f)
	{
		bAnimation = true;
		nAnimationFrame++;
		nAnimationTimer = timeGetTime();
	}
	else
	{
		bAnimation = false;
	}
	
	m_pSelectUI->SetSize(640.0f * 1.1f + (cosf((float)nAnimationFrame / 10) * 5.0f), 107.0f * 1.1f + (cosf((float)nAnimationFrame / 10)* 5.0f), 0.0f);

	if (nSelect)
	{
		if (ResultGameData.bWin)
		{
			m_pSelectUI->SetPositon(960.0f + Group.X, 790.0f, 0.0f);
		}
		else
		{
			m_pSelectUI->SetPositon(960.0f, 790.0f, 0.0f);
		}
		for (int nLoop = 0; nLoop < 2; nLoop++)
		{
			m_pStageSelectUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
			m_pNextUI[nLoop]->SetSize(608.0f * 1.1f, 76.0f * 1.1f, 0.0f);
		}
	}
	else
	{
		if (ResultGameData.bWin)
		{
			m_pSelectUI->SetPositon(960.0f + Group.X, 900.0f, 0.0f);
		}
		else
		{
			m_pSelectUI->SetPositon(960.0f, 900.0f, 0.0f);
		}
		for (int nLoop = 0; nLoop < 2; nLoop++)
		{
			m_pStageSelectUI[nLoop]->SetSize(608.0f * 1.1f, 76.0f * 1.1f, 0.0f);
			m_pNextUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
		}
	}
}

// --- カメラ情報の更新
void CSceneResult::SetCamData(void)
{
	m_pTextShadow->SetView(Get2DView());
	m_pTextShadow->SetProjection(Get2DProj());

	m_pBackGround->SetView(Get2DView());
	m_pBackGround->SetProjection(Get2DProj());
	
	m_pUnberUI->SetView(Get2DView());
	m_pUnberUI->SetProjection(Get2DProj());
	
	m_pSelectUI->SetView(Get2DView());
	m_pSelectUI->SetProjection(Get2DProj());
	
	m_pResultData->SetView(Get2DView());
	m_pResultData->SetProjection(Get2DProj());
	
	m_pLighting->SetView(Get2DView());
	m_pLighting->SetProjection(Get2DProj());

	m_pText->SetView(Get2DView());
	m_pText->SetProjection(Get2DProj());

	m_pCharacter->SetView(Get2DView());
	m_pCharacter->SetProjection(Get2DProj());

	m_pBestScore->SetView(Get2DView());
	m_pBestScore->SetProjection(Get2DProj());

	m_pCharacterShadow->SetView(Get2DView());
	m_pCharacterShadow->SetProjection(Get2DProj());

	m_pTextShadow->SetView(Get2DView());
	m_pTextShadow->SetProjection(Get2DProj());

	for (int nLoop = 0; nLoop < NONE_RANK; nLoop++)
	{
		m_pRank[nLoop]->SetView(Get2DView());
		m_pRank[nLoop]->SetProjection(Get2DProj());
	}

	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pStageSelectUI[nLoop]->SetView(Get2DView());
		m_pStageSelectUI[nLoop]->SetProjection(Get2DProj());
		
		m_pNextUI[nLoop]->SetView(Get2DView());
		m_pNextUI[nLoop]->SetProjection(Get2DProj());
	}
}

// --- 描画アニメーション
void CSceneResult::WinAnimation(void)
{
	if (bAnimation)
	{
		int nUvPos = 0;

		if (nAnimationFrame > 63)
		{
			nUvPos = 63;

			float fAnimeFrame = nAnimationFrame - 63;
			// --- イージング速度の変更
			fAnimeFrame = fAnimeFrame * 0.04f;
			if (fAnimeFrame > 1.0f)fAnimeFrame = 1.0f;

			Group.Y = (Easing39(fAnimeFrame,0.1f,0.9f) * 600.0f) - 600.0f;
			Group.X = (Easing39(fAnimeFrame,0.1f,0.9f) * 1920.0f) - 1920.0f;
	
			// --- positionの再設定
			m_pCharacter->SetPositon((460.0f + Group.Y), 350.0f, 0.0f);
			for (int nLoop = 0; nLoop < NONE_RANK; nLoop++)m_pRank[nLoop]->SetPositon(960.0f + Group.X, 450.0f, 0.0f);
			m_pText->SetPositon(960.0f + Group.X, 100.0f, 0.0f);
			m_pLighting->SetPositon((300.0f + Group.Y), 490.0f, 0.0f);
			m_pResultData->SetPositon(1600.0f + Group.X, 520.0f, 0.0f);
			m_pBestScore->SetPositon(1500.0f + Group.X, 230.0f, 0.0f);
			m_pCharacterShadow->SetPositon((330.0f + Group.Y), 850.0f, 0.0f);
			for (int nLoop = 0; nLoop < 2; nLoop++)
			{
				m_pStageSelectUI[nLoop]->SetPositon(960.0f + Group.X, 900.0f, 0.0f);
				m_pNextUI[nLoop]->SetPositon(960.0f + Group.X, 790.0f, 0.0f);
			}
			// --- Animation Loop
			if (bLoopAnime)
			{
				nLoopAnimeCount--;
				if (nLoopAnimeCount < 1)bLoopAnime = false;

			}
			else
			{
				nLoopAnimeCount++;
				if (nLoopAnimeCount > 15)bLoopAnime = true;
			}


			nUvPos -= nLoopAnimeCount;

		}
		else
		{
			nUvPos = nAnimationFrame;
		}

		int nShiftUvX;
		int nShiftUvY;

		nShiftUvX = nUvPos % 8;
		nShiftUvY = nUvPos / 8;

		fUvPos.X = (1.0f / 8.0f) * nShiftUvX;
		fUvPos.Y = (1.0f / 8.0f) * nShiftUvY;


	}

}

void CSceneResult::LoseAnimation(void)
{
	if (bAnimation)
	{
		float fAnime;
		fAnime = nAnimationFrame * 0.02f;
		m_pCharacter->SetPositon(960, 620.0f - Easing39(fAnime,0.3f,0.9f) * 1920.0f + 1920.0f , 0.0f);
	}
}

// --- 描画
void CSceneResult::WinDisp(void)
{
	m_pLighting->SetTexture();
	m_pLighting->Disp();

	m_pTextShadow->SetTexture();
	m_pTextShadow->Disp();

	m_pText->SetTexture();
	m_pText->Disp();

	m_pCharacterShadow->SetTexture();
	m_pCharacterShadow->Disp();

	m_pCharacter->SetUvSize(1.0f / 8.0f, 1.0f / 8.0f);
	m_pCharacter->SetUvPos(fUvPos.X, fUvPos.Y);
	m_pCharacter->SetTexture();
	m_pCharacter->Disp();
	m_pCharacter->SetUvSize(1.0f, 1.0f);
	m_pCharacter->SetUvPos(0.0f, 0.0f);

	m_pRank[NowRank]->SetTexture();
	m_pRank[NowRank]->Disp();

	if (bBestScore)
	{
		m_pBestScore->SetTexture();
		m_pBestScore->Disp();
	}
}

void CSceneResult::LoseDisp(void)
{
	m_pLighting->SetTexture();
	m_pLighting->Disp();

	m_pCharacterShadow->SetTexture();
	m_pCharacterShadow->Disp();

	m_pCharacter->SetTexture();
	m_pCharacter->Disp();

	m_pTextShadow->SetTexture();
	m_pTextShadow->Disp();

	m_pText->SetTexture();
	m_pText->Disp();
}

void CSceneResult::NumberDisp(void)
{
	
	m_pNumber->SetArrangment(CNumberUI::Left_AL);
	m_pNumber->SetNumberColor(2);

	int nMin, nSec;
	nMin = nSec = 0;

	nMin = ResultGameData.nTime / 60;
	nSec = ResultGameData.nTime % 60;

	nMinutes++;
	nSeconds++;

	if (nMinutes > nMin)
	{
		nMinutes = nMin;
		bTimeProsess = true;
	}
	else
	{
		bTimeProsess = false;
	}
	if (nSeconds > nSec)
	{
		nSeconds = nSec;
		bTimeProsess = true;
	}
	else
	{
		bTimeProsess = false;
	}
		
	m_pNumber->SetNumber(nMinutes);
	m_pNumber->SetLend(2);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1790.0f + Group.X,390.f,0.0f });
	m_pNumber->Draw();

	m_pNumber->SetNumber(nSeconds);
	m_pNumber->SetLend(2);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1880.0f + Group.X,390.f,0.0f });
	m_pNumber->Draw();

	if (bTimeProsess)
	{
		nHitPoint++;
		if (nHitPoint > ResultGameData.nHitPoint)
		{
			bHPProsess = true;
			nHitPoint = ResultGameData.nHitPoint;
		}
	}

	m_pNumber->SetNumber(nHitPoint);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1820.0f + Group.X,475.f,0.0f });
	m_pNumber->Draw();

	if (bHPProsess)
	{
		nSpawn++;
		if (nSpawn > ResultGameData.nSpawnCount)
		{
			bSpwanProsess = true;
			nSpawn = ResultGameData.nSpawnCount;
		}
	}

	m_pNumber->SetNumber(nSpawn);
	m_pNumber->SetScale({ 50.0f,50.0f,0.0f });
	m_pNumber->SetPos({ 1820.0f + Group.X,555.f,0.0f });
	m_pNumber->Draw();

	if (bSpwanProsess)
	{
		nAverage++;
		nDispCount++;
		if (nAverage > ResultGameData.nAverageSpwn)nAverage = ResultGameData.nAverageSpwn;
		if (nDispCount > ResultGameData.nDrawCount)nDispCount = ResultGameData.nDrawCount;
	}

	m_pNumber->SetNumber(nAverage);
	m_pNumber->SetScale({ 25.0f,25.0f,0.0f });
	m_pNumber->SetPos({ 1820.0f + Group.X,615.f,0.0f });
	m_pNumber->Draw();

	m_pNumber->SetNumber(nDispCount);
	m_pNumber->SetScale({ 25.0f,25.0f,0.0f });
	m_pNumber->SetPos({ 1820.0f + Group.X,650.f,0.0f });
	m_pNumber->Draw();

	m_pNumber->SetNumber(nScore);
	m_pNumber->SetScale({ 90.0f,90.0f,0.0f });
	m_pNumber->SetPos({ 1820.0f + Group.X,300.0f,0.0f });
	m_pNumber->Draw();
}
