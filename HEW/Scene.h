#pragma once

#include "Fade.h"

class CScene 
{
public:
	CScene();
	virtual ~CScene();

	// シーン内で継承先の更新処理とフェードの処理を呼び出すための更新関数 
	void RootUpdate();

	// シーン内で継承先の更新処理とフェードの処理を呼び出すための描画関数 
	void RootDraw();

	// 継承先のクラスで実装する更新処理と描画処理 
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// シーンで実行するフェードクラスを設定 
	void SetFade(CFade* fade);

	// 基本クラスでは、フェードアウトの終了を検知してシーンの切り替えを有効にする 
	virtual bool ChangeScene();

	// 次の切り替え先シーンを取得 
	int NextScene();

	// 切り替え先のシーンを設定 
	void SetNext(int next);
protected:
	CFade* m_pFade; // フェード処理クラス 
	int  m_next;  // 切り替え先のシーン 
};