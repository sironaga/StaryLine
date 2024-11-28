#include "BillBoard.h"

void InitBillBoard()
{

}

DirectX::XMMATRIX SetBillBoard(Camera* camera, DirectX::XMMATRIX world)
{
	DirectX::XMMATRIX view_mat = camera->GetView();
	view_mat = DirectX::XMMatrixInverse(nullptr, view_mat);
	// view_mat = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	return view_mat * world;
}
