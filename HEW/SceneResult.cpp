// --- Include
#include "SceneResult.h"
#include"Input.h"
#include"InputEx.h"
#include"Controller.h"
#include"InputEx.h"
#include"Easing.h"
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
	:nSelect(0), nAnimationFrame(0), bScore(true), bBestScore(false), bWorldClear(false), nPush{},bAnimation(false),fPiyoA(0.0f),bEnter(false)
{
	ResultGameData.bWin = 1;
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
	Conect_Init();

}
CSceneResult::~CSceneResult()
{

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
	m_pStageSelectUI[nPush[1]]->SetTexture();
	m_pStageSelectUI[nPush[1]]->Disp();

	m_pNextUI[nPush[0]]->SetTexture();
	m_pNextUI[nPush[0]]->Disp();

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
	ResultGameData.nAverageSpwn = 0;
	ResultGameData.nDrawCount	= 0;
	ResultGameData.nHitPoint	= 0;
	ResultGameData.nSpawnCount	= 0;
	ResultGameData.nTime		= 0;
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

	if (ResultGameData.bWin)
	{
		m_pNextUI[0] = new SpriteEx("Assets/Texture/Result/UI/Result_03.png");
		m_pNextUI[1] = new SpriteEx("Assets/Texture/Result/UI/Result_04.png");

		m_pText		= new SpriteEx("Assets/Texture/Result/Text/Result_Win_Text_top.png");
		m_pLighting = new SpriteEx("Assets/Texture/Result/Lighting/Result_Win_Light.png");

	}
	else
	{
		m_pNextUI[0] = new SpriteEx("Assets/Texture/Result/UI/Result_05.png");
		m_pNextUI[1] = new SpriteEx("Assets/Texture/Result/UI/Result_06.png");

		m_pLighting = new SpriteEx("Assets/Texture/Result/Lighting/Result_Win_Light.png");
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
			m_pStageSelectUI[nLoop]->SetPositon(960.0f, 900.0f, 0.0f);

			m_pNextUI[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pNextUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
			m_pNextUI[nLoop]->SetPositon(960.0f, 790.0f, 0.0f);
		}

		m_pSelectUI->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pSelectUI->SetSize(670.0f * 1.1f, 107.0f * 1.1f, 0.0f);
		m_pSelectUI->SetPositon(960.0f, 900.0f, 0.0f);

		m_pResultData->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pResultData->SetSize(710.0f * 0.9, 390.0f * 0.9, 0.0f);
		m_pResultData->SetPositon(1600.0f, 520.0f, 0.0f);

		m_pLighting->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pLighting->SetSize(969, 982, 0.0f);
		m_pLighting->SetPositon(300.0f, 490.0f, 0.0f);

		m_pText->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pText->SetSize(822.0f, 143.0f, 0.0f);
		m_pText->SetPositon(960.0f, 100.0f, 0.0f);

		for (int nLoop = 0; nLoop < NONE_RANK; nLoop++)
		{
			m_pRank[nLoop]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_pRank[nLoop]->SetSize(445.0f, 446.0f, 0.0f);
			m_pRank[nLoop]->SetPositon(960.0f, 520.0f, 0.0f);
		}

	}
	else
	{

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
					if (m_pResultSelectSE)SetVolumeBGM(m_pResultSelectSE);
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
					if (m_pResultSelectSE)SetVolumeBGM(m_pResultSelectSE);
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
				if (m_pResultSelectSE)SetVolumeBGM(m_pResultSelectSE);
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
				if (m_pResultSelectSE)SetVolumeBGM(m_pResultSelectSE);
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
		m_pSelectUI->SetPositon(960.0f, 790.0f, 0.0f);

		for (int nLoop = 0; nLoop < 2; nLoop++)
		{
			m_pStageSelectUI[nLoop]->SetSize(608.0f, 76.0f, 0.0f);
			m_pNextUI[nLoop]->SetSize(608.0f * 1.1f, 76.0f * 1.1f, 0.0f);
		}
	}
	else
	{
		m_pSelectUI->SetPositon(960.0f, 900.0f, 0.0f);

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
	m_pLighting->SetTexture();
	m_pLighting->Disp();

	m_pText->SetTexture();
	m_pText->Disp();

	m_pRank[A_RANK]->SetTexture();
	m_pRank[A_RANK]->Disp();
}

void CSceneResult::LoseAnimation(void)
{

}

// --- 描画
void CSceneResult::WinDisp(void)
{

}

void CSceneResult::LoseDisp(void)
{

}

void CSceneResult::NumberDisp(void)
{

}
