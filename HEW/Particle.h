// Particle.h
// 編集者 AT12A07_小川蓮
// 
// 
// 
//
#pragma once
#include "ModelEx.h"


class CParticle
{
public:
	CParticle(ID3D11ShaderResourceView* pTex);
	CParticle(CModelEx* pModel);
	~CParticle();
	void Create(float Time, float x, float y, float z, float sx, float sy, float sz);
	void Update();
	void Draw();
	bool ParticleTime();

private:
	ID3D11Buffer* m_pVtx; // 頂点バッファ
	ID3D11ShaderResourceView* m_pTex; // テクスチャ
	float m_fPosX, m_fPosY, m_fPosZ; // x,y,z座標
	float m_fSpeedX, m_fSpeedY, m_fSpeedZ; // x,y,z方向の移動速度
	int m_nParticleTime; // パーティクルの持続時間

};

