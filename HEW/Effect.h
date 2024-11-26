// Effect.h
// 編集者 AT12A07_小川蓮
// 
// 
// 
//
#pragma once
#include "Emitter.h"
#define MAX_EFFECT (100)

class CEffect
{
public:
	CEffect(float x, float y, int type); // 2Dエフェクトのコンストラクタ
	CEffect(float x, float y, float z, int type); // 3Dエフェクトのコンストラクタ
	~CEffect();

	void CreateEffect();
	void Update();
	void Draw();
	bool EffectTime();

private:
	
	int m_nTime; //エフェクトの持続時間

};

