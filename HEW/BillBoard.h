#pragma once

#include "Camera.h"
#include "DirectX.h"

void InitBillBoard();
DirectX::XMMATRIX SetBillBoard(Camera* camera, DirectX::XMMATRIX world);