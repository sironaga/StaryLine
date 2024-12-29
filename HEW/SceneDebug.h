#pragma once

#include "DebugRoom.h"
#include "Scene.h"

class CSceneDebug : public CScene
{
public:
	CSceneDebug();
	virtual ~CSceneDebug();
	virtual void Update()override;
	virtual void Draw()override;

private:
	CDebugRoom* m_pDebugRoom;
};