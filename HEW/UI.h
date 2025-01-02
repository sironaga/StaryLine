#ifndef __UI_H__
#define __UI_H__
#define _DIRECTX_

#include "SpriteEx.h"

class CUI
{
public:

	CUI();
	virtual ~CUI();
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:

};

#endif