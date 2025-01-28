#pragma once
#include "Scene.h"



class SaverMode : public CScene
{
public:
	SaverMode();
	virtual ~SaverMode();
	virtual void Update()override;
	virtual void Draw()override;
private:
	int nScore[10];
};

