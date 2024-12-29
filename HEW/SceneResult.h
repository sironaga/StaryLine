#pragma once

#include "Scene.h"

class CSceneResult : public CScene
{
public:
	CSceneResult();
	virtual ~CSceneResult();
	virtual void Update()override;
	virtual void Draw()override;
private:

};