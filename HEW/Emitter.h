// Emitter.h
// ï“èWé“ AT12A07_è¨êÏò@
// 
// 
// 
//
#pragma once
#include "Particle.h"

#define MAX_EFFECT (100)

class Emitter
{
public:
	Emitter(ID3D11ShaderResourceView* pTex, float x, float y);
	Emitter(CModelEx* Model, float x, float y, float z);
	~Emitter();
	void Update();
	void Draw();

private:
	CModelEx* m_pModel[MAX_EFFECT];
	ID3D11ShaderResourceView* m_pTex[MAX_EFFECT];
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
};

